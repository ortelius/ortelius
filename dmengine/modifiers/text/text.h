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

#ifndef __modifiers_text_h

#define __modifiers_text_h


#include "../../dmapi/modify.h"


class TextModifyProviderImpl : public ModifyProviderImpl
{
private:
	char *m_data;
	long m_size;

public:
	TextModifyProviderImpl(ExtendedStmt &parent);
	~TextModifyProviderImpl();

	void loadFile(const char *infile, class Context &ctx); 
	void saveFile(const char *outfile, class Context &ctx);

	void replace(class ExtendedStmt &stmt, class Context &ctx);

	void executeSubStmt(class ExtendedStmt &stmt, class Context &ctx);
};


class TextModifyProviderImplFactory : public ModifyProviderImplFactory
{
public:
	TextModifyProviderImplFactory();
	~TextModifyProviderImplFactory();

	ModifyProviderImpl *create(ExtendedStmt &parent);
};


#endif /*__modifiers_text_h*/
