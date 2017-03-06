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
#ifndef __extended_notifystmt_h

#define __extended_notifystmt_h


#include "../../dmapi/extended.h"


///////////////////////////////////////
// Notify
///////////////////////////////////////

class NotifyStmtImpl : public /*virtual*/ AuditableExtendedStmtImpl, public IObject
{
private:
	class ExtendedStmt &m_parent;
	List<class Attachment> *m_attachments;

public:
	NotifyStmtImpl(class DM &dm, class ExtendedStmt &parent);
	~NotifyStmtImpl();

	void executeWithAudit(class Context &ctx);

	void addAttachment(class Attachment *attach);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//Object *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class NotifyStmtImplFactory : public /*virtual*/ ExtendedStmtImplFactory
{
public:
	NotifyStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


///////////////////////////////////////
// Attachment
///////////////////////////////////////

class DMAPI_API AttachmentStmtImpl : public /*virtual*/ ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	AttachmentStmtImpl(ExtendedStmt &parent);
	~AttachmentStmtImpl();

	void execute(class Context &ctx);
};


class AttachmentStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	AttachmentStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__extended_notifystmt_h*/
