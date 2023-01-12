// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* The lexical analyzer returns a double floating point
number on the stack and the token NUM, or the numeric code
of the character read if not a number.  It skips all blanks
and tabs, and returns 0 for end-of-input.  */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lexer.h"

#include "node.h"	// for yyparam
#include "dm.tab.hpp"
#include "exceptions.h"
#include "filenamecache.h"
#include "context.h"
#include "dm.h"

#include "platform.h"


typedef struct tagKeyword {
	const char *keyword;
	int token;
	const char *debug;
} Keyword;

/**
 * Table of keywords - these are essentially special identifiers
 * This table is sorted alphabetically for efficiency and the lexer code relies
 * upon this fact.
 */
Keyword keywordTable[] = {
	{ "action",		T_ACTION,	"T_ACTION" },
	{ "break",		T_BREAK,	"T_BREAK" },
	{ "case",		T_CASE,		"T_CASE" },
	{ "catch",		T_CATCH,	"T_CATCH" },
	{ "continue",	T_CONTINUE,	"T_CONTINUE" },
	{ "decr",		T_DECR,		"T_DECR" },
	{ "default",	T_DEFAULT,	"T_DEFAULT" },
	{ "echo",		T_ECHO,		"T_ECHO" },
	{ "else",		T_ELSE,		"T_ELSE" },
	{ "eval",		T_EVAL,		"T_EVAL" },
	{ "false",		T_BOOL,		"false" },
	{ "finally",	T_FINALLY,	"T_FINALLY" },
	{ "for",		T_FOR,		"T_FOR" },
	{ "foreach",	T_FOREACH,	"T_FOREACH" },
	{ "function",	T_FUNC,		"T_FUNC" },
	{ "if",			T_IF,		"T_IF" },
	{ "in",			T_IN,		"T_IN" },
	{ "incr",		T_INCR,		"T_INCR" },
	{ "iterate",	T_ITERATE,	"T_ITERATE" },
	{ "null",       T_NULL,     "T_NULL" },
	{ "parallel",	T_PARALLEL,	"T_PARALLEL" },
	{ "post",		T_POST,		"T_POST" },
	{ "pre",		T_PRE,		"T_PRE" },
	{ "psloop",		T_PSLOOP,	"T_PSLOOP" },
	{ "return",		T_RETURN,	"T_RETURN" },
	{ "sequential",	T_SEQUENTIAL, "T_SEQUENTIAL" },
	{ "set",		T_SET,		"T_SET" },
	{ "switch",		T_SWITCH,	"T_SWITCH" },
	{ "true",		T_BOOL,		"true" },
	{ "try",		T_TRY,		"T_TRY" },
	{ "using",		T_USING,	"T_USING" },
	{ "while",		T_WHILE,	"T_WHILE" },
	{ NULL, 0, NULL }
};


void lexdebug(const char *fmt, ...)
{
	const char *lexdebuglog = getenv("trilexdebug");
	if(lexdebuglog) {
		FILE *f = fopen(lexdebuglog, "a");
		va_list args;
		va_start(args, fmt);
		vfprintf(f, fmt, args);
		va_end(args);
		fclose(f);
	}
}


bool ishexdigit(int c)
{
	return ((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f'));
}

int hexvalue(int c)
{
	if((c >= '0') && (c <= '9')) {
		return c - '0';
	}
	if((c >= 'A') && (c <= 'F')) {
		return c - 'A' + 10;
	}
	if((c >= 'a') && (c <= 'f')) {
		return c - 'a' + 10;
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////


LexerBuffer::LexerBuffer(LexerType type, const char *str, const char *filename)
	: m_type(type), m_buf(str), // m_filename(filename),
	  m_pos(0), m_peek(0), m_firstToken(true), m_noSpace(false),
	  m_wasSet(false), m_ppIf(false)
{
//if(type == expr_lexer) { printf("'%s'\n", str); }
	const char *leafname = NULL;
	if(filename) {
		// Just print the leafname if we can
		const char *ls = strrchr(filename, DIR_SEP_CHAR);
		leafname = (ls ? ++ls : filename);
	}

	yylloc.first_column = yylloc.last_column = 0;
	yylloc.first_line   = yylloc.last_line   = 1;
	yylloc.file_num     = (leafname ? FilenameCache::addFilename(leafname) : 0);
	yylloc.frag_num     = 0;
}


int LexerBuffer::fileno()
{
	return yylloc.file_num;
}


int LexerBuffer::lineno()
{
	return yylloc.last_line;
}


int LexerBuffer::fragno()
{
	return yylloc.frag_num;
}


int LexerBuffer::getc()
{
	if(!m_buf[m_pos]) {
//printf("getc: EOF\n");
		return EOF;
	}

	int c = m_buf[m_pos];
	m_peek = ++m_pos;
	if(c == '\n') {
		yylloc.last_column = 0;
	} else if(c != '\r') {
		yylloc.last_column++;
	}
	return c;
}


int LexerBuffer::peekc()
{
	if(!m_buf[m_peek]) {
//printf("peekc: EOF\n");
		return EOF;
	}

	return m_buf[m_peek++];
}


void LexerBuffer::scanint(int *res)
{
	if(!m_buf[m_pos]) {
		*res = 0;
		return;
	}

	int read = sscanf(&m_buf[m_pos], "%d", res);
	while(isdigit(m_buf[m_pos])) {
		++m_pos;
		++yylloc.last_column;
	}
	m_peek = m_pos;
}


void LexerBuffer::ungetc()
{
	m_peek = --m_pos;
	--yylloc.last_column;
}


void LexerBuffer::unpeekc()
{
	--m_peek;
}


void LexerBuffer::munch()
{
	yylloc.last_column += (m_peek - m_pos);
	m_pos = m_peek;
}


/**
 * Handling for string literals:
 * non-expanding: 'my string literal'  'C:\\Temp'  'O\'Malley'
 * expanding:     "my ${string} literal"  "with \"quotes\""
 * no-escape:     @'C:\Temp'
 */
int LexerBuffer::parseStringLiteral(char quote, bool noEscape)
{
	bool dynamic = false;
   	char buf[256];
	char *ptr = buf;
   	int n = 0;
	int cap = sizeof(buf);
	int c;

	do {
   		for(c = this->getc(); (c != quote) && (c != EOF); c = this->getc()) {
			// If we fill buffer switch to dynamic mode and increase size
			if(n > (cap - 2)) {
				cap += 256;
				if(dynamic) {
					ptr = (char*) realloc(ptr, cap);
				} else {
					dynamic = true;
					ptr = (char*) malloc(cap);
					strncpy(ptr, buf, n);
				}
			}
			if((c == '\r') || (c == '\n')) {
				// Check for newlines - we cannot recover if we find one
				throw SyntaxError(*this, "Unterminated string literal");
			}
			if(!noEscape && (c == '\\')) {
				switch(this->peekc()) {
				case 't':
					// Replace \t with the tab character
					c = '\t';
					this->munch();
					break;
				case 'n':
					// Replace \n with the newline character
					c = '\n';
					this->munch();
					break;
				case 'r':
					// Replace \r with the cr character
					c = '\r';
					this->munch();
					break;
				case 'u': {
					// Replace \u0123 with character, but if in ASCII range 20-7F
					int c1 = this->peekc();
					int c2 = this->peekc();
					int c3 = this->peekc();
					int c4 = this->peekc();
					if(c1 == '0' && c2 == '0' && ishexdigit(c3) && ishexdigit(c4)) {
						int t = hexvalue(c3) * 16 + hexvalue(c4);
						if((t > 32) && (t < 127)) {
							// Replace with calculated char
							c = t;
							this->munch();
							break;
						}
					}
					// Unget the four hex digits and the 'u'
					this->unpeekc();
					this->unpeekc();
					this->unpeekc();
					this->unpeekc();
					this->unpeekc();
					}
					break;
				case '\\':
				case '\'':
				case '"':
					// Remove the '\' from escaped quotes and keep the quote
					// Replace '\\' with single '\'
					c = this->getc();
					break;
				}
			}
   			ptr[n++] = c;
   		}
   		ptr[n] = '\0';

		// Now look to see if the next token is another string literal - skip
		// whitespace and if next character is a quote, concatenate, as in C/C++
		// We remember the line number and reset if we don't eat the next token
		int old_last_line = yylloc.last_line;
		while ((((c = this->peekc())) == ' ') || (c == '\t') || (c == '\r') || (c == '\n')) {
			if(c == '\n') {
				++yylloc.last_line;
			}
		}

		if(c == quote) {
			if(yylloc.last_line != old_last_line) { lexdebug("\n%02ds: ", yylloc.last_line); }
			this->munch();
		} else {
			yylloc.last_line = old_last_line;
		}
	} while(c == quote);


	yylval.str = dynamic ? ptr : strdup(buf);
	lexdebug((noEscape ? "@%c%s%c " : "%c%s%c "), quote, yylval.str, quote);
	return (quote == '\'') ? STR : STR2;
}


void LexerBuffer::readPreprocessingDirective(int c, char *pdbuf, char *ppbuf)
{
	int n = 0;
	while((n < 256) && (c != EOF) && ((c == '_') || isalnum(c))) {
		pdbuf[n++] = c;
		c = this->peekc();
	}
	pdbuf[n] = '\0';
	this->munch();
	//debug1("preprocessing directive: %s", pdbuf);

	n = 0;
	while((n < 256) && (c != EOF) && (c != '\r') && (c != '\n')) {
		ppbuf[n++] = c;
		c = this->peekc();
	}
	if((c == '\r') || (c == '\n')) {
		// Put back the newline so as not to break the line numbering
		this->unpeekc();
	}
	ppbuf[n] = '\0';
	this->munch();
	//debug1("preprocessing param: %s", ppbuf);
}


void LexerBuffer::parsePreprocessingDirective(int c)
{
	char pdbuf[256], ppbuf[256];
	readPreprocessingDirective(c, pdbuf, ppbuf);

	if(strcmp(pdbuf, "frag") == 0) {
		// Fragment marker - update the current fragment
		int frag = atoi(ppbuf);
		debug1("fragment %d", frag);
		yylloc.frag_num = frag;
	//} else if(strcmp(pdbuf, "if") == 0) {
	//	if(m_ppIf) {
	//		throw SyntaxError(*this, "Nesting of #if's is not allowed");
	//	}
	//	int cond = atoi(ppbuf);
	//	if(cond == 0) {
	//		// skip until we find matching #endif
	//		bool sol = true;
	//		while((c = this->peekc()) != EOF) {
	//			if(sol && (c == '#')) {
	//				// another preprocessing directive
	//				this->munch();
	//				readPreprocessingDirective(c, pdbuf, ppbuf);
	//				if(strcmp(pdbuf, "if") == 0) {
	//					throw SyntaxError(*this, "Nesting of #if's is not allowed");
	//				} else if(strcmp(pdbuf, "endif") == 0) {
	//					// found matching #endif
	//					break;
	//				}
	//			}
	//			if(c == '\n') {
	//				++yylloc.last_line;
	//			}
	//			sol = ((c == '\r') || (c == '\n')) ? true : false;
	//		}
	//	} else {
	//		// record that we expect an #endif
	//		m_ppIf = true;
	//	}
	//} else if(strcmp(pdbuf, "endif") == 0) {
	//	// check that we were in an #if
	//	if(m_ppIf) {
	//		m_ppIf = false;
	//	} else {
	//		throw SyntaxError(*this, "Unexpected #endif");
	//	}
	} else {
		throw SyntaxError(*this, "Unrecognized directive '%s'", pdbuf);
	}
}


int LexerBuffer::yylex()
{
	int c;

	// Inject a first token to start the parser down the right subset of the grammar
	if(m_firstToken) {
		m_firstToken = false;
		switch(m_type) {
		case expr_lexer: return START_EXPR;
		case eval_lexer: return START_STMTLIST;
		default: return START_ACTION;
		}
	}

	if(m_noSpace) {
		m_noSpace = false;
		lexdebug("NOWS ");
		return NOWS;
	}

next_token:
	yylloc.first_column = yylloc.last_column;

	/* Skip white space.  */
	while ((((c = this->getc())) == ' ') || (c == '\t') || (c == '\r') || (c == '\n')) {
		if(c == '\n') {
			++yylloc.last_line;
			lexdebug("\n%02dw: ", yylloc.last_line);
		}
		yylloc.first_column = yylloc.last_column;
	}

	yylloc.first_line = yylloc.last_line;

	if(m_wasSet && (c != '-')) {
		m_wasSet = false;
	}

	if((yylloc.first_column == 0) && (c == '#')) {
		// pre-processing directive - these don't get passed back to the parser
		c = this->peekc();
		if(isalpha(c)) {
			parsePreprocessingDirective(c);
			goto next_token;
		}
		lexdebug("# ");
		return '#';
	}

	switch(c)
	{
	case '$':
		c = this->peekc();
		switch(c) {
		case '{':
			this->munch();
			lexdebug("${ ");
			return DOLBR;
		case '?':
		case '$':
		case '!': {
			this->munch();
			char buf[2];
			buf[0] = c;
			buf[1] = '\0';
			yylval.str = strdup(buf);
			lexdebug("$%s ", buf);
			return DOLIDENT;
			}
		}
		lexdebug("$ ");
		return '$';

	case ':':
		c = this->peekc();
		switch(c) {
		case '+':
			this->munch();
			lexdebug(":+ ");
			return CLPL;
		case '-':
			this->munch();
			lexdebug(":- ");
			return CLMI;
		}
		lexdebug(": ");
		return ':';

	case '!':
		c = this->peekc();
		if(c == '=') {
			this->munch();
			lexdebug("!= ");
			return NE;
		}
		lexdebug("! ");
		return '!';

	case '>':
		c = this->peekc();
		switch(c) {
		case '=':
			this->munch();
			lexdebug(">= ");
			return GE;
		case '>':
			this->munch();
			lexdebug(">> ");
			return REDIR;
		}
		lexdebug("> ");
		return '>';

	case '<':
		c = this->peekc();
		if(c == '=') {
			this->munch();
			lexdebug("<= ");
			return LE;
		}
		lexdebug("< ");
		return '<';

	case '=':
		c = this->peekc();
		if(c == '>') {
			this->munch();
			lexdebug("=> ");
			return MAP;
		}
		lexdebug("= ");
		return '=';

	case '+':
		c = this->peekc();
		if(c == '=') {
			this->munch();
			lexdebug("+= ");
			return PE;
		}
		lexdebug("+ ");
		return '+';

	case '-':
		// We don't do special conversion when handling 'set' options
		if(!m_wasSet) {
			c = this->peekc();
			switch(c) {
			case 'a':
				this->munch();
				lexdebug("-a ");
				return '&';
			case 'o':
				this->munch();
				lexdebug("-o ");
				return '|';
			case 'n':
				c = this->peekc();
				if(c == 'e') { this->munch(); lexdebug("-ne "); return NE; }
				break;
			case 'g':
				c = this->peekc();
				if(c == 'e') { this->munch(); lexdebug("-ge "); return GE; }
				if(c == 't') { this->munch(); lexdebug("-gt "); return '>'; }
				break;
			case 'l':
				c = this->peekc();
				if(c == 'e') { this->munch(); lexdebug("-le "); return LE; }
				if(c == 't') { this->munch(); lexdebug("-lt "); return '<'; }
				break;
			case 'e':
				c = this->peekc();
				if(c == 'q') { this->munch(); lexdebug("-eq "); return '='; }
				break;
			case '=':
				this->munch();
				lexdebug("-= ");
				return ME;
			}
		}
		lexdebug("- ");
		return '-';

	case '*':
		c = this->peekc();
		if(c == '=') {
			this->munch();
			lexdebug("*= ");
			return TE;
		}
		lexdebug("* ");
		return '*';

	case '}':
		c = this->peekc();
	   	if((c == '$') || (c == '_') || isalnum(c)) {
	   		m_noSpace = true;
	   	}
		lexdebug("} ");
		return '}';


	case '@':
		// no-escape string literals
		c = this->peekc();
		if((c != '\'') && (c != '"')) {
			lexdebug("@ ");
			return '@';
		}
		this->munch();
		return parseStringLiteral(c, true);

	case '\'':
	case '"':
		// non-expanding and expanding string literals
		return parseStringLiteral(c, false);

	case '/':
		// C++-style comments
		c = this->peekc();
	   	if(c == '/') {
			while( ((c = this->peekc()) != '\n') && (c != '\r') && (c != EOF));
			this->unpeekc();
			this->munch();
			lexdebug("COMMENT ");
			goto next_token;
	   	}
		// C-style comments
		if(c == '*') {
			while(c != EOF) {
				if((c = this->peekc()) == '*') {
					if((c = this->peekc()) == '/') {
						this->munch();
						lexdebug("COMMENT2 ");
						goto next_token;
					}
				}
				if(c == '\n') {
					++yylloc.last_line;
					lexdebug("\n%02dc: ", yylloc.last_line);
				}
			}
			// Unterminated comment
			throw SyntaxError(*this, "Unterminated C-style comment");
		}
		lexdebug("/ ");
		return '/';
	}

	// Numbers
	if(isdigit(c))
	 {
	   this->ungetc();
	   this->scanint(&yylval.ival);
	   lexdebug("%d ", yylval.ival);
	   return NUM;
	 }

	// Identifiers
	if((c == '_') || isalpha(c)) {
	   char buf[256];
	   int n = 0;
	   while((n < 256) && (c != EOF) && ((c == '_') || isalnum(c))) {
	   	buf[n++] = c;
	   	c = this->peekc();

		// Keep parsing if identifier is dep.xxx - don't like this, but maintains compatability
		if((n == 3) && (c == '.')) {
			if(strncmp(buf, "dep", 3) == 0) {
	   			buf[n++] = c;
	   			c = this->peekc();
			}
		}
	   }
	   if(n < 256) {
	   		if(c == '$') {
	   			m_noSpace = true;
	   		}
			if(c != EOF) {
	     		this->unpeekc();
			}
	   }
	   this->munch();
	   buf[n] = '\0';

	   Keyword *k;
	   // Find first keyword with same start letter
	   for(k = keywordTable; k && k->keyword && (k->keyword[0] < buf[0]); k++) { /*empty*/ }
	   // Now compare only those keywords with the same start letter
	   for(; k && k->keyword && (k->keyword[0] == buf[0]); k++) {
		   if(strcmp(buf, k->keyword) == 0) {
			   lexdebug("%s ", k->debug);
			   if(k->token == T_SET) {
				   m_wasSet = true;
			   }
			   if(k->token == T_BOOL) {
				   yylval.bval = (strcmp(buf, "true") == 0) ? true : false;
			   }
			   return k->token;
		   }
	   }

	   yylval.str = strdup(buf);
	   lexdebug("%s ", buf);
	   return IDENT;
	}

	/* Return end-of-input.  */
	if (c == EOF) {
		//fprintf(stderr, "End of input\n");
	 	return 0;
	}

	/* Return a single char.  */
	if(c != '\n') {	lexdebug("%c ", c); }
	return c;
}


char *LexerBuffer::getErrorLine(int lineno)
{
	int pos, line = 1;
	for(pos = 0; m_buf[pos] && (line < lineno); pos++) {
		if(m_buf[pos] == '\n') {
			line++;
		}
	}
//printf("DEBUG: pos = %d; line = %d\n", pos, line);

	if(!m_buf[pos]) {
		return NULL;
	}

	int end;
	for(end = pos+1; m_buf[end] && (m_buf[end] != '\n'); end++);

//printf("DEBUG: end = %d\n", end);

	char *ret = (char*) malloc((end - pos) + 2);
	if (ret) {
		strncpy(ret, &m_buf[pos], (end - pos) + 1);
		ret[(end - pos)] = '\0';
	}
	return ret;
}


///////////////////////////////////////////////////////////////////////////////


/* Called by yyparse on error.  */
void yyerror(void *yyparam, char const *s)
{
	LexerBuffer *lb = ((yyparse_param*) yyparam)->buffer;

	char *line = lb->getErrorLine(yylloc.first_line);
	if(line) {
//printf("DEBUG: first_column = %d; len = %d\n", yylloc.first_column, strlen(line));
		if(yylloc.first_column <= strlen(line)) {
			int toklen = yylloc.last_column - yylloc.first_column;
			if(toklen > 0) {
				line = (char*) realloc(line, strlen(line) + 7);
				char *tok = (char*) malloc(toklen+1);
				if (tok) {
					strncpy(tok, &line[yylloc.first_column], toklen);
					tok[toklen] = '\0';
					char *rol = strdup(&line[yylloc.last_column]);
					sprintf(&line[yylloc.first_column], ">>>%s<<<%s", (const char*) tok, (const char*) rol);
					SAFE_FREE(tok);
					SAFE_FREE(rol);
				}
			}
		}
		//fprintf(stderr, "%s\n", line);
		((yyparse_param*) yyparam)->ex = new SyntaxError(*lb, "%s\n%s", line, s);
		SAFE_FREE(line);
	} else {
		((yyparse_param*) yyparam)->ex = new SyntaxError(*lb, "%s", s);
	}

	//fprintf(stderr, "%s at line %d\n", s, yylloc.first_line);
}


extern int yyparse(void *buffer);

Expr *LexerBuffer::parseExpression(Context &ctx)
{
	Expr *ret = NULL;
	yyparse_param param;
	param.buffer = this;
	int res = yyparse(&param);
	if((res == 0) && param.expr) {
		ret = param.expr->evaluate(ctx, false);
		delete param.expr;
	} else {
		if(param.ex) {
			param.ex->print(ctx.dm());
			throw *(param.ex->clone());
		}
	}
	return ret;
}


/**
 * yylex fn which is called by yyparse().
 */
int yylex(LexerBuffer *buffer)
{
	return buffer ? buffer->yylex() : EOF;
}
