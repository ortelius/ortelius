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
#define TEXTMODIFY_EXPORT __declspec(dllexport)
#else
#define TEXTMODIFY_EXPORT
#endif /*WIN32*/

#include <stdio.h>
#include <sys/stat.h>

#include "text.h"

#include "../../dmapi/context.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/node.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/rewrap.h"


#define TEXTMODIFY_PLUGIN_VERSION "1.00"
#define TEXTMODIFY_MODIFY_NAME    "text"
#define TEXTMODIFY_REPLACE_NAME   "text_replace"


///////////////////////////////////////////////////////////////////////////////
// TextModifyProviderImpl
///////////////////////////////////////////////////////////////////////////////

TextModifyProviderImpl::TextModifyProviderImpl(ExtendedStmt &parent)
	: ModifyProviderImpl(parent), m_data(NULL)
{}


TextModifyProviderImpl::~TextModifyProviderImpl()
{
	SAFE_FREE(m_data);
}


void TextModifyProviderImpl::loadFile(const char *infile, class Context &ctx)
{
	struct stat buf;
	if(stat(infile, &buf) == -1) {
		throw RuntimeError(ctx.stack(), "Failed to find text file \"%s\"", infile);
	}

	m_data = (char*) malloc(buf.st_size + 1);
	if (m_data) {

#ifdef WIN32
		FILE *in = fopen(infile, "rb");
#else
		FILE *in = fopen(infile, "r");
#endif /*WIN32*/
		if(!in) {
			throw RuntimeError(ctx.stack(), "Failed to load text file \"%s\"", infile);
		}
		size_t count = fread(m_data, 1, buf.st_size, in);
		fclose(in);

		m_size = count;
		m_data[count] = '\0';
	}
}


void TextModifyProviderImpl::saveFile(const char *outfile, class Context &ctx)
{
	if(m_data) {
		//if(debuglevel() >= 1) {
		//	debug1("Contents to be written to '%s' are:", outfile);
		//	m_doc->Print();
		//}

#ifdef WIN32
		FILE *out = fopen(outfile, "wb");
#else
		FILE *out = fopen(outfile, "w");
#endif /*WIN32*/
		if(!out) {
			throw RuntimeError(ctx.stack(), "Failed to save text file \"%s\"", outfile);
		}
		/*size_t count =*/ fwrite(m_data, 1, m_size, out);
		fclose(out);
	}
}


void TextModifyProviderImpl::replace(ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr find = stmt.getArgAsString("find", ctx);
	if(!find) {
		throw SyntaxError(stmt, ctx.stack(), "replace requires a find");
	}

	ConstCharPtr replace = stmt.getArgAsString("replace", ctx);
	if(!replace) {
		throw SyntaxError(stmt, ctx.stack(), "replace requires a replace");
	}

	bool lineProcessing = stmt.getArgAsBoolean("line", ctx);

	if(!m_data) {
		throw RuntimeError(stmt, ctx.stack(), "Data not set - help!");
	}

	debug3("find=[%s] replace=[%s] lineprocessing=[%s]",(const char *)find,(const char *)replace,lineProcessing?"true":"false");
	bool noIterate=(find[0]=='^');
	RegExpWrapper re(find);
	if (lineProcessing) {
		// Process a line at a time
		char *res = (char *)0;
		char *t = strdup(m_data);
		char *p = t;
		char *ls = p;	// line start
		
		while (*ls) {
			while (*p) {
				if (*p=='\r' || *p=='\n') break;
				p++;
			}
			// EOL or EOF
			char lec = *p;							// save line ending character
			*p='\0';								// terminate line
			char *nl = re.replaceAll(ls, replace, noIterate);	// process line
			if (!res) {
				res = (char *)malloc(strlen(nl)+2);
				strcpy(res,nl);	
			} else {
				res = (char *)realloc(res,strlen(res)+strlen(nl)+2);
				strcat(res,nl);
			}
			int w=strlen(res);
			res[w]=lec;	// original line ending character
			res[w+1]='\0';
			if (lec=='\r') {
				// Could be a CR-LF pair
				if (*(p+1)=='\n') {
					// Yes it was - add the \n before moving on to next line
					p++;
					res = (char *)realloc(res,w+2);
					res[w]='\n';
					res[w+1]='\0';
				}
			}
			p++;
			ls = p;
		}
		m_data = res;
		SAFE_FREE(t);
	} else {
		char *temp = re.replaceAll(m_data, replace, noIterate);
		SAFE_FREE(m_data);
		m_data = temp;
		debug3("temp=[%s]",temp);
	}
	m_size = strlen(m_data);
}


void TextModifyProviderImpl::executeSubStmt(ExtendedStmt &stmt, Context &ctx)
{
	if(strcmp(stmt.ident(), TEXTMODIFY_REPLACE_NAME) == 0) {
		replace(stmt, ctx);
	} else {
		throw RuntimeError(stmt, ctx.stack(),
			"\"%s\" is not valid with a modify of type \""
			TEXTMODIFY_MODIFY_NAME "\"", stmt.ident());
	}
}


///////////////////////////////////////////////////////////////////////////////
// TextModifyProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

TextModifyProviderImplFactory::TextModifyProviderImplFactory()
{}


TextModifyProviderImplFactory::~TextModifyProviderImplFactory()
{}


ModifyProviderImpl *TextModifyProviderImplFactory::create(ExtendedStmt &parent)
{
	return new TextModifyProviderImpl(parent);
}


/////////////////////////////////////////////////////////////////////////////////
//// TextModify_ReplaceStmtImpl
/////////////////////////////////////////////////////////////////////////////////
//
//TextModify_ReplaceStmtImpl::TextModify_ReplaceStmtImpl(ExtendedStmt &parent)
//	: ModifyProviderStmtImpl(parent)
//{}
//
//
//TextModify_ReplaceStmtImpl::~TextModify_ReplaceStmtImpl()
//{}
//
//
//void TextModify_ReplaceStmtImpl::modify(ModifyStmtImpl &modifier, class Context &ctx)
//{
//	modifier.executeSubStmt(m_parent, ctx);
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// TextModify_ReplaceStmtImplFactory
/////////////////////////////////////////////////////////////////////////////////
//
//TextModify_ReplaceStmtImplFactory::TextModify_ReplaceStmtImplFactory()
//{}
//
//
//TextModify_ReplaceStmtImplFactory::~TextModify_ReplaceStmtImplFactory()
//{}
//
//
//ExtendedStmtImpl *TextModify_ReplaceStmtImplFactory::create(class DM &dm, ExtendedStmt &parent)
//{
//	return new TextModify_ReplaceStmtImpl(parent);
//}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "textmodify_" prefix when moving to a DLL

extern "C" TEXTMODIFY_EXPORT int textmodify_PluginStart(DM &dm)
{
	dm.writeToLogFile("Text Modify plugin V" TEXTMODIFY_PLUGIN_VERSION);
	
	ModifyProviderImplRegistry::instance().registerFactory(
		TEXTMODIFY_MODIFY_NAME, new TextModifyProviderImplFactory());
	ExtendedStmtImplRegistry::instance().registerFactory(
		TEXTMODIFY_REPLACE_NAME, new ModifyProviderStmtImplFactory());
	return 0;
}


extern "C" TEXTMODIFY_EXPORT int textmodify_PluginInstall(DM &dm)
{
	return 0;
}
