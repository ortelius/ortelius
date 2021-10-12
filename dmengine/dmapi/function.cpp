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

#include <stdio.h>

#include "function.h"

#include "expr.h"
#include "charptr.h"
#include "node.h"
#include "exceptions.h"
#include "cmdline.h"
#include "envp.h"
#include "dropzone.h"
#include "context.h"
#include "execute.h"
#include "dm.h"
#include "lexer.h"
#include "transfer.h"
#include "outputstream.h"
#ifndef WIN32
#define _stricmp strcasecmp
#define stricmp strcasecmp
#endif


extern int yyparse(void *buffer);


///////////////////////////////////////////////////////////////////////////////
// FunctionNodeImpl
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ FunctionNodeImpl::~FunctionNodeImpl()
{}


///////////////////////////////////////////////////////////////////////////////
// FunctionNodeImplFactory
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ FunctionNodeImplFactory::~FunctionNodeImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// FunctionNodeImplRegistry
///////////////////////////////////////////////////////////////////////////////

FunctionNodeImplRegistry *FunctionNodeImplRegistry::s_instance = NULL;


FunctionNodeImplRegistry::FunctionNodeImplRegistry()
	: m_factories(false, true)
{}


/*static*/ FunctionNodeImplRegistry &FunctionNodeImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new FunctionNodeImplRegistry();
	}
	return *s_instance;
}


/*static*/ void FunctionNodeImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void FunctionNodeImplRegistry::registerFactory(const char *name, FunctionNodeImplFactory *factory)
{
	m_factories.put(name, factory);
}


FunctionNodeImplFactory *FunctionNodeImplRegistry::getFactory(const char *name)
{
	return m_factories.get(name);
}


void FunctionNodeImplRegistry::registerBuiltIns()
{
	//registerFactory("lower",       new LowerFunctionImplFactory());
}


///////////////////////////////////////////////////////////////////////////////
// LocalScriptFunctionImpl
///////////////////////////////////////////////////////////////////////////////

LocalScriptFunctionImpl::LocalScriptFunctionImpl(
		FunctionNode &parent, Action &action
	)
	: m_parent(parent), m_action(action)
{}


LocalScriptFunctionImpl::~LocalScriptFunctionImpl()
{}


Expr* LocalScriptFunctionImpl::evaluate(
	ExprList *args, Context &ctx, bool stringify)
{
	const char *rawfilepath = m_action.filepath();

	// Expand variables in the filepath
	Node nfilepath(NODE_STR, strdup(rawfilepath), true);
	ExprPtr efilepath = nfilepath.evaluate(ctx);
	ConstCharPtr filepath = efilepath->stringify();

	if(!filepath) {
		throw RuntimeError(m_parent, ctx.stack(),
			"filepath must be specified");
	}

	const char *cwd = NULL;

	// Has a dropzone been specified?  If so, calculate the path and we will set this as the CWD
	Scope *dzscope = ctx.stack().getScope(DROPZONE_SCOPE);
	Dropzone *dz = dzscope ? (Dropzone*) dzscope->getScopeObject() : NULL;
	if(dz) {
		debug1("using dropzone '%s' - cwd will be '%s'", dz->name(), dz->pathname());
		cwd = dz->pathname();
	}

	CmdLine cmd;
#ifdef WIN32
	if(	STRCASECMP(&filepath[strlen(filepath)-4], ".vbs") == 0 ||
		STRCASECMP(&filepath[strlen(filepath)-4], ".wsf") == 0 ||
		STRCASECMP(&filepath[strlen(filepath)-3], ".js") == 0)  {
		cmd.add("cscript").add("/nologo");
	}
#endif /*WIN32*/
	cmd.add(filepath);

	Envp env(ctx.stack());

	//List<ActionArg> *ordargs = m_action.getArgs();

	//if((args && !ordargs) || (!args && ordargs && (ordargs->size() != 0))) {
	//	throw RuntimeError(m_parent, ctx.stack(),
	//		"Arguments to function '%s' do not match the declaration",
	//		m_action.name());
	//}

	//if(args && ordargs) {
	//	if(args->size() != ordargs->size()) {
	//		throw RuntimeError(m_parent, ctx.stack(),
	//			"Arguments to function '%s' do not match the declaration",
	//			m_action.name());
	//	}

	//	// Everything seems okay, go through the args in order
	//	ListIterator<ActionArg> oit(*ordargs);
	//	ListIterator<Expr> eit(*args);
	//	Expr *e = eit.first();
	//	for(ActionArg *lsca = oit.first(); e && lsca; lsca = oit.next()) {
	//		ConstCharPtr val = e->toString();
	//		if(val) {
	//			// Prefix with optional switch
	//			if(lsca->getSwitch()) {
	//				cmd.add(lsca->getSwitch());
	//			}
	//			cmd.add(val);
	//			env.putTri(lsca->name(), val);
	//		} else {
	//			// Positional parameter was missing, so pad to keep arguments in correct positions
	//			cmd.add("");
	//		}
	//		e = eit.next();
	//	}
	//}

	StringList *params = m_action.mapArgsForFunction(m_parent, args, ctx, &env);
	cmd.add(*params);

	debug3("Running %s...", cmd.toCommandString());

	CapturedData *cd = NULL;
	int tempExitStatus;
	int ret = executeAndCapture(ctx.stream(), cmd, cwd, false, ctx.threadId(), &tempExitStatus, &cd, env.ptr());
	if(ret) {
		ctx.dm().writeToStdOut("localscript '%s' failed to execute", m_action.name());
	}

	Expr *e = NULL;

	if(cd->bytesOfStandardOut() > 0) {
		//ctx.writeBufferToStdOut(cd->standardOut(), cd->bytesOfStandardOut());
		CharPtr temp = (char*) malloc(cd->bytesOfStandardOut() + 1);
		strncpy(temp, cd->standardOut(), cd->bytesOfStandardOut());
		temp[(int)(cd->bytesOfStandardOut())] = '\0';

		if(m_action.resultIsExpr()) {
			// Need to parse the output as an expression
			// e.g. { 'a' => 'b', 'c' => 'd' }
			LexerBuffer lb(expr_lexer, (const char*) temp, NULL);

			yyparse_param param;
			param.buffer = &lb;
			int res = yyparse(&param);
			if((res == 0) && param.expr) {
				e = param.expr->evaluate(ctx, stringify);
				delete param.expr;
			} else {
				if(param.ex) {
					param.ex->print(ctx.dm());
					throw *(param.ex->clone());
				}
			}
		} else {
			e = new Expr((const char*) temp);
		}
	}
	if(cd->bytesOfStandardErr() > 0) {
		ctx.writeBufferToStdErr(ctx.threadId(), cd->standardErr(), cd->bytesOfStandardErr());
	}

	SAFE_DELETE(cd);

	return e ? e : new Expr(KIND_ERROR, "function did not return an expression");
}




///////////////////////////////////////////////////////////////////////////////
// RemoteScriptFunctionImpl
///////////////////////////////////////////////////////////////////////////////

RemoteScriptFunctionImpl::RemoteScriptFunctionImpl(
		FunctionNode &parent, Action &action
	)
	: m_parent(parent), m_action(action)
{}


RemoteScriptFunctionImpl::~RemoteScriptFunctionImpl()
{}


Expr *RemoteScriptFunctionImpl::evaluateOne(
	Server &target, TransferProviderImpl &xfer, ExprList *args,
	Context &ctx, bool stringify)
{
	// Look for a platform-specific script first, then try the default, which may be null
	const char *rawfilepath = target.serverType() ? m_action.filepath(*target.serverType()) : NULL;
	if(!rawfilepath) {
		rawfilepath = m_action.filepath();
	}
	if(!rawfilepath) {
		if(target.serverType()) {
			throw RuntimeError(m_parent, ctx.stack(),
				"No filepath given for remote function '%s' with target system type '%s'",
				m_parent.name(), target.serverType()->hosttype());
		} else {
			throw RuntimeError(m_parent, ctx.stack(),
				"No filepath given for remote function '%s'", m_parent.name());
		}
	}

	// Expand variables in the filepath
	Node nfilepath(NODE_STR, strdup(rawfilepath), true);
	ExprPtr efilepath = nfilepath.evaluate(ctx);
	ConstCharPtr filepath = efilepath->stringify();

	if(!filepath) {
		throw RuntimeError(m_parent, ctx.stack(),
			"filepath must be specified");
	}

	const char *script = filepath;

	StringList *params = m_action.mapArgsForFunction(m_parent, args, ctx);

	debug3("Running %s...", (const char*) script);

	OutputStream output;
	Context newctx(ctx, &output);
	int exitCode = xfer.runScriptOnServer(NULL, script, /*&*/params, m_action.copyToRemote(), m_action.useTTY(), newctx);
	newctx.stack().setGlobal("?", exitCode);

	Expr *e = NULL;

	if(output.size() > 0) {
		// ctx.writeBufferToStdOut(1,output.buffer(), output.size());

		if (m_action.resultIsExpr()) {
			// Result is an expression. If we failed to run the script, just bail out...
			if (exitCode != 0) throw RuntimeError(m_parent, ctx.stack(),output.buffer());
			// If we get here, exitcode was 0. Need to parse the output as an expression
			// e.g. { 'a' => 'b', 'c' => 'd' }
			LexerBuffer lb(expr_lexer, output.buffer(), NULL);

			yyparse_param param;
			param.buffer = &lb;
			int res = yyparse(&param);
			if((res == 0) && param.expr) {
				e = param.expr->evaluate(ctx, stringify);
				delete param.expr;
			} else {
				if(param.ex) {
					param.ex->print(ctx.dm());
					throw *(param.ex->clone());
				}
			}
		} else {
			e = new Expr(output.buffer());
		}
	}

	return e ? e : new Expr(KIND_ERROR, "function did not return an expression");
}

Expr *RemoteScriptFunctionImpl::evaluate(ExprList *args, Context &ctx, bool stringify)
{
	Environment *targetEnv = ctx.dm().getTargetEnvironment();
	if(!targetEnv) {
		throw RuntimeError(m_parent, ctx.stack(),
				"Target environment not set - unable to run remote function");
	}

	Expr *ret = NULL;
	DMArray *arr = NULL;

	ListIterator<Server> iter(ctx.servers());
	for(Server *s = iter.first(); s; s = iter.next()) {
		// each thread gets its own copy of the stack
		ScopeStack *threadStack = new ScopeStack(ctx.stack());

		debug1("Getting Server '%s' credentials", s->name());
		Credentials *creds = s->getCredentials();
		if(!creds) {
			debug1("Getting Environment '%s' credentials", targetEnv->name());
			creds = targetEnv->getCredentials();
		}
		if(!creds) {
			debug1("Getting dialog credentials");
			creds = ctx.dm().getDialogCredentials();
		}
		if (_stricmp(s->hostname(),"localhost")) {
			// Not localhost
			if(!creds) {
				// No credentials for server - abort
				//TODO: throw RuntimeError("No credentials for Server '%s' - unable to deploy", s->name());
				ctx.dm().writeToStdOut("WARNING: No credentials for Server '%s' - unable to execute",s->name());
				debug1("SKIPPING SERVER '%s'", s->name());
				continue;
			} else {
				if(debuglevel() >= 1) {
					printf("DEBUG: Credentials: "); creds->print(0); printf("\n");
				}
			}
		}

		// push the server onto the copied stack (if we are not in a psloop)
		bool popsrv = !threadStack->contains(SERVER_SCOPE);
		if(popsrv) {
			ctx.stack().push(s->getVars());
		}

		// TODO: Should we reduce the server list to just this one server?
		try {
			// Now create a transfer provider for us to talk to the server
			TransferProviderImplFactory *factory = TransferProviderImplRegistry::instance().getFactory(s->protocol());
			if(!factory) {
				throw RuntimeError(m_parent, ctx.stack(),
					"Unable to find transfer provider for protocol '%s'",
					(s->protocol() ? s->protocol() : "(null)"));
			}

			// TODO: Allow component and workdir to be passed in
			TransferProviderImpl *xfer = factory->create(*s, targetEnv, creds, NULL, NULL);	// PAG
			if(!xfer) {
				throw RuntimeError(m_parent, ctx.stack(),
					"Unable to create transfer provider for server '%s'", s->name());
			}
			Expr *res = evaluateOne(*s, *xfer, args, ctx, stringify);

			// If we are doing the server loop, build an array, otherwise just
			// return the one expression
			if(popsrv) {
				if(!ret) {
					arr = new DMArray(false, false, true);
					ret = new Expr(arr);
				}
				if(res) {
					switch(res->kind()) {
					case KIND_STR:
						arr->put(s->name(), new Variable(NULL, res->toString()));
						break;
					case KIND_ARRAY:
						arr->put(s->name(), new Variable(NULL, res->toArray()));
						break;
					default:
						printf("WARNING: Unknown expression type %d when converting result\n", res->kind());
						break;
					}
				}
			} else {
				ret = res;
			}
		} catch(...) {
			if(popsrv) { ctx.stack().pop(SERVER_SCOPE); }
			throw;
		}

		if(popsrv) {
			ctx.stack().pop(SERVER_SCOPE);
		}
	}

	return ret ? ret : new Expr(KIND_ERROR, "function did not return an expression");
}
