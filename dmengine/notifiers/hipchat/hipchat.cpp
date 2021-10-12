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
#define HIPCHAT_EXPORT __declspec(dllexport)
#else
#define HIPCHAT_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#include "hipchat.h"

#include "../../dmapi/expr.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/extended.h"
#include "../../dmapi/properties.h"


#define HIPCHAT_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////
// HipChatNotifyImpl
///////////////////////////////////////////////////////////////////////////////

HipChatNotifyImpl::HipChatNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		const char *servername, int port, bool secure, const char *url, const char *logfile, bool useHTML)
	: NotifyProviderImpl(factory, implId, notify),
	  m_servername(DUP_NULL(servername)), m_port(port), m_secure(secure), 
	  m_url(DUP_NULL(url)), m_logfile(DUP_NULL(logfile)), m_useHTML(useHTML)
{}


HipChatNotifyImpl::~HipChatNotifyImpl()
{
	SAFE_FREE(m_servername);
	SAFE_FREE(m_logfile);
	SAFE_FREE(m_url);
}

/**
 * Very simple test to make sure that the notifier is correctly configured
 * with all the mandatory properties.
 */
bool HipChatNotifyImpl::testProvider(class Context &ctx)
{
	if(!m_url) {
		ctx.writeToStdErr("\"URL\" property must be specified");
		return false;
	}

	return true;
}


void HipChatNotifyImpl::notify(
	ExtendedStmt &stmt, OutputStream &body,
	List<Attachment> *attachments, Context &ctx)
{
	ConstCharPtr subject = stmt.getArgAsString("subject", ctx);

	debug1("subject=[%s]",(const char *)subject);

	bool isTestMode = stmt.getArgAsBoolean("isTestMode", ctx);

	size_t c=strlen(body.buffer()+1);
	char *p=(char *)body.buffer();
	while (*p) {
		if (*p=='\n') c+=2;
		p++;
	}
	char *escapedText=(char *)malloc(c);
	char *q=escapedText;
	p=(char *)body.buffer();
	while (*p) {
		if (*p=='\n') {
			q[0]='\\';
			q[1]='n';
			q+=2;
		} else if (*p!='\r') {
			*q=*p;
			q++;
		}
		p++;
	}
	*q='\0';

	char *params = (char *)malloc(body.size()+512);
	sprintf(params,
		"{\"color\":\"red\","
		"\"message\":\"%s\","
		"\"notify\":true,"
		"\"message_format\":\"%s\","
		"\"card\": {\"style\": \"application\","
		"\"id\": \"db797a68-0aff-4ae8-83fc-2e72dbb1a707\","		// ID is required for grouping
		"\"title\":\"DeployHub\","
		"\"description\": {\"format\": \"%s\",\"value\": \"%s\"}, "
		"\"format\": \"medium\","
		"\"icon\": {\"url\": \"https://www.openmakesoftware.com/images/dhlogo.png\"}}}",
		escapedText,
		m_useHTML?"html":"text",
		m_useHTML?"html":"text",
		escapedText);
	char *content;
	char *contentType;
	int status;
	DoHttpRequest(m_servername,m_port,m_url,params,MESSAGE_TYPE_POST,true,m_servername,NULL,NULL,NULL,&status,&contentType,&content,m_logfile);
	if (status != 204) {
		// Something has gone wrong
	}
	free(escapedText);
	free(params);
}


Expr *HipChatNotifyImpl::getAttribute(const char *name, Context &ctx)
{
	return NotifyProviderImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// HipChatNotifyImplFactory
///////////////////////////////////////////////////////////////////////////////

HipChatNotifyImplFactory::HipChatNotifyImplFactory()
{}


NotifyProviderImpl *HipChatNotifyImplFactory::create(
	DMINT32 implId, Notify &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr fullurl  = stmt.getOverridableArgAsString("webhook", parent, true, ctx);
	ConstCharPtr logfile  = stmt.getOverridableArgAsString("logfile", parent, false, ctx);
	bool useHTML          = stmt.getOverridableArgAsBoolean("HTML",   parent, false, ctx);

	FILE *out = (FILE *)0;

	if (logfile) out = fopen(logfile,"a+");
	if (out) {
		fprintf(out,"-- HIPCHAT LOGGING ---\n");
		fprintf(out,"webhook=[%s]\n",(const char *)fullurl);
	}

	char *servername;
	int port;
	bool secure;
	char *url;
	getConnectionDetails(fullurl,&servername,&port,&secure,&url);
	if (out) {
		fprintf(out,"servername=[%s]\n",servername);
		fprintf(out,"port=%d\n",port);
		fprintf(out,"secure=%s\n",secure?"true":"false");
		fprintf(out,"url=%s\n",url);
	}

	return new HipChatNotifyImpl(*this, implId, parent,
		servername, port, secure, url, logfile, useHTML);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "HipChat_" prefix when moving to a DLL

extern "C" HIPCHAT_EXPORT int hipchat_PluginStart(DM &dm)
{
	dm.writeToLogFile("HipChat plugin V%s", HIPCHAT_PLUGIN_VERSION);
	
	NotifyProviderImplRegistry::instance().registerFactory("hipchat", new HipChatNotifyImplFactory());
	return 0;
}


extern "C" HIPCHAT_EXPORT int HipChat_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("mailserver", true, false));
	propdefs->add(new PropertyDef("from",      true, false));
	propdefs->add(new PropertyDef("username",  false, false));
	propdefs->add(new PropertyDef("password",  false, false));
	propdefs->add(new PropertyDef("mailport",  false, false));
	propdefs->add(new PropertyDef("logfile",   false, false));

	dm.installProviderImpl("hipchat", NULL, OBJ_KIND_NOTIFY, propdefs);
	return 0;
}
