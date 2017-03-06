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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif
#include <sys/stat.h>

#include "dm.h"
#include "expr.h"
#include "charptr.h"
#include "context.h"
#include "scopestack.h"
#include "model.h"
#include "dropzone.h"
#include "platform.h"
#include "exceptions.h"
#include "regexp.h"
#include "rewrap.h"
#include "autoptr.h"
#include "pathname.h"
#include "lexer.h"	// for sum()
#include "node.h"	// for sum()
#include "datetime.h"

#include "../plugins/restful/include/xml2json.hpp"

extern char *base64encode(unsigned char *data, unsigned long datalen);
extern unsigned char *decryptbase64(char *md,int *retlen);

typedef struct tagDMExprFnTable {
	const char *name;
	int min_argc;
	int max_argc;
	DMExprFnPtr fn;
} DMExprFnTable;

typedef struct tagDMObjFnTable {
	const char *name;
	int ekind;
	OBJECT_KIND okind;
	int min_argc;
	int max_argc;
	DMObjFnPtr fn;
} DMObjFnTable;


///////////////////////////////////////////////////////////////////////////////
// Object Functions
///////////////////////////////////////////////////////////////////////////////

Expr *function_basename(int argc, Expr **argv, Context &ctx);
Expr *function_dirname(int argc, Expr **argv, Context &ctx);

Expr *function_obj_append(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Server *srv = (Server*) self->toObjectReference()->toObject();
	ConstCharPtr pathname = argv[0]->toString();
	ConstCharPtr pathname2 = argv[1]->toString();
	AutoPtr<PathNameImpl> path1 = srv->createPath(pathname);
	AutoPtr<PathNameImpl> path2 = path1->append(pathname2);
	return new Expr(path2->path());
}

Expr *function_obj_server_deptime(int argc, Expr *self, Expr **argv, Context &ctx)
{
	// Takes a Component Object as a parameter, returns a DateTime object representing
	// last deployment of this Component to this Server
	Server *srv = (Server*) self->toObjectReference()->toObject();
	Component *comp = (Component*) argv[0]->toObjectReference()->toObject();
	DateTime *res = ctx.dm().getDeployTime(comp,srv);
	return new Expr(res);
}

Expr *function_obj_environment_deptime(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Environment *env = (Environment*) self->toObjectReference()->toObject();
	Application *app = (Application*) argv[0]->toObjectReference()->toObject();
	DateTime *res = ctx.dm().getDeployTime(app,env);
	return new Expr(res);
}



Expr *function_obj_appendstd(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Server *srv = (Server*) self->toObjectReference()->toObject();
	ConstCharPtr pathname = argv[0]->toString();
	ConstCharPtr pathname2 = argv[1]->toString();
	AutoPtr<PathNameImpl> path1 = srv->createPath(pathname);
	AutoPtr<PathNameImpl> path2 = path1->appendstd(pathname2);
	return new Expr(path2->path());
}

Expr *function_obj_stdpath(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Server *srv = (Server*) self->toObjectReference()->toObject();
	ConstCharPtr pathname = argv[0]->toString();
	AutoPtr<PathNameImpl> path = srv->createPath(pathname);
	return new Expr(path->stdpath());
}

Expr *function_obj_basename(int argc, Expr *self, Expr **argv, Context &ctx)
{
	//Expr *args[] = {
	//	argv[0],
	//	self->object_access("type", ctx),
	//	NULL
	//};
	//return function_basename(2, args, ctx);

	Server *srv = (Server*) self->toObjectReference()->toObject();
	ConstCharPtr pathname = argv[0]->toString();
	AutoPtr<PathNameImpl> path = srv->createPath(pathname);
	if(path) {
		AutoPtr<PathNameImpl> basename = path->basename();
		return new Expr(basename->path());
	}
	throw RuntimeError("Unable to create path '%s' for server '%s'", (const char*) pathname, srv->name());
}

Expr *function_obj_applications(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Domain *dom = (Domain*) self->toObjectReference()->toObject();
	List<Application> *apps = dom->getApplications();
	DMArray *res = new DMArray(true,false,false);

	ListIterator<Application> iter(*apps);
	for(Application *app = iter.first(); app; app = iter.next()) {
		res->add(new Variable(NULL,app));
	}
	return new Expr(res);
}

Expr *function_obj_children(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Application *app = (Application*) self->toObjectReference()->toObject();
	// Gets the children (descendents) of this application version.
	// Options are:
	// true: All children, grandchildren, great-grandchildren etc.
	// false (default): Just immediate descendents
	//
	bool option = (argc>0)?argv[0]->toBool():false;		
	List<Application> *apps = app->getChildren(option);

	DMArray *res = new DMArray(true,false,false);

	ListIterator<Application> iter(*apps);
	for(Application *a = iter.first(); a; a = iter.next()) {
		res->add(new Variable(NULL,a));
	}
	return new Expr(res);
}

Expr *function_obj_components(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Application *app = (Application*) self->toObjectReference()->toObject();
	//
	// Gets the components of this application version.
	//	
	List<Component> *comps =  app->getComponents();

	// ctx.dm().writeToStdOut("app->getComponents returns, size=%d\n",comps->size());

	DMArray *res = new DMArray(true,false,false);

	ListIterator<Component> iter(*comps);
	for(Component *c = iter.first(); c; c = iter.next()) {
		res->add(new Variable(NULL,c));
	}
	return new Expr(res);
}


Expr *function_obj_approvals(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Application *app = (Application*) self->toObjectReference()->toObject();
	bool option = (argc>0)?argv[0]->toBool():true;
	List<Domain> *domains = app->getApprovalDomains(option);
	DMArray *res = new DMArray(true,false,false);

	ListIterator<Domain> iter(*domains);
	for(Domain *dom = iter.first(); dom; dom = iter.next()) {
		res->add(new Variable(NULL,dom));
	}
	return new Expr(res);
}

Expr *function_obj_application_defects(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Application *app = (Application*) self->toObjectReference()->toObject();
	bool option = (argc>0)?argv[0]->toBool():false;
	DMArray *res = new DMArray(true,false,false);
	List<Defect> *defects = app->getDefects(option);
	ListIterator<Defect> iter(*defects);
	for(Defect *d = iter.first(); d; d = iter.next()) {
		res->add(new Variable(NULL,d));
	}
	return new Expr(res);
}



Expr *function_obj_latest(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Application *app = (Application*) self->toObjectReference()->toObject();
	char *label = (argc>0)?argv[0]->toString():NULL;
	ApplicationVersion *av = app->getLatestVersion(label);
	return new Expr(av);
}

Expr *function_obj_environment_getatt(int argc,Expr *self, Expr **argv, Context &ctx)
{
	Environment *env = (Environment *) self->toObjectReference()->toIObject();
	ConstCharPtr attname = (argc>0)?argv[0]->toString():NULL;
	Scope *scope = env->getVars();
	Expr *ret;
	if (attname) {
		Variable *v = scope->get(attname);
		ret = new Expr(v);
	} else {
		DMArray *arr = scope->getVars();
		ret = new Expr(arr);
	}
	return ret;
}

Expr *function_obj_application_getatt(int argc,Expr *self, Expr **argv, Context &ctx)
{
	Application *app = (Application *) self->toObjectReference()->toIObject();
	ConstCharPtr attname = (argc>0)?argv[0]->toString():NULL;
	Scope *scope = app->getVars();
	Expr *ret;
	if (attname) {
		Variable *v = scope->get(attname);
		ret = new Expr(v);
	} else {
		DMArray *arr = scope->getVars();
		ret = new Expr(arr);
	}
	return ret;
}

Expr *function_obj_server_getatt(int argc,Expr *self, Expr **argv, Context &ctx)
{
	Server *server = (Server *) self->toObjectReference()->toIObject();
	ConstCharPtr attname = (argc>0)?argv[0]->toString():NULL;
	Scope *scope = server->getVars();
	Variable *v = scope->get(attname);
	Expr *ret;
	if (attname) {
		Variable *v = scope->get(attname);
		ret = new Expr(v);
	} else {
		DMArray *arr = scope->getVars();
		ret = new Expr(arr);
	}
	return ret;
}

/*
Expr *function_obj_environment_setatt(int argc,Expr *self, Expr **argv, Context &ctx)
{
	Expr *ret = function_obj_environment_getatt(argc,self,argv,ctx);
	Environment *env = (Environment *) self->toObjectReference()->toIObject();
	ConstCharPtr attname = argv[0]->toString();
	env->setVar(attname,argv[1]);
	return ret;
}

Expr *function_obj_application_setatt(int argc,Expr *self, Expr **argv, Context &ctx)
{
	Expr *ret = function_obj_application_getatt(argc,self,argv,ctx);
	Application *app = (Application *) self->toObjectReference()->toIObject();
	ConstCharPtr attname = argv[0]->toString();
	app->setVar(attname,argv[1]);
	return ret;
}

Expr *function_obj_server_setatt(int argc,Expr *self, Expr **argv, Context &ctx)
{
	Expr *ret = function_obj_server_getatt(argc,self,argv,ctx);
	Server *serv = (Server *) self->toObjectReference()->toIObject();
	ConstCharPtr attname = argv[0]->toString();
	serv->setVar(attname,argv[1]);
	return ret;
}
*/

Expr *function_obj_dropzone_dir(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Dropzone *dz = (Dropzone*) self->toObjectReference()->toIObject();
	return new Expr(dz->pathname());
}

Expr *function_obj_dropzone_delete(int argc, Expr *self, Expr **argv, Context &ctx)
{
	DropzoneFile *dzf = (DropzoneFile*) self->toObjectReference()->toIObject();
	CHMOD(dzf->dropzoneFilename(), FILE_WRITE_PERMISSIONS);
	if(unlink((const char *)dzf->dropzoneFilename()) == 0) {
		dzf->markAsDeleted();
		return new Expr(0);	// success
	}
	return new Expr(1);		// failed
}



Expr *function_obj_dirname(int argc, Expr *self, Expr **argv, Context &ctx)
{
	//Expr *args[] = {
	//	argv[0],
	//	self->object_access("type", ctx),
	//	NULL
	//};
	//return function_dirname(2, args, ctx);

	Server *srv = (Server*) self->toObjectReference()->toObject();
	ConstCharPtr pathname = argv[0]->toString();
	AutoPtr<PathNameImpl> path = srv->createPath(pathname);
	if(path) {
		AutoPtr<PathNameImpl> dirname = path->dirname();
		return new Expr(dirname->path());
	}
	throw RuntimeError("Unable to create path '%s' for server '%s'", (const char*) pathname, srv->name());
}


// TODO: This doesn't cope with lists
Expr *function_obj_array_filter(int argc, Expr *self, Expr **argv, Context &ctx)
{
	DMArray *in = self->toArray();
	DMArray *out = new DMArray(false, false, true);

	CharPtr pattern = argv[0]->toString();
	regexp *re = regcomp((char*) pattern);
	if(!re) {
		throw RuntimeError("Bad regular expression '%s'", (const char*) pattern);
	}

	AutoPtr<StringList> keys = in->keys();
	StringListIterator iter(*keys);
	for(const char *key = iter.first(); key; key = iter.next()) {
		if(regexec(re, (char*) key)) {
			out->put(key, new Variable(*(in->get(key))));
		}
	}

	return new Expr(out);
}


Expr *function_obj_dropzone_find(int argc, Expr *self, Expr **argv, Context &ctx)
{
	Dropzone *dz = (Dropzone*) self->toObjectReference()->toIObject();
	CharPtr patt = argv[0]->toString();
	return new Expr(dz->find(patt));
}


Expr *function_obj_array_flatten(int argc, Expr *self, Expr **argv, Context &ctx)
{
	ConstCharPtr str = self->stringify();
	return new Expr((const char*) str);
}

Expr *function_obj_array_to_json(int argc, Expr *self, Expr **argv, Context &ctx)
{
	DMArray *res = self->toArray();
	ConstCharPtr str = res->toJSON();
	return new Expr((const char*) str);
}


Expr *function_obj_string_length(int argc, Expr *self, Expr **argv, Context &ctx)
{
	CharPtr str = self->toString();
	return new Expr((int) strlen(str));
}

Expr *function_obj_stream_length(int argc, Expr *self, Expr **argv, Context &ctx)
{
	OutputStream *stream = self->toStream();
	return new Expr((int)stream->size());
}

Expr *function_obj_stream_base64_encode(int argc, Expr *self, Expr **argv, Context &ctx)
{
	OutputStream *stream = self->toStream();
	char *res = base64encode((unsigned char *)stream->buffer(),stream->size());
	return new Expr(res);

}

Expr *function_obj_stream_base64_decode(int argc, Expr *self, Expr **argv, Context &ctx)
{
	// We need to be in a stream for this to work. If we're not, throw a runtime
	// exception.
	OutputStream *s = NULL;
	if (argc>0) {
		// Stream Specified
		if (argv[0]->kind() != KIND_STREAM) throw RuntimeError("Invalid Parameter: not a 'stream' object");
		s = argv[0]->toStream();
	} else {
		// No stream parameter - we need to be in "using stream" block
		if (!ctx.inUsingStream()) throw RuntimeError("base64decode needs 'stream' parameter or to be within a 'using stream' block");
	}
	
	int retlen;
	// Remove all newline chars from the base64 encoded string
	char *ts = (char *)malloc(strlen(self->toString())+1);
	char *ss = self->toString();
	int dslen = strlen(ss);
	int x=0;
	for (int i=0;i<dslen;i++) {
		if (ss[i]!='\n' && ss[i]!='\r') ts[x++]=ss[i];
	}
	ts[x]='\0';
	unsigned char *res = decryptbase64(ts,&retlen);
	free(ts);
	// Write the decoded data to output (it will be written to the stream)
	if (s) {
		s->writeBufferToStdOut(0,(const char *)res,retlen);
	} else {
		ctx.writeBufferToStdOut(0,(const char *)res,retlen);
	}
	return new Expr(retlen);	// return is the length of the decoded data
	
}


Expr *function_obj_array_length(int argc, Expr *self, Expr **argv, Context &ctx)
{
	return new Expr(self->toArray()->count());
}


Expr *function_obj_string_lower(int argc, Expr *self, Expr **argv, Context &ctx)
{
	return self->toLower();
}


Expr *function_obj_string_upper(int argc, Expr *self, Expr **argv, Context &ctx)
{
	return self->toUpper();
}

Expr *function_obj_string_parsexml(int argc, Expr *self, Expr **argv, Context &ctx)
{
	// Convert XML to JSON format
	try {
		std::string json_str = xml2json(self->toString());
		const char *c = json_str.c_str();
		LexerBuffer lb(expr_lexer, (char *)c, NULL);
		return lb.parseExpression(ctx); // May raise an exception
	} catch(rapidxml::parse_error err) {
		throw RuntimeError(err.what());
	}
}

Expr *function_obj_stream_parsexml(int argc, Expr *self, Expr **argv, Context &ctx)
{
	// Convert XML to JSON format
	try {
		OutputStream *stream = self->toStream();
		std::string json_str = xml2json(stream->buffer());
		const char *c = json_str.c_str();
		LexerBuffer lb(expr_lexer, (char *)c, NULL);
		return lb.parseExpression(ctx); // May raise an exception
	} catch(rapidxml::parse_error err) {
		throw RuntimeError(err.what());
	}
}

Expr *function_obj_string_split(int argc, Expr *self, Expr **argv, Context &ctx)
{
	ConstCharPtr s = self->toString();
	ConstCharPtr c = argv[0]->toString();
	DMArray *res = new DMArray(true,false,false);
	char *temp = strdup(s);
	for(char *f = strtok(temp, c); f; f = strtok(NULL, c)) {
		res->add(new Variable(NULL,f));
	}
	SAFE_FREE(temp);
	return new Expr(res);
}

Expr *function_obj_string_instr(int argc, Expr *self, Expr **argv, Context &ctx)
{
	ConstCharPtr s = self->toString();
	ConstCharPtr c = argv[0]->toString();
	const char *res = strstr(s,c);
	return new Expr(res?(int)(res-((const char *)s)):(int)(-1)); 
}


Expr *function_obj_hex(int argc, Expr *self, Expr **argv, Context &ctx)
{
	char *num = self->toString();
	int base=10;
	char buf[256];
	char *endptr;
	if (argc==1) {
		// Conversion base has been specified
		base = argv[0]->toInt();
	}
	sprintf(buf, "%x", strtol(num,&endptr,base));
	return new Expr(buf);
}

Expr *function_obj_decimal(int argc, Expr *self, Expr **argv, Context &ctx)
{
	char *num = self->toString();
	int base=10;
	char buf[256];
	char *endptr;
	if (argc==1) {
		// Conversion base has been specified
		base = argv[0]->toInt();
	}
	sprintf(buf, "%ld", strtol(num,&endptr,base));
	return new Expr(buf);
}



Expr *function_obj_octal(int argc, Expr *self, Expr **argv, Context &ctx)
{
	char *num = self->toString();
	int base=10;
	char buf[256];
	char *endptr;
	if (argc==1) {
		// Conversion base has been specified
		base = argv[0]->toInt();
	}
	sprintf(buf, "%o", strtol(num,&endptr,base));
	return new Expr(buf);
}


Expr *function_obj_regexp(int argc, Expr *self, Expr **argv, Context &ctx)
{
	return new Expr(KIND_ERROR, "regexp() not yet implemented!");
}


Expr *function_obj_regsub(int argc, Expr *self, Expr **argv, Context &ctx)
{
	// regexp library uses nasty char*s, so can't use CharPtr's
	ConstCharPtr val = self->toString();
	ConstCharPtr re = argv[0]->toString();
	ConstCharPtr sub = argv[1]->toString();

	//debug1("val = '%s'; re = '%s'", val, re);

	//regexp *ry = regcomp(re);
	//if(!ry) {
	//	throw RuntimeError("Bad regular expression '%s'", re);
	//}

	Expr *ret = NULL;

	//if(regexec(ry, val)) {
	//	char dst[1024];
	//	regsub(ry, sub, dst);
	//	ret = new Expr(dst);		// Expr will strdup the buffer
	//} else {
	//	ret = new Expr("");
	//}

	RegExpWrapper ry(re);
	ret = new Expr(ry.replaceAll(val, sub));

	//SAFE_FREE(ry);
	return ret;
}


Expr *function_obj_rtrim(int argc, Expr *self, Expr **argv, Context &ctx)
{
	CharPtr str = self->stringify();
	for(int n = strlen(str) - 1;
		(n > 0) && ((str[n] == '\r') || (str[n] == '\n')
		  || (str[n] == ' ') || (str[n] == '\t')); n--) {
			str[n] = '\0';
	}
	return new Expr((const char*) str);
}


Expr *function_obj_substr(int argc, Expr *self, Expr **argv, Context &ctx)
{
	ConstCharPtr str = self->stringify();			// was toString, but want to handle streams
	int olen = strlen(str);
	int pos = argv[0]->toInt();
	if(pos >= olen) {
		return new Expr("");
	}

	if(argc == 2) {
		int nlen = argv[1]->toInt();
		CharPtr dup = strdup(&str[pos]);
		if(nlen < strlen(dup)) {
			dup[nlen] = '\0';
		}
		return new Expr((const char*) dup);
	}

	return new Expr(&str[pos]);					// Expr will strdup this
}


extern int yyparse(void *param);

Expr *function_obj_array_sum(int argc, Expr *self, Expr **argv, Context &ctx)
{
	DMArray *arr = self->toArray();

	ConstCharPtr expr = argv[0]->toString();
	LexerBuffer lb(expr_lexer, expr, NULL);
	yyparse_param param;
	param.buffer = &lb;
	int res = yyparse(&param);
	if((res != 0) && !param.expr) {
		if(param.ex) {
			param.ex->print(ctx.dm());
			throw *(param.ex->clone());
		}
		return new Expr();
	}

	// parsed okay, now evaluate once for each key
	Scope *maxScope = new Scope("max");
	ctx.stack().push(maxScope);
	int total = 0;
	try {
		AutoPtr<ExprList> keys = self->array_keys();
		ListIterator<Expr> iter(*keys);
		for(Expr *k = iter.first(); k; k = iter.next()) { 
			ConstCharPtr str = k->toString();
			maxScope->set("key", (const char*) str);			
			Variable *value = arr->get(str);
			if(value) {
				switch(value->type()) {
				case vartype_bool:    maxScope->set("value", value->getInt()); break;	// should be 1/0
				case vartype_integer: maxScope->set("value", value->getInt()); break;
				case vartype_string:  maxScope->set("value", value->getString()); break;
				case vartype_array:   maxScope->set("value", value->getArray()); break;
				case vartype_object:  maxScope->set("value", value->getObjectReference()); break;
				case vartype_stream:  maxScope->set("value", value->getStream()); break;
				}
			} else {
				maxScope->set("value", "");
			}
			ExprPtr e = param.expr->evaluate(ctx, false);
			total += e->toInt();
		}
	} catch(...) {
		delete param.expr;
		ctx.stack().pop("max");
		throw;
	}
	delete param.expr;
	ctx.stack().pop("max");

	return new Expr(total);
}


/**
 * date.to_char([fmt [, offset]]) or to_char(date [, fmt [, offset]])
 */
Expr *function_obj_date_to_char(int argc, Expr *self, Expr **argv, Context &ctx)
{
	DateTime *date = self->toDate();
	ConstCharPtr fmt;
	int offset = 0;

	if(argc > 0) {
		fmt = argv[0]->toString();
	}
	if(argc > 1) {
		offset = argv[1]->toInt() * 3600;
	}

	Expr *ret = NULL;
	if(offset != 0) {
		AutoPtr<DateTime> date2 = *date + offset;
		ret = new Expr(date2->toString(fmt));
	} else {
		ret = new Expr(date->toString(fmt));
	}
	return ret;
}

Expr *function_obj_date_to_int(int argc, Expr *self, Expr **argv, Context &ctx)
{
	DateTime *date = self->toDate();
	ConstCharPtr fmt;
	int offset = 0;

	if(argc > 0) {
		offset = argv[0]->toInt();
	}

	Expr *ret = NULL;
	if(offset != 0) {
		AutoPtr<DateTime> date2 = *date + offset;
		ret = new Expr(date2->toInt());
	} else {
		ret = new Expr(date->toInt());
	}
	return ret;
}

// This table is only scanned for functions. Note that several attributes (such as children for applications)
// are also handled in ::getAttribute. 
DMObjFnTable obj_fn_table[] = {
	{ "append",			KIND_OBJECT, OBJ_KIND_SERVER,      2, 2, function_obj_append }, 
	{ "appendstd",		KIND_OBJECT, OBJ_KIND_SERVER,      2, 2, function_obj_appendstd },  
	{ "applications",	KIND_OBJECT, OBJ_KIND_DOMAIN,      0, 0, function_obj_applications },
	{ "approvals",		KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_approvals },
	{ "base64decode",	KIND_STR,    OBJ_KIND_NONE,        0, 1, function_obj_stream_base64_decode },
	{ "base64encode",	KIND_STREAM, OBJ_KIND_NONE,        0, 0, function_obj_stream_base64_encode },
	{ "basename",		KIND_OBJECT, OBJ_KIND_SERVER,      1, 1, function_obj_basename },
	{ "children",		KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_children },
	{ "children",		KIND_OBJECT, OBJ_KIND_APPVERSION,  0, 1, function_obj_children },
	{ "components",		KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_components },
	{ "decimal",		KIND_ALL,	 OBJ_KIND_NONE,        0, 1, function_obj_decimal },
	{ "defects",		KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_application_defects },
	{ "defects",		KIND_OBJECT, OBJ_KIND_APPVERSION,  0, 1, function_obj_application_defects },
	{ "delete",			KIND_OBJECT, OBJ_KIND_DROPZONEFILE,0, 0, function_obj_dropzone_delete },
	{ "deptime",		KIND_OBJECT, OBJ_KIND_SERVER,      1, 1, function_obj_server_deptime },
	{ "deptime",		KIND_OBJECT, OBJ_KIND_ENVIRONMENT, 1, 1, function_obj_environment_deptime },
	{ "dirname",		KIND_OBJECT, OBJ_KIND_SERVER,      1, 1, function_obj_dirname },
	{ "dir",			KIND_OBJECT, OBJ_KIND_DROPZONE,    0, 0, function_obj_dropzone_dir },
	{ "filter",			KIND_ARRAY,  OBJ_KIND_NONE,        1, 1, function_obj_array_filter },
	{ "find",			KIND_OBJECT, OBJ_KIND_DROPZONE,    1, 1, function_obj_dropzone_find },
	{ "flatten",		KIND_ARRAY,  OBJ_KIND_NONE,        0, 0, function_obj_array_flatten },
	{ "getatt",			KIND_OBJECT, OBJ_KIND_ENVIRONMENT, 0, 1, function_obj_environment_getatt },
	{ "getatt",			KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_application_getatt },
	{ "getatt",			KIND_OBJECT, OBJ_KIND_APPVERSION,  0, 1, function_obj_application_getatt },
	{ "getatt",			KIND_OBJECT, OBJ_KIND_SERVER,      0, 1, function_obj_server_getatt },
	{ "hex",			KIND_ALL,    OBJ_KIND_NONE,        0, 1, function_obj_hex },
	{ "instr",			KIND_STR,    OBJ_KIND_NONE,        1, 1, function_obj_string_instr },
	{ "latest",			KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_latest },
	{ "length",			KIND_STR,    OBJ_KIND_NONE,        0, 0, function_obj_string_length },
	{ "length",			KIND_ARRAY,  OBJ_KIND_NONE,        0, 0, function_obj_array_length },
	{ "length",			KIND_STREAM, OBJ_KIND_NONE,        0, 0, function_obj_stream_length },
	{ "lower",			KIND_STR,    OBJ_KIND_NONE,        0, 0, function_obj_string_lower },
	{ "octal",			KIND_ALL,    OBJ_KIND_NONE,        0, 1, function_obj_octal },
	{ "regexp",			KIND_ALL,    OBJ_KIND_NONE,        1, 1, function_obj_regexp },
	{ "regsub",			KIND_ALL,    OBJ_KIND_NONE,        2, 2, function_obj_regsub },
	{ "requests",		KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_application_defects },
	{ "requests",		KIND_OBJECT, OBJ_KIND_APPVERSION,  0, 1, function_obj_application_defects },
	{ "rtrim",			KIND_ALL,    OBJ_KIND_NONE,        0, 0, function_obj_rtrim },
	//{ "setatt",			KIND_OBJECT, OBJ_KIND_ENVIRONMENT, 2, 2, function_obj_environment_setatt },
	//{ "setatt",			KIND_OBJECT, OBJ_KIND_APPLICATION, 2, 2, function_obj_application_setatt },
	//{ "setatt",			KIND_OBJECT, OBJ_KIND_APPVERSION,  2, 2, function_obj_application_setatt },
	//{ "setatt",			KIND_OBJECT, OBJ_KIND_SERVER,      2, 2, function_obj_server_setatt },
	{ "split",			KIND_STR,    OBJ_KIND_NONE,        1, 1, function_obj_string_split },
	{ "stdpath",		KIND_OBJECT, OBJ_KIND_SERVER,      1, 1, function_obj_stdpath },    
	{ "substr",			KIND_ALL,    OBJ_KIND_NONE,        1, 2, function_obj_substr },
	{ "sum",			KIND_ARRAY,  OBJ_KIND_NONE,        1, 1, function_obj_array_sum },
	{ "to_char",		KIND_DATE,   OBJ_KIND_NONE,        0, 2, function_obj_date_to_char },
	{ "to_int",			KIND_DATE,   OBJ_KIND_NONE,        0, 1, function_obj_date_to_int },
	{ "to_json",		KIND_ARRAY,	 OBJ_KIND_NONE,        0, 0, function_obj_array_to_json },
	{ "upper",			KIND_STR,    OBJ_KIND_NONE,        0, 0, function_obj_string_upper },
	{ "versions",		KIND_OBJECT, OBJ_KIND_APPLICATION, 0, 1, function_obj_children },
	{ "xmlparse",		KIND_STR,    OBJ_KIND_NONE,        0, 0, function_obj_string_parsexml },
	{ "xmlparse",		KIND_STREAM, OBJ_KIND_NONE,        0, 0, function_obj_stream_parsexml },
	{ NULL, 0, OBJ_KIND_NONE, 0, 0, NULL }
};


///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////


/**
 * createPath() always returns a new PathNameImpl or throws an exception - it
 * will never return NULL.  If epathfmt is specified, this is used, otherwise
 * we attempt to find a server on the stack and use the path format from that.
 */
PathNameImpl *createPath(Expr *epath, Expr *epathfmt, Context &ctx)
{
	ConstCharPtr pathname = epath->toString();

	PathNameImpl *ret = NULL;

	if(epathfmt) {
		ConstCharPtr pathformat = epathfmt->toString();
		PathNameImplFactory *fact = PathNameImplRegistry::instance().getFactory(pathformat);
		if(!fact) {
			throw RuntimeError(ctx.stack(), "Unable to find factory for path format '%s'", (const char*) pathformat);
		}
		if(!(ret = fact->create(pathname))) {
			throw RuntimeError("Unable to create path '%s' for path format '%s'", (const char*) pathname, (const char*) pathformat);
		}
	} else {
		Scope * ss = ctx.stack().getScope(SERVER_SCOPE);
		IObject * so = ss ? ss->getScopeObject() : NULL;
		if(!so) {
			throw RuntimeError(ctx.stack(), "No server on stack - please use 2 argument or object form");
		}
		Server *s = (Server*) so;
		if(!(ret = s->createPath(pathname))) {
			throw RuntimeError("Unable to create path '%s' for server '%s'", (const char*) pathname, s->name());
		}
	}
	return ret;
}


Expr *function_now(int argc, Expr **argv, Context &ctx)
{
	//ConstCharPtr fmt;
	//if(argc > 0) {
	//	ConstCharPtr fmt = argv[0]->toString();
	//} else {
	//	fmt = strdup("%#c");
	//}
	//char buf[256];
	//time_t timenow;
	//struct tm *t;
	//time(&timenow);
	//t = localtime(&timenow);
	//if(strftime(buf, sizeof(buf), fmt, t) > 0) {
	//	return new Expr(buf);
	//}
	//return new Expr(KIND_ERROR, "Failed to format time");
	return new Expr(new DateTime());
}


/**
 * to_date(year, month, day [, hour [, min [, sec]]])
 */
Expr *function_to_date(int argc, Expr **argv, Context &ctx)
{
	struct tm t;
	if (argc==1) {
		// Single parameter = epoch
		return new Expr(new DateTime((long)argv[0]->toInt()));
	} else if (argc==2) {
		throw RuntimeError(ctx.stack(), "Invalid Parameters");
	} else if (argc>=3) {
		time_t x;
		t.tm_wday = t.tm_yday = 0;
		t.tm_isdst = -1;
		t.tm_year = argv[0]->toInt();
		t.tm_mon  = argv[1]->toInt() - 1;
		t.tm_mday = argv[2]->toInt();
		t.tm_hour = (argc > 3) ? argv[3]->toInt() : 0;
		t.tm_min  = (argc > 4) ? argv[4]->toInt() : 0;
		t.tm_sec  = (argc > 5) ? argv[5]->toInt() : 0;

		if (t.tm_year < 1970) throw RuntimeError(ctx.stack(),"Years prior to 1970 are not supported");

		if(t.tm_year > 1900) {
			t.tm_year -= 1900;
		}

		time_t tt = mktime(&t);
		if(tt == -1) {
			throw RuntimeError(ctx.stack(), "Bad date");
		}

		return new Expr(new DateTime((long) tt));
	}
}


Expr *function_dropzone(int argc, Expr **argv, Context &ctx)
{
	ConstCharPtr dropzone = argv[0]->toString();
	if(dropzone) {
		Dropzone *dz = ctx.dm().getDropzone(dropzone);
		if(!dz) {
			dz = ctx.dm().newDropzone(dropzone, false);
			dz->create(true);
		}
		if(dz) {
			return new Expr(dz->pathname());
		}
	}	
	return new Expr(KIND_ERROR, "failed to get dropzone");
}


/**
 * basename(pathname [, pathformat])
 */
Expr *function_basename(int argc, Expr **argv, Context &ctx)
{
	//char *name = argv[0]->toString();
	//char *s1 = strrchr(name, '/');
	//char *s2 = strrchr(name, '\\');
	//if(!s1 && !s2) {
	//	return new Expr(KIND_ERROR, "Not a valid path");
	//}
	//return new Expr((s1 > s2) ? strdup(++s1) :  strdup(++s2));

	AutoPtr<PathNameImpl> path = createPath(argv[0], (argc>1 ? argv[1] : NULL), ctx);
	AutoPtr<PathNameImpl> basename = path->basename();
	return new Expr(basename->path());
}


/**
 * dirname(pathname [, pathformat])
 */
Expr *function_dirname(int argc, Expr **argv, Context &ctx)
{
	//char *name = strdup(argv[0]->toString());
	//char *s1 = strrchr(name, '/');
	//char *s2 = strrchr(name, '\\');
	//if(!s1 && !s2) {
	//	return new Expr(KIND_ERROR, "Not a valid path");
	//}
	//if(s1 > s2) { *s1 = '\0'; } else { *s2 = '\0'; }
	//return new Expr(name);	

	AutoPtr<PathNameImpl> path = createPath(argv[0], (argc>1 ? argv[1] : NULL), ctx);
	AutoPtr<PathNameImpl> dirname = path->dirname();
	return new Expr(dirname->path());
}


/**
 * append(pathname1, pathname2 [, pathformat])
 */
Expr *function_append(int argc, Expr **argv, Context &ctx)
{
	ConstCharPtr pathname2 = argv[1]->toString();
	AutoPtr<PathNameImpl> path1 = createPath(argv[0], (argc>2 ? argv[2] : NULL), ctx);
	AutoPtr<PathNameImpl> path2 = path1->append(pathname2);
	return new Expr(path2->path());
}


/**
 * appendstd(pathname1, pathname2 [, pathformat])
 */
Expr *function_appendstd(int argc, Expr **argv, Context &ctx)
{
	ConstCharPtr pathname2 = argv[1]->toString();
	AutoPtr<PathNameImpl> path1 = createPath(argv[0], (argc>2 ? argv[2] : NULL), ctx);
	AutoPtr<PathNameImpl> path2 = path1->appendstd(pathname2);
	return new Expr(path2->path());
}


/**
 * stdpath(pathname [, pathformat])
 */
Expr *function_stdpath(int argc, Expr **argv, Context &ctx)
{
	AutoPtr<PathNameImpl> path = createPath(argv[0], (argc>1 ? argv[1] : NULL), ctx);
	return new Expr(path->stdpath());
}


/**
 * random(limit)
 */
Expr *function_random(int argc, Expr **argv, Context &ctx)
{
	int limit = argv[0]->toInt();
	int n = rand();
	return new Expr(n % limit);
}

Expr *function_getenvironment(int argc,Expr **argv, Context &ctx)
{
	ConstCharPtr envname = argv[0]->toString();
	return new Expr(ctx.dm().getModel()->getEnvironment(envname,true));
}

Expr *function_getcredential(int argc,Expr **argv, Context &ctx)
{
	ConstCharPtr credname = argv[0]->toString();
	return new Expr(ctx.dm().getModel()->getCredential(credname,true));
}

Expr *function_getapplication(int argc,Expr **argv, Context &ctx)
{
	ConstCharPtr appname = argv[0]->toString();
	return new Expr(ctx.dm().getModel()->getApplication(appname,true));
}

Expr *function_getuser(int argc,Expr **argv, Context &ctx)
{
	ConstCharPtr username = argv[0]->toString();
	return new Expr(ctx.dm().getModel()->getUser(username));
}

Expr *function_getusergroup(int argc,Expr **argv, Context &ctx)
{
	ConstCharPtr groupname = argv[0]->toString();
	return new Expr(ctx.dm().getModel()->getUserGroup(groupname,true));
}

Expr *function_me(int argc,Expr **argv, Context &ctx)
{
	return new Expr(ctx.dm().getModel()->getCurrentUser());
}

Expr *function_getserver(int argc,Expr **argv, Context &ctx)
{
	ConstCharPtr servname = argv[0]->toString();
	return new Expr(ctx.dm().getModel()->getServer(servname,true));
}

Expr *function_getcomponent(int argc,Expr **argv, Context &ctx)
{
	ConstCharPtr compname = argv[0]->toString();
	return new Expr(ctx.dm().getModel()->getComponent(compname));
}

DMExprFnTable fn_table[] = {
	{ "append",			2, 3, function_append },
	{ "appendstd",		2, 3, function_appendstd },
	{ "basename",		1, 2, function_basename },
	{ "dirname",		1, 2, function_dirname },
	{ "dropzone",		1, 1, function_dropzone },
	{ "getapplication",	1, 1, function_getapplication },
	{ "getcomponent",	1, 1, function_getcomponent },
	{ "getcredential",	1, 1, function_getcredential },
	{ "getenvironment", 1, 1, function_getenvironment },
	{ "getserver",		1, 1, function_getserver },
	{ "getuser",		1, 1, function_getuser },
	{ "getusergroup",	1, 1, function_getusergroup },
	{ "me",				0, 0, function_me },
	{ "now",			0, 0, function_now },
	{ "random",			1, 1, function_random },
	{ "stdpath",		1, 2, function_stdpath },
	{ "date",			1, 6, function_to_date },
	{ NULL, 0, 0, NULL }
};


///////////////////////////////////////////////////////////////////////////////


ObjectReference::ObjectReference(Scope* stack)
	: m_kind(OBJREF_STACK), m_stack(stack)
{}


ObjectReference::ObjectReference(IObject *obj)
	: m_kind(OBJREF_OBJECT), m_obj(obj)
{}


ObjectReference::ObjectReference(class DMException &ex)
	: m_kind(OBJREF_EXCEPTION), m_ex(ex.clone())
{}


ObjectReference::ObjectReference(const ObjectReference &copy)
	: m_kind(copy.m_kind)
{
	switch(copy.m_kind) {
	case OBJREF_STACK: 	   m_stack = copy.m_stack; break;
	case OBJREF_OBJECT:    m_obj   = copy.m_obj; break;
	case OBJREF_EXCEPTION: m_ex    = (copy.m_ex ? copy.m_ex->clone() : NULL); break;
	}
}


ObjectReference::~ObjectReference()
{
	if(m_kind == OBJREF_EXCEPTION) {
		SAFE_DELETE(m_ex);
	}
}


Expr *ObjectReference::object_access(const char *str, Context &ctx) const
{
	switch(m_kind)
	{
	case OBJREF_STACK: {
		// First check to see if the stack frame has a variable of this name
		Variable* var = m_stack->get(str);
		if(var) {
			return new Expr((Variable*) var);
		}
		// Not a variable, is it an XML attribute
		if(var = m_stack->getXML(str, ctx)) {
			AutoPtr<Variable> autoVar(var);			// getXML() always returns a new Variable
			return new Expr((Variable*) var);
		}
		}
		break;
	case OBJREF_OBJECT:
		// Meta-objects are not on the stack, for now we look only at their attributes
		// TODO: Look at variables defined by the object - this means relating the
		// object to the model version with its populated variable set
		if(m_obj) {
			//const char *ret = m_obj->getAttribute(str, ctx);
			//if(ret) {
			//	return new Expr(ret);
			//}
			//DMArray *ht = m_obj->getArrayAttribute(str, ctx);
			//if(ht) {
			//	return new Expr(ht);
			//}
			//IObject *obj = m_obj->getObjectAttribute(str, ctx);
			//if(obj) {
			//	return new Expr(obj);
			//}
			return m_obj->getAttribute(str, ctx);
		}
		break;
	case OBJREF_EXCEPTION:
		if(m_ex) {
			if(strcmp(str, "message") == 0) {
				return new Expr(m_ex->getMessage());
			} else if(strcmp(str, "kind") == 0) {
				return new Expr(m_ex->getKind());
			} else if(strcmp(str, "stacktrace") == 0) {
				const char *trace = m_ex->getStacktrace();
				return new Expr(trace ? trace : "");
			}
		}
		break;
	default:
		debug1("Corrupt object reference - object_access('%s', ...) - kind %d", str, m_kind);
		break;
	}

	//return new Expr(KIND_ERROR, "No such property");
	debug1("No such property '%s'", str);
	return new Expr("");
}


class IObject *ObjectReference::toIObject() const
{
	switch(m_kind)
	{
	case OBJREF_STACK:
		return m_stack->getScopeObject();
	case OBJREF_OBJECT:
		return m_obj;
	}
	return NULL;
}


class Object *ObjectReference::toObject() const
{
	switch(m_kind)
	{
	case OBJREF_STACK: {
		IObject *obj = m_stack->getScopeObject();
		return obj ? obj->toObject() : NULL;
		}
	case OBJREF_OBJECT:
		return m_obj ? m_obj->toObject() : NULL;
	}
	return NULL;
}


OBJECT_KIND ObjectReference::kind() const
{
	switch(m_kind) {
	case OBJREF_STACK:
	case OBJREF_EXCEPTION:
		return OBJ_KIND_NONE;
	case OBJREF_OBJECT: {
		Object *obj = m_obj->toObject();
		return obj ? obj->kind() : OBJ_KIND_NONE;
		}
	}
	return OBJ_KIND_NONE;
}


char *ObjectReference::stringify() const
{
	if((m_kind == OBJREF_EXCEPTION) && m_ex) {
		const char *msg = m_ex->getMessage();
		return msg ? strdup(msg) : strdup("");
	}

	return strdup("");
}


// Compare object references - at the moment just compare pointers and rely on caching
int ObjectReference::compare(ObjectReference &other) const
{
	Object *aobj = toObject();
	Object *bobj = other.toObject();
	// Some kind of object - compare ids if of the same kind
	if(aobj && bobj && (aobj->kind() == bobj->kind())) {
		if(aobj->kind() == OBJ_KIND_APPVERSION) {
			debug1("Comparing two application versions with ids %d and %d", aobj->id(), bobj->id());
			// Are a and b the same object?
			if(aobj->id() == bobj->id()) return 0;
			// First test path from a to b
			for(ApplicationVersion *av = (ApplicationVersion*) aobj; av; av = av->getPredecessor()) {
				if(av->id() == bobj->id()) {
					return 1;
				}
			}
			// No joy, so try path from b to a
			for(ApplicationVersion *av = (ApplicationVersion*) bobj; av; av = av->getPredecessor()) {
				if(av->id() == aobj->id()) {
					return -1;
				}
			}
			// No path between a and b in either direction, so not related
			throw RuntimeError(
				"Invalid comparison - application versions %d and %d are not siblings",
				aobj->id(), bobj->id());
		}

		debug1("Comparing two objects of kind %d with ids %d and %d", aobj->kind(), aobj->id(), bobj->id());
		if(aobj->id() > bobj->id()) return 1;
		if(aobj->id() < bobj->id()) return -1;
		return 0;
	}
	// Not an object, so just compare pointers - what else can we do?
	debug1("Comparing two objects - pointers are %p and %p", m_obj, other.m_obj);
	if(m_obj > other.m_obj) return 1;
	if(m_obj < other.m_obj) return -1;
	return 0;
}


void ObjectReference::print() const
{
	switch(m_kind) {
	case OBJREF_STACK:     printf("\t=>\t@%s\n", m_stack->name()); break;
	case OBJREF_OBJECT:    {
		Object *obj = m_obj->toObject();
		int kind = obj ? obj->kind() : OBJ_KIND_NONE;
		const char *name = obj ? obj->name() : "<not_an_object>";
		printf("\t=>\t<%d:%s>\n", kind, name);
		}
		break;
	case OBJREF_EXCEPTION: printf("\t=>\t<exception:%s>\n", m_ex->getMessage()); break;
	}
}


///////////////////////////////////////////////////////////////////////////////


Expr::Expr()
	: m_kind(KIND_ERROR), m_str(NULL), m_nocase(false), m_simple(false)
{}


Expr::Expr(bool bval)
	: m_kind(KIND_BOOL), m_bval(bval), m_nocase(false), m_simple(true)
{}


Expr::Expr(int ival)
	: m_kind(KIND_INT), m_ival(ival), m_nocase(false), m_simple(true)
{}


Expr::Expr(const char *str, bool nocase /*= false*/)
	: m_kind(KIND_STR), m_str(DUP_NULL(str)), m_nocase(nocase), m_simple(true)
{}


Expr::Expr(class DMArray *arr)
	: m_kind(KIND_ARRAY), m_arr(arr), m_nocase(false), m_simple(true)
{
	// Increment reference count on the array
	if(arr) {
		arr->addRef();
	}
}


Expr::Expr(class IObject *obj)
	: m_kind(KIND_OBJECT), m_obj(obj ? new ObjectReference(obj) : NULL),
	  m_nocase(false), m_simple(true)
{}


Expr::Expr(class Scope *scope)
	: m_kind(KIND_OBJECT), m_obj(scope ? new ObjectReference(scope) : NULL),
	  m_nocase(false), m_simple(true)
{}


Expr::Expr(class ObjectReference *objref)
	: m_kind(KIND_OBJECT), m_obj(objref ? new ObjectReference(*objref) : NULL),
	  m_nocase(false), m_simple(true)
{}


Expr::Expr(class DateTime *date)
	: m_kind(KIND_DATE), m_date(date ? new DateTime(*date) : NULL), m_nocase(false), m_simple(false)
{}


Expr::Expr(int kind, const char* str)
	: m_kind(kind), m_str(DUP_NULL(str)), m_nocase(false), m_simple(false)
{}


Expr::Expr(Variable *var)
	: m_str(NULL), m_nocase(false), m_simple(false)
{
	switch(var->type()) {
	case vartype_integer:
		m_kind = KIND_INT;
		m_ival = var->getInt();
		break;
	case vartype_string:
		m_kind = KIND_STR;
		m_str = strdup(var->getString());
		m_nocase = var->getNoCase();
		break;
	case vartype_array:
		m_kind = KIND_ARRAY;
		m_arr = var->getArray();
		// Increment reference count on the array
		if(m_arr) {
			m_arr->addRef();
		}
		break;
	case vartype_object: {
		m_kind = KIND_OBJECT;
		// Expr owns the objref and will delete it, so use copy constructor
		ObjectReference *objref = var->getObjectReference();
		m_obj = objref ? new ObjectReference(*objref) : NULL;
		}
		break;
	case vartype_stream:
		m_kind = KIND_STREAM;
		m_stream = var->getStream();
		// add our reference
		if(m_stream) {
			m_stream->addRef();
		}
		break;
	case vartype_date: {
		m_kind = KIND_DATE;
		DateTime *date = var->getDate();
		m_date = date ? new DateTime(*date) : NULL;
		}
		break;
	case vartype_bool:
		m_kind = KIND_BOOL;
		m_ival = var->getInt();
		break;
	default:
		throw RuntimeError("Unable to copy variable of type %d", var->type());
	}
}


Expr::Expr(const Expr &copy)
	: m_kind(copy.m_kind), m_nocase(copy.m_nocase), m_simple(copy.m_simple)
{
	switch(copy.m_kind)
	{
	case KIND_BOOL: m_bval = copy.m_bval; break;
	case KIND_INT:  m_ival = copy.m_ival; break;
	case KIND_STR: case KIND_ERROR: m_str = strdup(copy.m_str); break;
	case KIND_ARRAY:
		m_arr = copy.m_arr;
		// Increment reference count on the array
		if(m_arr) {
			m_arr->addRef();
		}
		break;
	case KIND_OBJECT: m_obj = copy.m_obj; break;
	case KIND_STREAM:
		m_stream = copy.m_stream;
		// add our reference
		if(m_stream) {
			m_stream->addRef();
		}
		break;
	case KIND_DATE:
		m_date = copy.m_date ? new DateTime(*copy.m_date) : NULL;
		break;
	default: throw RuntimeError("Unable to copy expr of kind %d", copy.m_kind);
	}
}


Expr::~Expr()
{
	switch(m_kind) {
	case KIND_STR:
	case KIND_ERROR:
		// string is owned
		SAFE_FREE(m_str);
		break;
	case KIND_ARRAY:
		// release our reference
		if(m_arr) {
			m_arr->releaseRef();
		}
		break;
	case KIND_OBJECT:
		// object reference is owned
		SAFE_DELETE(m_obj);
		break;
	case KIND_STREAM:
		// release our reference
		if(m_stream) {
			m_stream->releaseRef();
		}
		break;
	case KIND_DATE:
		// date is owned
		SAFE_DELETE(m_date);
		break;
	}
	
	// arr is not owned
}


Expr *Expr::operator_not(class Context &ctx) const
{
	switch(m_kind) {
	case KIND_BOOL:
	case KIND_INT:
		return new Expr(!toBool());
	case KIND_STR:
		return new Expr(!m_str || (strlen(m_str) == 0));
	case KIND_OBJECT:
		return new Expr(!m_obj);
	}
	return new Expr();
}

	
//Expr *Expr::operator + (const Expr &b) const
Expr *Expr::operator_plus(const Expr &b, Context &ctx) const
{
	if((m_kind == KIND_ARRAY) && (b.m_kind == KIND_ARRAY)) {
		if(m_arr && b.m_arr) {
			return new Expr(*m_arr + *b.m_arr);
		}
		debug1("at least one array is null %p ?= %p", m_arr, b.m_arr);
		return new Expr();
	}
	if(b.isNumber()) {
		switch(m_kind) {
		case KIND_DATE:
			if(m_date) {
				return new Expr(*m_date + b.toInt());
			}
			debug1("date is null");
			return new Expr(b.toInt());
		case KIND_STR: {
			// Adding a number to a string
			char buf[32];
			sprintf(buf,"%d",b.toInt());
			char *tmp = (char *)malloc(strlen(m_str)+strlen(buf)+1);
			sprintf(tmp,"%s%s",m_str,buf);
			Expr *e = new Expr(tmp);
			free(tmp);
			return e;
			}
			break;
		case KIND_OBJECT:
			if(m_obj) {
				Object *obj = m_obj->toObject();
				if(obj && (obj->kind() == OBJ_KIND_APPVERSION)) {
					return new Expr(((ApplicationVersion*) obj)->operator_plus(b.toInt(), ctx));
				}
			}
			// objref is null, fall thru
			break;
		}
	}
	if(isNumber()) {
		switch(b.m_kind) {
		case KIND_DATE:
			if(b.m_date) {
				return new Expr(*b.m_date + toInt());
			}
			debug1("date is null");
			return new Expr(toInt());
		case KIND_STR: {
			// Adding a string to a number. Result is a string
			char buf[32];
			sprintf(buf,"%d",toInt());
			char *s = b.toString();
			char *tmp = (char *)malloc(strlen(buf)+strlen(s)+1);
			sprintf(tmp,"%s%s",buf,s);
			Expr *e = new Expr(tmp);
			free(tmp);
			return e;
			}
			break;
		case KIND_OBJECT:
			if(b.m_obj) {
				Object *obj = b.m_obj->toObject();
				if(obj && (obj->kind() == OBJ_KIND_APPVERSION)) {
					return new Expr(((ApplicationVersion*) obj)->operator_plus(toInt(), ctx));
				}
			}
			// objref is null, fall thru
			break;
		}
	}
	if(isNumber() && b.isNumber()) {
		return new Expr(toInt() + b.toInt());
	}
	if(m_kind == KIND_STR && b.kind() == KIND_STR) {
		// Adding a string to a string
		char *str1 = toString();
		char *str2 = b.toString();
		char *res = (char *)malloc(strlen(str1)+strlen(str2)+1);
		sprintf(res,"%s%s",str1,str2);
		Expr *e = new Expr(res);
		free(res);
		return e;
	}
	return new Expr();
}


/**
 * Unary minus
 */
//Expr *Expr::operator - () const
Expr *Expr::operator_uminus(Context &ctx) const
{
	if(isNumber()) {
		return new Expr(-toInt());
	}
	return new Expr();
}


//Expr *Expr::operator - (const Expr &b) const
Expr *Expr::operator_minus(const Expr &b, Context &ctx) const
{
	if((m_kind == KIND_DATE) && (b.m_kind == KIND_DATE)) {
		if(m_date && b.m_date) {
			return new Expr(*m_date - *b.m_date);
		}
		debug1("at least one date is null %p ?= %p", m_date, b.m_date);
		return new Expr();
	}
	if(b.isNumber()) {
		switch(m_kind) {
		case KIND_DATE: 
			if(m_date) {
				return new Expr(*m_date - b.toInt());
			}
			debug1("date is null");
			return new Expr(-b.toInt());
		case KIND_OBJECT:
			if(m_obj) {
				Object *obj = m_obj->toObject();
				if(obj && (obj->kind() == OBJ_KIND_APPVERSION)) {
					return new Expr(((ApplicationVersion*) obj)->operator_minus(b.toInt(), ctx));
				}
			}
			// objref is null, fall thru
			break;
		}
	}
	if(isNumber() && b.isNumber()) {
		return new Expr(toInt() - b.toInt());
	}
	return new Expr();
}


//Expr *Expr::operator * (const Expr &b) const
Expr *Expr::operator_times(const Expr &b, Context &ctx) const
{
	if(isNumber() && b.isNumber()) {
		return new Expr(toInt() * b.toInt());
	}
	return new Expr();
}


//Expr *Expr::operator / (const Expr &b) const
Expr *Expr::operator_div(const Expr &b, Context &ctx) const
{
	if(isNumber() && b.isNumber()) {
		int bval = b.toInt();
		if(bval != 0) {
			return new Expr(toInt() / bval);
		}
	}
	return new Expr();
}


//Expr *Expr::operator % (const Expr &b) const
Expr *Expr::operator_mod(const Expr &b, Context &ctx) const
{
	if(isNumber() && b.isNumber()) {
		return new Expr(toInt() % b.toInt());
	}
	return new Expr();
}


//Expr *Expr::operator && (const Expr &b) const
Expr *Expr::operator_and(const Expr &b, Context &ctx) const
{
	if(isNumber() && b.isNumber()) {
		return new Expr(toBool() && b.toBool());
	}
	return new Expr();
}


//Expr *Expr::operator || (const Expr &b) const
Expr *Expr::operator_or(const Expr &b, Context &ctx) const
{
	if(isNumber() && b.isNumber()) {
		return new Expr(toBool() || b.toBool());
	}
	return new Expr();
}


int Expr::compare(const Expr &b) const
{
	// printf("in compare m_kind=%d b.m_kind=%d m_str=%s b.m_str=%s\n",m_kind, b.m_kind,m_str,b.m_str);

	if((m_kind == KIND_BOOL) && (b.m_kind == KIND_BOOL)) {
		if(m_bval && !b.m_bval) return 1;
		if(!m_bval && b.m_bval) return -1;
		return 0;
	}
	if((m_kind == KIND_INT) && (b.m_kind == KIND_INT)) {

		// printf("m_ival = %d b.m_ival = %d\n",m_ival,b.m_ival);

		if(m_ival > b.m_ival) return 1;
		if(m_ival < b.m_ival) return -1;
		return 0;
	}
	if((m_kind == KIND_DATE) && (b.m_kind == KIND_DATE)) {
		if(m_date && b.m_date) {
			return m_date->compare(*b.m_date);
		}
		debug1("at least one date is null %p ?= %p", m_date, b.m_date);
		return 1;
	}
	if((m_kind == KIND_STR) && (b.m_kind == KIND_STR)) {
		return (m_nocase || b.m_nocase) ?  STRCASECMP(m_str, b.m_str) : strcmp(m_str, b.m_str);
	}
	if((((m_kind == KIND_INT) || (m_kind == KIND_BOOL)) && (b.m_kind == KIND_STR))
		|| ((m_kind == KIND_STR) && ((b.m_kind == KIND_INT) || (b.m_kind == KIND_BOOL)))) {
		int ia = toInt();
		int ib = b.toInt();
		if(ia > ib) return 1;
		if(ia < ib) return -1;
		return 0;
	}

	if((m_kind == KIND_OBJECT) && (b.m_kind == KIND_OBJECT)) {
		if(m_obj && b.m_obj) return m_obj->compare(*b.m_obj);
		if(!m_obj && !b.m_obj) return 0;
		if(!b.m_obj) return 1;
		if(!m_obj) return -1;
		debug1("unable to compare objects %p ?= %p", m_obj, b.m_obj);
		return 1;
	}

	// TODO: Compare streams

	// TODO: Remove this
	debug1("unhandled compare of types %d and %d\n", m_kind, b.m_kind);
	if(m_kind == KIND_ERROR) { debug1("ERROR: %s\n", m_str); }
	if(b.m_kind == KIND_ERROR) { debug1("ERROR2: %s\n", b.m_str); }
	return 1;
}


bool Expr::matches(const Expr &b) const
{
	char *str = toString();
	char *re = b.toString();
	regexp *ry = regcomp(re);
	if(!ry) {
		throw SyntaxError("Bad regular expression '%s'", re);
	}

	bool ret = regexec(ry, str) ? true : false;
	SAFE_FREE(ry); SAFE_FREE(re); SAFE_FREE(str);
	return ret;
}


Expr *Expr::toLower() const
{
	if(m_kind == KIND_STR) {
		CharPtr val = strdup(m_str);
		for(char *x = val; *x; x++) { *x = tolower(*x); }
		return new Expr((const char*) val);
	}
	return new Expr();
}


Expr *Expr::toUpper() const
{
	if(m_kind == KIND_STR) {
		CharPtr val = strdup(m_str);
		for(char *x = val; *x; x++) { *x = toupper(*x); }
		return new Expr((const char*) val);
	}
	return new Expr();
}


Expr *Expr::array_access(const Expr &a) const
{
	if(m_kind != KIND_ARRAY) {
		return new Expr(KIND_ERROR, "Not an array");
	}

	ConstCharPtr subscript = a.toString();

	Expr *e = NULL;

	Variable *val = m_arr->get(subscript);
	if(val) {
		e = new Expr(val);
	}
	
	if(!e) {	
		return new Expr(KIND_ERROR, "Subscript not found");
	}
	
	e->m_simple = false;
	return e;
}


Expr *Expr::array_length() const
{
	if(m_kind != KIND_ARRAY) {
		return new Expr(KIND_ERROR, "Not an array");
	}

	return new Expr(m_arr->count());
}


ExprList *Expr::array_keys() const
{
	if(m_kind != KIND_ARRAY) {
		return NULL;
	}

	ExprList *ret = new ExprList();
	StringList *keys = m_arr->keys();

	StringListIterator iter(*keys);
	for(const char *key = iter.first(); key; key = iter.next()) {
		ret->add(new Expr(key));
	}

	SAFE_DELETE(keys);
	return ret;
}


Expr *Expr::object_access(const char *str, Context &ctx) const
{
	if((m_kind != KIND_OBJECT) && (m_kind != KIND_ARRAY)) {
		debug1/*1*/("Not an object or array - %s", kindStr());
		if (strcmp(kindStr(),"string")==0) {
			debug1("String is %s",toString());
			
		}
		if (strcmp(kindStr(),"error")==0) {
			debug1("error is %s",this->stringify());
		}
		return new Expr(KIND_ERROR, "Not an object or array");
	}

	// Alternate syntax for array access - treat keys as properties
	if(m_kind == KIND_ARRAY) {
		Variable *val = m_arr->get(str);
		if(!val) {
			debug1("No such element '%s'", str);
			return new Expr("");
		}
		return new Expr(val);
	}

	if(!m_obj) {
		debug1("Object ref was null");
		//return new Expr(KIND_ERROR, "Assertion failure: Object ref was null");
		throw NullReferenceException(ctx.stack());
	}

	Expr *e = m_obj->object_access(str, ctx);
	if(e) {
		e->m_simple = false;
		return e;
	}
	
	//return new Expr(KIND_ERROR, "No such object or property");
	debug1("No such object or property '%s'", str);
	return new Expr("");
}


DMObjFnPtr Expr::find_object_function(const char *str, int argc, Context &ctx)
{
	OBJECT_KIND okind = OBJ_KIND_NONE;
	switch(m_kind) {
	case KIND_OBJECT: {
		if(!m_obj) {
			debug1("Object ref was null");
			throw NullReferenceException(ctx.stack());
		}
		IObject *iobj = m_obj->toIObject();
		if(iobj) { okind = iobj->kind(); }
		}
		break;
	}

	for(DMObjFnTable *f = obj_fn_table; f && f->name; f++) {
		// Name and expression type must match
		// printf("Comparing [%s/%d] to [%s/%d]\n",str,m_kind,f->name,f->ekind);
		if((strcmp(f->name, str) == 0) && ((f->ekind == KIND_ALL) || (f->ekind == m_kind))) {
			// printf("in here KIND_OBJECT=%d okind=%d f->okind=%d\n",KIND_OBJECT,okind,f->okind);
			if((f->ekind == KIND_OBJECT) && (f->okind != OBJ_KIND_NONE) && (okind != f->okind)) {
				// Not the right kind of object
				continue;
			}
			// printf("found it\n");
			if((argc < f->min_argc) || (argc > f->max_argc)) {
				throw RuntimeError(ctx.stack(), "Incorrect number of arguments to method '%s'", str);
			}
			if(!f->fn) {
				throw RuntimeError(ctx.stack(), "Method '%s' not implemented", str);
			}			
			return f->fn;
		}
	}
	objectKindStr(okind);
	//const char *typestr = (m_kind==KIND_OBJECT)?objectKindStr(okind):kindStr();
	//throw RuntimeError(ctx.stack(),"Invalid method '%s' for object type '%s'",str,typestr);

	return NULL;
}


Expr *Expr::object_function(const char *str, const ExprList &args, Context &ctx)
{
	//
	// Called from object.method() syntax
	//
	int argc = args.size();
	
	DMObjFnPtr f = find_object_function(str, argc, ctx);
	if(f) {
		Expr** argv = args.toArgv();
		try {
			Expr *e = (*f)(argc, this, argv, ctx);
			if (e) e->m_simple = false;
			SAFE_FREE(argv);
			return e;
		} catch(...) {
			SAFE_FREE(argv);
			throw;
		}
	}
	
	return new Expr(KIND_ERROR, "Unknown object function");
}


void Expr::print() const
{
	switch(m_kind) {
	case KIND_ERROR:
		if(m_str) {
			printf("\t=>\tERROR: %s\n", m_str);
		} else {
			printf("\t=>\tERROR\n");
		}
		break;
	case KIND_INT: printf("\t=>\t%d\n", m_ival); break;
	case KIND_STR: printf("\t=>\t'%s'\n", m_str); break;
	case KIND_ARRAY: printf("\t=>\tarray\n"); break;
	case KIND_OBJECT:
		if(m_obj) {
			m_obj->print();
		} else {
			printf("\t=>\tnull\n");
		}
		break;
	case KIND_STREAM: printf("\t=>\tstream\n"); break;
	case KIND_DATE: printf("\t=>\tdate(%ld)\n", m_date); break;
	default: printf("\t=>\t<UNKNOWN %d>\n", m_kind); break;
	}
}


int Expr::kind() const
{
	return m_kind;
}


const char* Expr::kindStr() const
{
	switch(m_kind) {
	case KIND_ERROR:  return "error";
	case KIND_BOOL:   return "boolean";
	case KIND_INT:    return "integer";
	case KIND_STR:    return "string";
	case KIND_ARRAY:  return "array";
	case KIND_OBJECT: return "object";
	case KIND_STREAM: return "stream";
	case KIND_DATE:   return "date";
	default: return "*unknown*";
	}
}

const char* Expr::objectKindStr(OBJECT_KIND okind)
{
	switch(okind) {
		case OBJ_KIND_NONE:				return "none";
		case OBJ_KIND_ENVIRONMENT:		return "environment";
		case OBJ_KIND_DOMAIN:			return "domain";
		case OBJ_KIND_SERVER:			return "server";
		case OBJ_KIND_APPLICATION:		return "application";
		case OBJ_KIND_USER:				return "user";
		case OBJ_KIND_USERGROUP:		return "usergroup";
		case OBJ_KIND_REPOSITORY:		return "repository";
		case OBJ_KIND_NOTIFY:			return "notifier";
		case OBJ_KIND_DATASOURCE:		return "datasource";
		case OBJ_KIND_ACTION:			return "action";
		case OBJ_KIND_DROPZONE:			return "dropzone";
		case OBJ_KIND_COMPONENT:		return "component";
		case OBJ_KIND_COMPONENTITEM:	return "componentitem";
		case OBJ_KIND_DROPZONEFILE:		return "dropzonefile";
		case OBJ_KIND_TRANSFER:			return "transfer";
		case OBJ_KIND_APPVERSION:		return "application";
		case OBJ_KIND_TASK:				return "task";
		case OBJ_KIND_CREDENTIALS:		return "credential";
		case OBJ_KIND_DEFECT:			return "defect";
		case OBJ_KIND_BUILDENGINE:		return "buildengine";
		case OBJ_KIND_REPOSITORY_IMPL:	return "repository_impl";
		case OBJ_KIND_NOTIFY_IMPL:		return "notify_impl";
		case OBJ_KIND_DATASOURCE_IMPL:	return "datasource_impl";
		case OBJ_KIND_STATEMENT_IMPL:	return "statement_impl";
		case OBJ_KIND_TASK_IMPL:		return "task_impl";
		case OBJ_KIND_PLUGIN_OBJECT:	return "plugin";
		default:						return "unknown";
	}
}


bool Expr::isSimple() const
{
	return m_simple;
}


const char* Expr::error() const
{
	return m_str;
}


/**
 * This performs a normal conversion to a string - only numbers and strings are
 * converted - arrays, objects, dates and errors return an empty string.
 */
char *Expr::toString() const
{
	switch(m_kind) {
	case KIND_BOOL:
		return strdup(m_bval ? "1" : "0");
	case KIND_INT: {
		char buf[256];
		sprintf(buf, "%d", m_ival);
		return strdup(buf);
		}
	case KIND_STR:
		return strdup(m_str ? m_str : "");
	case KIND_DATE:
		debug1("Attempt to call toString() on a date");
		break;
	}

	// array, object and stream
	return strdup("");
}


/**
 * Performs a forced conversion to a string - numbers and strings are converted
 * as per toString(), arrays are flattened and errors are reduced to their
 * messages.
 */
char *Expr::stringify() const
{
	switch(m_kind) {
	case KIND_BOOL:
		return strdup(m_bval ? "true" : "false");
	case KIND_INT: {
		char buf[256];
		sprintf(buf, "%d", m_ival);
		return strdup(buf);
		}
	case KIND_STR:
		return strdup(m_str ? m_str : "");
	case KIND_ARRAY: {
		// Reuse flatten code in Variable
		Variable temp(NULL, m_arr);
		return temp.flatten();
		}
	case KIND_STREAM:
		return strdup(m_stream->buffer());
	case KIND_OBJECT:
		return m_obj ? m_obj->stringify() : strdup("null");
	case KIND_DATE:
		return m_date ? m_date->toString() : strdup("");
	}
	
	// object
	return strdup("");
}


int Expr::toInt() const
{
	switch(m_kind) {
	case KIND_BOOL: return (m_bval ? 1 : 0);
	case KIND_INT: return m_ival;
	case KIND_STR: return atoi(m_str);
	case KIND_DATE: return m_date ? m_date->toInt() : 0;
	}
	return 0;
}


bool Expr::toBool() const
{
	switch(m_kind) {
	case KIND_BOOL: return m_bval;
	case KIND_INT: return (m_ival ? true : false);
	case KIND_STR: return (atoi(m_str) ? true : false);
	case KIND_DATE: return m_date && m_date->toInt() ? true : false;
	}
	return false;
}


bool Expr::isNumber() const
{
	switch(m_kind) {
	case KIND_BOOL:
	case KIND_INT:
		return true;
	case KIND_STR:
		for(const char *x = m_str; *x; x++) {
			if(!isdigit(*x)) return false;
		}
		return true;
	}
	return false;
}


bool Expr::isNull() const
{
	return ((m_kind == KIND_OBJECT) && !m_obj) ? true : false;
}


class OutputStream *Expr::toStream() const
{
	if(m_kind == KIND_STREAM) {
		return m_stream;
	}
	return NULL;
}


class DMArray *Expr::toArray() const
{
	if(m_kind == KIND_ARRAY) {
		return m_arr;
	}
	return NULL;
}


ObjectReference *Expr::toObjectReference() const
{
	if(m_kind == KIND_OBJECT) {
		return m_obj;
	}
	return NULL;
}


class DateTime *Expr::toDate() const
{
	if(m_kind == KIND_DATE) {
		return m_date;
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////


ExprList::ExprList()
	: List<Expr>(true)
{}


ExprList::~ExprList()
{}


Expr **ExprList::toArgv() const
{
	Expr **argv = (Expr**) malloc((size() + 1) * sizeof(Expr*));
	if (argv) {
		int n = 0;
		ListIterator<Expr> iter(*this);
		for(Expr *e = iter.first(); e; e = iter.next()) {
			argv[n++] = e;
		}
		argv[n] = NULL;
	}
	return argv;
}


///////////////////////////////////////////////////////////////////////////////
// ExprPtr
///////////////////////////////////////////////////////////////////////////////

ExprPtr::ExprPtr()
	: m_ptr(NULL)
{}


ExprPtr::ExprPtr(Expr *ptr)
	: m_ptr(ptr)
{}


ExprPtr::~ExprPtr()
{
	SAFE_DELETE(m_ptr);
}


Expr *ExprPtr::operator =(Expr *ptr)
{
	SAFE_DELETE(m_ptr);
	m_ptr = ptr;
	return m_ptr;
}


Expr *ExprPtr::operator ->()
{
	return m_ptr;
}


ExprPtr::operator Expr*()
{
	return m_ptr;
}


Expr &ExprPtr::operator *()
{
	return *m_ptr;
}


bool ExprPtr::operator ==(Expr *ptr) const
{
	return (m_ptr == ptr) ? true : false;
}


bool ExprPtr::operator !() const
{
	return m_ptr ? false : true;
}


ExprPtr::operator bool() const
{
	return m_ptr ? true : false;
}


bool ExprPtr::isNull()
{
	return m_ptr ? false : true;
}


///////////////////////////////////////////////////////////////////////////////


Expr *lookup_object(const char *str, Context &ctx)
{
	Scope *scope = ctx.stack().getScope(str);
	if(scope) {
			return new Expr(scope);
	}	
	return NULL;
}


Expr *lookup_variable(const char *str, bool allowXml, Context &ctx)
{
	Expr *obj = lookup_object(str, ctx);
	if(obj) {
		return obj;
	}
	
	Variable *var = ctx.stack().get(str);
	if(var) {
		// TODO: Lazy expansion goes here
		return new Expr((Variable*) var);
	}
	if(allowXml && (var = ctx.stack().getXML(str, ctx))) {
		AutoPtr<Variable> autoVar(var);			// getXML() always returns a new Variable
		return new Expr((Variable*) var);
	}

	return new Expr("");
}


Expr *namelist_expr_or_obj(const char *str, Context &ctx)
{
	Expr *obj = lookup_object(str, ctx);
	if(obj) {
		return obj;
	}
	
	return new Expr(str);
}


/**
 * This is normally used to either replace the value of a simple variable or
 * return the original expression.  As the replacement will be lost, as a side-
 * effect it deletes the expression that is being replaced.
 */
Expr *namelist_variable(Expr *e, Context &ctx)
{
	if(e && e->isSimple() && (e->kind() == KIND_STR)) {
		ConstCharPtr v = e->toString();
		Expr *ret = lookup_variable(v, true, ctx);		// need to allow xml here as object could be an attribute value
		SAFE_DELETE(e);
		return ret;
	}
	return e;
}


/**
 * Returns the result from a built-in function or NULL if not found.
 */
Expr *evaluate_function(const char *str, const ExprList *args, Context &ctx)
{
	int argc = args ? args->size() : 0;

	// Check built-in functions first
	for(DMExprFnTable *f = fn_table; f && f->name; f++) {
		if(strcmp(f->name, str) == 0) {
			if((argc < f->min_argc) || (argc > f->max_argc)) {
				throw RuntimeError(ctx.stack(), "Incorrect number of arguments to function '%s'", str);
			}
			if(!f->fn) {
				throw RuntimeError(ctx.stack(), "Function '%s' not implemented", str);
			}
			Expr *ret;
			if(args) {
				Expr** argv = args->toArgv();
				try {
					ret = (*f->fn)(argc, argv, ctx);
				} catch(...) {
					SAFE_FREE(argv);
					throw;
				}
				SAFE_FREE(argv);
			} else {
				Expr** argv = { NULL };
				ret = (*f->fn)(argc, argv, ctx);
			}
			return ret;
		}
	}

	if(argc < 1) {
		return NULL;
	}

	// Now check object functions - first arg is self
	ListIterator<Expr> iter(*args);
	Expr *self = iter.first();

	//
	// Called from function(object) syntax
	//
	DMObjFnPtr f = self->find_object_function(str, argc-1, ctx);
	if(f) {
		Expr **argv = args->toArgv();
		try {
			Expr *e = (*f)(argc-1, self, (argv+1), ctx);
			e->m_simple = false;
			SAFE_FREE(argv);
			return e;
		} catch(...) {
			SAFE_FREE(argv);
			throw;
		}
	}
	
	return NULL;
}


Expr *concatenate_expr(const Expr &e1, const Expr &e2)
{
	if((e1.kind() == KIND_STR) && (e2.kind() == KIND_STR)) {
		ConstCharPtr s1 = e1.toString(), s2 = e2.toString();
		ConstCharPtr rs = concatenate_string(s1, s2);
		return new Expr((const char*) rs);
	}
	
	return new Expr();
}


char *concatenate_string(const char *str1, const char *str2)
{
	char *ret = (char*) malloc(strlen(str1) + strlen(str2) + 1);
	sprintf(ret, "%s%s", str1, str2);
	return ret;
}


/**
 * This returns one or the other of the expressions and is normmally used to
 * replace the value, so as a side-effect, it deletes the unused expression
 * to prevent leaks.
 */
Expr *check_if_set(Expr *e1, Expr *e2)
{
	ConstCharPtr value = e1->toString();
	if(strlen(value) > 0) {
		SAFE_DELETE(e2);
		return e1;
	}
	SAFE_DELETE(e1);
	return e2;
}


/**
 * This returns one or the other of the expressions and is normmally used to
 * replace the value, so as a side-effect, it deletes the unused expression
 * to prevent leaks.
 */
Expr *check_if_not_set(Expr *e1, Expr *e2)
{
	ConstCharPtr value = e1->toString();
	if(strlen(value) == 0) {
		SAFE_DELETE(e2);
		return e1;
	}
	SAFE_DELETE(e1);
	return e2;
}
