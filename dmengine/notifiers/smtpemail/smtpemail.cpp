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
#ifdef WIN32
#include "windows.h"
#define SMTPEMAIL_EXPORT __declspec(dllexport)
#else
#define SMTPEMAIL_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#include "smtpemail.h"

#include "../../dmapi/expr.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/extended.h"
#include "../../dmapi/properties.h"

#include "smtp.h"


#define SMTPEMAIL_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////
// SmtpEmailNotifyImpl
///////////////////////////////////////////////////////////////////////////////

SmtpEmailNotifyImpl::SmtpEmailNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		const char *mailserver, Expr *from, const char *username,
		const char *password, int port, const char *logfile, bool useSSL, bool useHTML)
	: NotifyProviderImpl(factory, implId, notify),
	  m_mailserver(DUP_NULL(mailserver)), m_from(from),
	  m_username(DUP_NULL(username)), m_password(DUP_NULL(password)),
	  m_mailport(port), m_logfile(DUP_NULL(logfile)), m_useSSL(useSSL), m_useHTML(useHTML)
{
	debug1("SmtpEmailNotifyImpl('%s', '%s', '********', %d)",
		NULL_CHECK(m_mailserver), NULL_CHECK(m_username), m_mailport);
}


SmtpEmailNotifyImpl::~SmtpEmailNotifyImpl()
{
	SAFE_FREE(m_mailserver);
	SAFE_DELETE(m_from);
	SAFE_FREE(m_username);
	SAFE_FREE(m_password);
	SAFE_FREE(m_logfile);
}


char *getOwnerEmail(IObject *obj, Context &ctx)
{
	ExprPtr eowner = obj->getAttribute("owner", ctx);
	ObjectReference *oowner = eowner ? eowner->toObjectReference() : NULL;
	IObject *owner = oowner ? oowner->toIObject() : NULL;
	ExprPtr eowneremail = owner ? owner->getAttribute("email", ctx) : NULL;
	return eowneremail ? eowneremail->toString() : NULL;
}

void handleRecipients(ExtendedStmt &stmt, Expr *recip, StringList &list, Context &ctx)
{
	if(!recip) {
		return;
	}
	// recip->print();
	
	switch(recip->kind()) {
	case KIND_OBJECT: {
		ObjectReference *obj = recip->toObjectReference();
		if((obj->kind() != OBJ_KIND_USER) && (obj->kind() != OBJ_KIND_USERGROUP)) {
			throw RuntimeError(stmt, ctx.stack(),
					"Object must be of type 'user' or 'usergroup' for notifications");
		}
		ExprPtr eemail = obj->object_access("email", ctx);
		// temp
		if(obj->kind() == OBJ_KIND_USERGROUP) {
			UserGroup *ug = (UserGroup*) obj->toObject();
			debug1("User group '%s'",ug->name());
		} else {
			User *u = (User*) obj->toObject();
			debug1("User '%s'",u->name());
		}
		// end temp
		if((obj->kind() == OBJ_KIND_USERGROUP) && !eemail) {
			// No email on usergroup object - add all the members
			UserGroup *ug = (UserGroup*) obj->toObject();
			debug1("No email address on group '%s' - adding members", ug->name());
			List<User> *users = ug->getUsers();
			ListIterator<User> iter(*users);
			for(User *user = iter.first(); user; user = iter.next()) {
				debug1("Adding user '%s'", user->name());
				ExprPtr euser = new Expr(user);
				handleRecipients(stmt, euser, list, ctx);
			}
		} else {
			ConstCharPtr email = eemail ? eemail->toString() : NULL;
			debug1("User or group with email '%s'", NULL_CHECK((const char*) email));
			if(email && email[0]) { list.add(email); }
		}
		}
		break;
	case KIND_ARRAY: {
		DMArray *arr = recip->toArray();
		if(arr) {
			AutoPtr<StringList> keys = arr->keys();
			StringListIterator iter(*keys);
			for(const char *k = iter.first(); k; k = iter.next()) {
				Variable *v = arr->get(k);
				if(v) {
					ExprPtr ev = new Expr(v);
					handleRecipients(stmt, ev, list, ctx);
				}
			}
		}
		}
		break;
	default: {
		ConstCharPtr email = recip->toString();
		if(email) { list.add(email); }
		}
		break;	
	}
}



/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that a
 * mailserver has been specified.
 */
bool SmtpEmailNotifyImpl::testProvider(class Context &ctx)
{
	if(!m_mailserver) {
		ctx.writeToStdErr("\"mailserver\" property must be specified");
		return false;
	}
	if(!m_from) {
		ctx.writeToStdErr("\"from\" property must be specified");
		return false;
	}

	return true;
}


void SmtpEmailNotifyImpl::notify(
	ExtendedStmt &stmt, OutputStream &body,
	List<Attachment> *attachments, Context &ctx)
{
	StringList lto;
	StringList lcc;
	
	ExprPtr eto = stmt.getArg("to", ctx);
	handleRecipients(stmt, eto, lto, ctx);

	ExprPtr ecc = stmt.getArg("cc", ctx);
	handleRecipients(stmt, ecc, lcc, ctx);

	ConstCharPtr subject = stmt.getArgAsString("subject", ctx);

	debug1("subject=[%s]",(const char *)subject);

	ConstCharPtr from;
	if(m_from) {
		if(m_from->kind() == KIND_OBJECT) {
			ObjectReference *obj = m_from->toObjectReference();
			if(obj->kind() != OBJ_KIND_USER) {
				throw RuntimeError(stmt, ctx.stack(),
					"Object must be of type 'user' for notifications");
			}
			ExprPtr efrom = obj->object_access("email", ctx);
			from = efrom ? efrom->toString() : NULL;
		} else {
			from = m_from->toString();
		}
	}

	ExprPtr eattach = stmt.getArg("attachment", ctx);
	OutputStream *attach = eattach ? eattach->toStream() : NULL;

	if(!from || !subject) {
		throw RuntimeError(stmt, ctx.stack(),
			"Notify failed - missing from or subject");
	}

	bool isTestMode = stmt.getArgAsBoolean("isTestMode", ctx);

	SMTP smtp(m_mailserver, m_mailport, m_username, m_password, from, subject,
		body.buffer(), m_logfile, m_useSSL, m_useHTML, (isTestMode ? ctx.dm().toOutputStream() : NULL));
	//if(email) {
	//	ctx.dm().writeToLogFile("Adding recipient [<%s>]", (const char*) email);
	//	smtp.addRecipient(email);
	if(lto.size() > 0) {
		StringListIterator iter(lto);
		for(const char *email = iter.first(); email; email = iter.next()) {
			ctx.dm().writeToLogFile("Adding recipient [<%s>]", email);
			smtp.addRecipient(email);
		}
	} else {
		// No "to" specified, so first try and find the server owner(s), then the environment owner
		if(ctx.servers().size() != 0) {
			ListIterator<Server> iter(ctx.servers());
			for(Server *ps = iter.first(); ps; ps = iter.next()) {
				ConstCharPtr owneremail = getOwnerEmail(ps, ctx);
				debug1("server \"%s\" owner \"%s\"", ps->name(), (owneremail ? (const char*) owneremail : "(no email address)"));
				if(owneremail) {
					smtp.addRecipient(owneremail);
				}
			}
		}
		Environment *env = ctx.dm().getTargetEnvironment();
		if(env) {
			ConstCharPtr owneremail = getOwnerEmail(env, ctx);
			debug1("environment \"%s\" owner \"%s\"", env->name(), (owneremail ? (const char*) owneremail : "(no email address)"));
			if(owneremail) {
				smtp.addRecipient(owneremail);
			}
		}
	}

	//if(email_cc) {
	//	ctx.dm().writeToLogFile("Adding cc recipient [<%s>]", (const char*) email_cc);
	//	smtp.addCarbonCopy(email_cc);
	//}
	if(lcc.size() > 0) {
		StringListIterator iter(lto);
		for(const char *email = iter.first(); email; email = iter.next()) {
			ctx.dm().writeToLogFile("Adding cc recipient [<%s>]", email);
			smtp.addCarbonCopy(email);
		}
	}

	if(attach) {
		smtp.addAttachment("attachment.txt", (unsigned char*) attach->buffer(), attach->size());
	}
	if(attachments) {
		ListIterator<Attachment> iter(*attachments);
		for(Attachment *att = iter.first(); att; att = iter.next()) {
			smtp.addAttachment(att->name(), att->data(), att->datasize());
		}
	}

	if(smtp.isOkay()) {
		ctx.dm().writeToLogFile("Sending email notification");
		if(getenv("trinonotify")) {
			smtp.dump();
		} else {
			smtp.sendMessage();
		}
	} else {
		ctx.writeToStdOut("Message not okay - no recipients?\n");
	}
}


Expr *SmtpEmailNotifyImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "mailserver") == 0) {
		return new Expr(m_mailserver);
	} else if(strcmp(name, "mailport") == 0) {
		return new Expr(m_mailport);
	}

	return NotifyProviderImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// SmtpEmailNotifyImplFactory
///////////////////////////////////////////////////////////////////////////////

SmtpEmailNotifyImplFactory::SmtpEmailNotifyImplFactory()
{}


NotifyProviderImpl *SmtpEmailNotifyImplFactory::create(
	DMINT32 implId, Notify &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr mailserver  = stmt.getOverridableArgAsString("mailserver", parent, true, ctx);
	Expr* from               = stmt.getOverridableArg("from", parent, true, ctx);
	ConstCharPtr username    = stmt.getOverridableArgAsString("username",   parent, false, ctx);
	ConstCharPtr password    = stmt.getOverridableArgAsString("password",   parent, false, ctx);
	ConstCharPtr smailport   = stmt.getOverridableArgAsString("mailport",   parent, false, ctx);
	int mailport = smailport ? atoi(smailport) : 0;
	ConstCharPtr logfile     = stmt.getOverridableArgAsString("logfile",    parent, false, ctx);
	bool useSSL              = stmt.getOverridableArgAsBoolean("SSL",		parent, false, ctx);
	bool useHTML             = stmt.getOverridableArgAsBoolean("HTML",		parent, false, ctx);

	printf("useHTML=%s\n",useHTML?"true":"false");

	// Default to port 25 (SMTP) is none specified
	if(!mailport) {
		mailport = 25;
	}
	Credentials *cred = parent.getCredentials();
	if (cred) {
		// Credentials object specified - use the username/password in here in preference.
		username = cred->getDecryptedUsername(ctx);
		password = cred->getDecryptedPassword(ctx);
	}

	return new SmtpEmailNotifyImpl(*this, implId, parent,
		mailserver, from, username, password, mailport, logfile, useSSL, useHTML);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "smtpemail_" prefix when moving to a DLL

extern "C" SMTPEMAIL_EXPORT int smtpemail_PluginStart(DM &dm)
{
	dm.writeToLogFile("SMTP Email plugin V" SMTPEMAIL_PLUGIN_VERSION);
	
	NotifyProviderImplRegistry::instance().registerFactory("smtpemail", new SmtpEmailNotifyImplFactory());
	return 0;
}


extern "C" SMTPEMAIL_EXPORT int smtpemail_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("mailserver", true, false));
	propdefs->add(new PropertyDef("from",      true, false));
	propdefs->add(new PropertyDef("username",  false, false));
	propdefs->add(new PropertyDef("password",  false, false));
	propdefs->add(new PropertyDef("mailport",  false, false));
	propdefs->add(new PropertyDef("logfile",   false, false));

	dm.installProviderImpl("smtpemail", NULL, OBJ_KIND_NOTIFY, propdefs);
	return 0;
}
