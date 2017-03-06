/*
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __lexer_h

#define __lexer_h


#include "modelbase.h"
#include "dmapiexport.h"


typedef enum tagLexerType {
	action_lexer,
	expr_lexer,
	eval_lexer
} LexerType;


class DMAPI_API LexerBuffer : public ILocatable
{
private:
	LexerType m_type;
	const char *m_buf;
	//const char *m_filename;
	int m_pos;
	int m_peek;
	bool m_firstToken;
	bool m_noSpace;
	bool m_wasSet;
	bool m_ppIf;

	int parseStringLiteral(char quote, bool noEscape);
	void readPreprocessingDirective(int c, char *pdbuf, char *ppbuf);
	void parsePreprocessingDirective(int c);

public:
	LexerBuffer(LexerType type, const char *str, const char *filename);

	//const char *filename()  { return m_filename; }

	// ILocatable
	int fileno();
	int lineno();
	int fragno();

	int getc();
	int peekc();
	void scanint(int *res);
	void ungetc();
	void unpeekc();
	void munch();

	int yylex();

	char *getErrorLine(int lineno);

	class Expr *parseExpression(class Context &ctx);
};

#endif /*__lexer_h*/
