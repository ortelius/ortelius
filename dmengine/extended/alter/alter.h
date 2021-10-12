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

#ifndef __extended_alter_h

#define __extended_alter_h


#include "../../dmapi/extended.h"


//////////////////////////////////////////////////////////////////////////////
// Alter
//////////////////////////////////////////////////////////////////////////////

class AlterStmtImpl : public ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	AlterStmtImpl(class ExtendedStmt &parent);
	~AlterStmtImpl();

	void execute(class Context &ctx);
};


class AlterStmtImplFactory : public ExtendedStmtImplFactory
{
public:
	AlterStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__extended_alter_h*/
