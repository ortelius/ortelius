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

#ifdef WIN32
#include "windows.h"
#define TXTLOCAL_EXPORT __declspec(dllexport)
#else
#define TXTLOCAL_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#include "txtlocal.h"

#include "../../dmapi/expr.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/extended.h"
#include "../../dmapi/properties.h"

#include "https.h"


#define TXTLOCAL_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////


char *urlencode(const char *str)
{
	int size = 2 * strlen(str);
	char *ret = (char*) malloc(size);
	if (ret) {
		int n = 0;

		for(const char *x = str; x && *x; x++) {
			if((n + 4) >= size) {
				size += strlen(str);
				char *ret2 = (char*) realloc(ret, size);
				if (ret2) {
					ret = ret2;
				}
			}
			switch(*x) {
			case '\t':
			case '\r':
			case '\n':
			case ' ':
			case '&':
				ret[n++] = '%';
				sprintf(&ret[n], "%02X", (int) *x);
				n+=2;
				break;
			default:
				ret[n++] = *x;
				break;
			}
		}
		ret[n] = '\0';
	}
	return ret;
}


///////////////////////////////////////////////////////////////////////////////
// TxtLocalNotifyImpl
///////////////////////////////////////////////////////////////////////////////

TxtLocalNotifyImpl::TxtLocalNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		Expr *from, const char *username, const char *password)
	: NotifyProviderImpl(factory, implId, notify), m_from(from),
	  m_username(DUP_NULL(username)), m_password(DUP_NULL(password))
{
	debug1("TxtLocalNotifyImpl('%s', '********')", NULL_CHECK(m_username));
}


TxtLocalNotifyImpl::~TxtLocalNotifyImpl()
{
	SAFE_DELETE(m_from);
	SAFE_FREE(m_username);
	SAFE_FREE(m_password);
}


char *getOwnerPhone(IObject *obj, Context &ctx)
{
	ExprPtr eowner = obj->getAttribute("owner", ctx);
	ObjectReference *oowner = eowner ? eowner->toObjectReference() : NULL;
	IObject *owner = oowner ? oowner->toIObject() : NULL;
	ExprPtr eownerphone = owner ? owner->getAttribute("phone", ctx) : NULL;
	return eownerphone ? eownerphone->toString() : NULL;
}


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.
 * TODO: Make this try to connect to the service ensuring firewall ports open
 */
bool TxtLocalNotifyImpl::testProvider(class Context &ctx)
{
	if(!m_from) {
		ctx.writeToStdErr("\"from\" property must be specified");
		return false;
	}
	return true;
}


void TxtLocalNotifyImpl::notify(
	ExtendedStmt &stmt, OutputStream &body,
	List<Attachment> *attachments, Context &ctx)
{
	if(attachments) {
		throw RuntimeError(stmt, ctx.stack(),
			"txtlocal notify does not support attachments");
	}

	ExprPtr eto = stmt.getArg("to", ctx);
	if(eto && (eto->kind() == KIND_OBJECT)) {
		ObjectReference *obj = eto->toObjectReference();
		if((obj->kind() != OBJ_KIND_USER) && (obj->kind() != OBJ_KIND_USERGROUP)) {
			throw RuntimeError(stmt, ctx.stack(),
				"Object must be of type 'user' or 'usergroup' for notifications");
		}
		eto = obj->object_access("phone", ctx);
	}
	ConstCharPtr phone = eto ? eto->toString() : NULL;

	//ExprPtr esubject = stmt.getArg("subject", ctx);
	//CharPtr subject = esubject ? esubject->toString() : NULL;

	ConstCharPtr from;
	if(m_from) {
		if(m_from->kind() == KIND_OBJECT) {
			ObjectReference *obj = m_from->toObjectReference();
			if(obj->kind() != OBJ_KIND_USER) {
				throw RuntimeError(stmt, ctx.stack(),
					"Object must be of type 'user' for notifications");
			}
			ExprPtr efrom = obj->object_access("phone", ctx);
			from = efrom ? efrom->toString() : NULL;
		} else {
			from = m_from->toString();
		}
	}

	ConstCharPtr logfile = stmt.getArgAsString("logfile", ctx);

	if(!from /*|| !subject*/) {
		throw RuntimeError(stmt, ctx.stack(),
				"Notify failed - missing from");
	}

	bool info = true;
	bool test = getenv("trinonotify") ? true : false;		// ##### set to true whilst developing to prevent using credits

	char *selectednums = NULL;

	if(phone) {
		/*ctx.dm().writeToLogFile*/ debug1("Adding recipient [%s]", (const char*) phone);
		selectednums = strdup(phone);
	} else {
		// No "to" specified, so first try and find the server owner(s), then the environment owner
		if(ctx.servers().size() != 0) {
			ListIterator<Server> iter(ctx.servers());
			for(Server *ps = iter.first(); ps; ps = iter.next()) {
				//IObject *owner = ps->getObjectAttribute("owner", ctx);
				//const char *ownerphone = owner ? owner->getAttribute("phone", ctx) : NULL;
				ConstCharPtr ownerphone = getOwnerPhone(ps, ctx);
				debug1("server \"%s\" owner \"%s\"", ps->name(), (ownerphone ? (const char*) ownerphone : "(no phone number)"));
				if(ownerphone) {
					if(selectednums) {
						if(!strstr(selectednums, ownerphone)) {
							selectednums = (char*) realloc(selectednums, strlen(selectednums) + strlen(ownerphone) + 2);
							strcat(selectednums, ",");
							strcat(selectednums, ownerphone);
						} else {
							debug1("skipping duplicate phone number %s", (const char*) ownerphone);
						}
					} else {
						selectednums = strdup(ownerphone);
					}
				}
			}
		}
		Environment *env = ctx.dm().getTargetEnvironment();
		if(env) {
			//IObject *owner = env->getObjectAttribute("owner", ctx);
			//const char *ownerphone = owner ? owner->getAttribute("phone", ctx) : NULL;
			ConstCharPtr ownerphone = getOwnerPhone(env, ctx);
			debug1("environment \"%s\" owner \"%s\"", env->name(), (ownerphone ? (const char*) ownerphone : "(no phone number)"));
			if(ownerphone) {
				if(selectednums) {
					if(!strstr(selectednums, ownerphone)) {
						selectednums = (char*) realloc(selectednums, strlen(selectednums) + strlen(ownerphone) + 2);
						strcat(selectednums, ",");
						strcat(selectednums, ownerphone);
					} else {
						debug1("skipping duplicate phone number %s", (const char*) ownerphone);
					}
				} else {
					selectednums = strdup(ownerphone);
				}
			}
		}
	}

	if(!selectednums) {
		ctx.writeToStdOut("Message not okay - no recipients?\n");
		return;
	}

	char *encbody = urlencode(body.buffer());

	char *content = (char*) malloc(strlen(encbody) + 1024);
	sprintf(content, "uname=%s&pword=%s&message=%s&from=%s&selectednums=%s&info=%d&test=%d",
		m_username, m_password, encbody, (const char*) from,
		selectednums, (info ? 1 : 0), (test ? 1 : 0));

	SAFE_FREE(encbody);
	SAFE_FREE(selectednums);

	HTTPS https("www.txtlocal.com", 443, "/sendsmspost.php", content, logfile);

	if(https.isOkay()) {
		ctx.dm().writeToLogFile("Sending txtlocal notification");
		if(getenv("trinonotify")) {
			https.dump();
		}
		if(!https.post()) {
			printf("Failed to send\n");
		}
	} else {
		ctx.writeToStdOut("Message not okay - no recipients?\n");
	}

	SAFE_FREE(content);
}


Expr *TxtLocalNotifyImpl::getAttribute(const char *name, Context &ctx)
{
	return NotifyProviderImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// TxtLocalNotifyImplFactory
///////////////////////////////////////////////////////////////////////////////

TxtLocalNotifyImplFactory::TxtLocalNotifyImplFactory()
{}


NotifyProviderImpl *TxtLocalNotifyImplFactory::create(
	DMINT32 implId, Notify &parent, ExtendedStmt &stmt, Context &ctx)
{
	Expr *from = stmt.getOverridableArg("from", parent, true, ctx);
	ConstCharPtr username = stmt.getOverridableArgAsString("username", parent, true, ctx);
	ConstCharPtr password = stmt.getOverridableArgAsString("password", parent, true, ctx);

	return new TxtLocalNotifyImpl(*this, implId, parent, from, username, password);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "txtlocal_" prefix when moving to a DLL

extern "C" TXTLOCAL_EXPORT int txtlocal_PluginStart(DM &dm)
{
	dm.writeToLogFile("TxtLocal plugin V" TXTLOCAL_PLUGIN_VERSION);

	NotifyProviderImplRegistry::instance().registerFactory("txtlocal", new TxtLocalNotifyImplFactory());
	return 0;
}


extern "C" TXTLOCAL_EXPORT int txtlocal_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("username", false, false));
	propdefs->add(new PropertyDef("password", false, false));

	dm.installProviderImpl("txtlocal", NULL, OBJ_KIND_NOTIFY, propdefs);
	return 0;
}
