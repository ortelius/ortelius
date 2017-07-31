#ifdef WIN32
#include "windows.h"
#define SLACK_EXPORT __declspec(dllexport)
#else
#define SLACK_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#include "slack.h"

#include "../../dmapi/expr.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/extended.h"
#include "../../dmapi/properties.h"



#define SLACK_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////
// SlackNotifyImpl
///////////////////////////////////////////////////////////////////////////////

SlackNotifyImpl::SlackNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		const char *servername, int port, bool secure, const char *url, const char *channel, const char *logfile)
	: NotifyProviderImpl(factory, implId, notify),
	  m_servername(DUP_NULL(servername)), m_port(port), m_secure(secure),
	  m_url(DUP_NULL(url)), m_channel(DUP_NULL(channel)), m_logfile(DUP_NULL(logfile))
{}

SlackNotifyImpl::~SlackNotifyImpl()
{
	SAFE_FREE(m_servername);
	SAFE_FREE(m_channel);
	SAFE_FREE(m_url);
	SAFE_FREE(m_logfile);
}

/*
void SlackNotifyImpl::handleRecipients(ExtendedStmt &stmt, Expr *recip, StringList &list, Context &ctx)
{
	// For HipChat, roomids are associated with user groups. If the recipient is a
	// user group, then add the roomid to the list. If the recipient is a user, then
	// get the list of user group(s) to which they belong and then add those roomids.
	// If the recipient is a string, assume that's a room id. Note if room ids are
	// not found then the default room id associated with the notifier is used.
	//
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
		if(obj->kind() == OBJ_KIND_USERGROUP) {
			// Recipient is a user group - get the room id (if any)
			UserGroup *ug = (UserGroup*) obj->toObject();
			char *roomid = ug->getRoomId();
			if (roomid && roomid[0]) list.add(roomid);
		} else {
			User *user = (User *) obj->toObject();
			List<UserGroup> *groups = user->getUserGroups();
			ListIterator<UserGroup> iter(*groups);
			for(UserGroup *ug = iter.first(); ug; ug = iter.next()) {
				debug1("Adding usergroup '%s'",ug->name());
				ExprPtr egroup = new Expr(ug);
				handleRecipients(stmt, egroup, list, ctx);
			}
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
		ConstCharPtr roomid = recip->toString();
		if(roomid && roomid[0]) list.add(roomid);
		}
		break;	
	}
}
*/


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that a
 * mailserver has been specified.
 */
bool SlackNotifyImpl::testProvider(class Context &ctx)
{
	if(!m_servername) {
		ctx.writeToStdErr("\"webhook\" property must be specified");
		return false;
	}
	if(!m_channel) {
		ctx.writeToStdErr("\"channel\" property must be specified");
		return false;
	}

	return true;
}


void SlackNotifyImpl::notify(
	ExtendedStmt &stmt, OutputStream &body,
	List<Attachment> *attachments, Context &ctx)
{
	/*
	StringList lto;

	ExprPtr eto = stmt.getArg("to", ctx);
	handleRecipients(stmt, eto, lto, ctx);

	ConstCharPtr subject = stmt.getArgAsString("subject", ctx);

	debug1("subject=[%s]",(const char *)subject);
	*/
	bool isTestMode = stmt.getArgAsBoolean("isTestMode", ctx);

	/*
	if(lto.size() > 0) {
		StringListIterator iter(lto);
		for(const char *roomid = iter.first(); roomid; roomid = iter.next()) {
			ctx.dm().writeToLogFile("Sending message to roomid [%s]", roomid);
			char *url = (char *)malloc(strlen(roomid)+strlen(m_authtoken)+128);
			sprintf(url,"v2/room/%s/notification?auth_token=%s",roomid,m_authtoken);
			char *params = (char *)malloc(body.size()+256);
			sprintf(params,"{\"color\":\"green\",\"message\":\"%s\",\"notify\":false,\"message_format\":\"%s\"}",body,m_useHTML?"html":"text");
			char *content;
			char *contentType;
			int status;
			int m_port = 443;	// TO DO - get this from properties.
			DoHttpRequest(m_servername,m_port,url,params,MESSAGE_TYPE_POST,true,m_servername,NULL,NULL,NULL,&status,&contentType,&content);
		}
	} else {
		// No "to" specified - use default room id
		
	}
	*/
	char *params = (char *)malloc(body.size()+512);
	// Slack requires only 3 HTML entities to be escaped - &, < and > (see https://api.slack.com/docs/message-formatting)
	size_t c=strlen(body.buffer()+1);
	char *p=(char *)body.buffer();
	while (*p) {
		if (*p=='&') c+=4;
		if (*p=='<' || *p=='>') c+=3;
		if (*p=='\n') c+=2;
		p++;
	}
	char *escapedText=(char *)malloc(c);
	char *q=escapedText;
	p=(char *)body.buffer();
	while (*p) {
		if (*p=='&') {
			q[0]='&';
			q[1]='a';
			q[2]='m';
			q[3]='p';
			q[4]=';';
			q+=5;
		} else if (*p=='<') {
			q[0]='&';
			q[1]='l';
			q[2]='t';
			q[3]=';';
			q+=4;
		} else if (*p=='>') {
			q[0]='&';
			q[1]='g';
			q[2]='t';
			q[3]=';';
			q+=4;
		} else if (*p=='\n') {
			q[0]='\\';
			q[1]='n';
			q+=2;
		} else if (*p!='\r') {
			*q = *p;
			q++;
		}
		p++;
	}
	*q='\0';

	sprintf(params,
		"{\"channel\": \"%s\" ,"
		"\"username\": \"DeployHub\", "
		"\"text\": \"%s\", "
		"\"icon_url\": \"https://www.openmakesoftware.com/images/dhlogo.png\" }",
		m_channel,
		escapedText);

	free(escapedText);

	char *content;
	char *contentType;
	int status;
	DoHttpRequest(m_servername,m_port,m_url,params,MESSAGE_TYPE_POST,true,m_servername,NULL,NULL,NULL,&status,&contentType,&content,m_logfile);
}


Expr *SlackNotifyImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "channel") == 0) {
		return new Expr(m_channel);
	}

	return NotifyProviderImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// SlackNotifyImplFactory
///////////////////////////////////////////////////////////////////////////////

SlackNotifyImplFactory::SlackNotifyImplFactory()
{}


NotifyProviderImpl *SlackNotifyImplFactory::create(
	DMINT32 implId, Notify &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr webhook  = stmt.getOverridableArgAsString("webhook", parent, true, ctx);
	ConstCharPtr channel  = stmt.getOverridableArgAsString("channel", parent, true, ctx);
	ConstCharPtr logfile  = stmt.getOverridableArgAsString("logfile", parent, false, ctx);

	FILE *out = (FILE *)0;

	if (logfile) out = fopen(logfile,"a+");
	if (out) {
		fprintf(out,"-- SLACK LOGGING ---\n");
		fprintf(out,"webhook=[%s]\n",(const char *)webhook);
	}
	char *servername;
	int port;
	bool secure;
	char *url;
	getConnectionDetails(webhook,&servername,&port,&secure,&url);
	if (out) {
		fprintf(out,"servername=[%s]\n",servername);
		fprintf(out,"port=%d\n",port);
		fprintf(out,"secure=%s\n",secure?"true":"false");
		fprintf(out,"url=%s\n",url);
		fclose(out);
	}

	return new SlackNotifyImpl(*this, implId, parent,
		servername, port, secure, url, channel, logfile);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "Slack_" prefix when moving to a DLL

extern "C" SLACK_EXPORT int slack_PluginStart(DM &dm)
{
	dm.writeToLogFile("Slack plugin V" SLACK_PLUGIN_VERSION);
	
	NotifyProviderImplRegistry::instance().registerFactory("slack", new SlackNotifyImplFactory());
	return 0;
}


extern "C" SLACK_EXPORT int Slack_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("mailserver", true, false));
	propdefs->add(new PropertyDef("from",      true, false));
	propdefs->add(new PropertyDef("username",  false, false));
	propdefs->add(new PropertyDef("password",  false, false));
	propdefs->add(new PropertyDef("mailport",  false, false));
	propdefs->add(new PropertyDef("logfile",   false, false));

	dm.installProviderImpl("slack", NULL, OBJ_KIND_NOTIFY, propdefs);
	return 0;
}
