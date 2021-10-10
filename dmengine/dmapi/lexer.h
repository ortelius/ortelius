/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
