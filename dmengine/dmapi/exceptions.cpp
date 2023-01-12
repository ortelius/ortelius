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

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "exceptions.h"
#include "outputstream.h"
#include "platform.h"
#include "expr.h"
#include "filenamecache.h"
#include "scopestack.h"


///////////////////////////////////////////////////////////////////////////////
// DMException
///////////////////////////////////////////////////////////////////////////////

DMException::DMException(const char *msg)
	: m_fileno(0), m_lineno(0), m_fragno(0),
	  m_msg(DUP_NULL(msg)), m_stacktrace(NULL)
{}


DMException::DMException(ScopeStack &stack, const char *msg)
	: m_fileno(0), m_lineno(0), m_fragno(0),
	  m_msg(DUP_NULL(msg)), m_stacktrace(NULL)
{
	OutputStream temp;
	stack.trace(temp, *this);
	debug3("stacktrace = '%s'", temp.buffer());
	m_stacktrace = strdup(temp.buffer());
}


DMException::DMException(ILocatable &loc, const char *msg)
	: m_fileno(loc.fileno()), m_lineno(loc.lineno()), m_fragno(loc.fragno()),
	  m_msg(DUP_NULL(msg)), m_stacktrace(NULL)
{}


DMException::DMException(ILocatable &loc, ScopeStack &stack, const char *msg)
	: m_fileno(loc.fileno()), m_lineno(loc.lineno()), m_fragno(loc.fragno()),
	  m_msg(DUP_NULL(msg)), m_stacktrace(NULL)
{
	OutputStream temp;
	stack.trace(temp, *this);
	debug3("stacktrace = '%s'", temp.buffer());
	m_stacktrace = strdup(temp.buffer());
}


DMException::DMException(const DMException& copy)
	: m_fileno(copy.m_fileno), m_lineno(copy.m_lineno),
	  m_fragno(copy.m_fragno), m_msg(DUP_NULL(copy.m_msg)),
	  m_stacktrace(DUP_NULL(copy.m_stacktrace))
{}


DMException::~DMException()
{
	SAFE_FREE(m_msg);
	SAFE_FREE(m_stacktrace);
}


const char *DMException::getMessage()
{
	return m_msg;
}

const char *DMException::getStacktrace()
{
	return m_stacktrace;
}


/*virtual*/ DMException *DMException::clone()
{
	return new DMException(*this, m_msg);
}


/**
 * This will be used if an exception class does not override the print()
 * method.  It should not normally be called.
 */
/*virtual*/ void DMException::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		ostr.writeToStdErr("Exception at line %d of \"%s\": %s", m_lineno, filename, (m_msg ? m_msg : "(null)"));
	} else {
		ostr.writeToStdErr("Exception at line %d: %s", m_lineno, (m_msg ? m_msg : "(null)"));
	}
}


///////////////////////////////////////////////////////////////////////////////
// SyntaxError
///////////////////////////////////////////////////////////////////////////////

SyntaxError::SyntaxError(const char *fmt, ...)
	: DMException(NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


SyntaxError::SyntaxError(class ScopeStack &stack, const char *fmt, ...)
	: DMException(stack, NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


SyntaxError::SyntaxError(ILocatable &loc, const char *fmt, ...)
	: DMException(loc, NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


SyntaxError::SyntaxError(
		ILocatable &loc, class ScopeStack &stack, const char *fmt, ...
	)
	: DMException(loc, stack, NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


SyntaxError::SyntaxError(const SyntaxError& copy)
	: DMException(copy)
{}


DMException *SyntaxError::clone()
{
	return new SyntaxError(*this);
}


void SyntaxError::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		ostr.writeToStdErr("Syntax error at line %d of \"%s\": %s", m_lineno,  filename, (m_msg ? m_msg : "(null)"));
	} else {
		ostr.writeToStdErr("Syntax error at line %d: %s", m_lineno, (m_msg ? m_msg : "(null)"));
	}
}


///////////////////////////////////////////////////////////////////////////////
// RuntimeError
///////////////////////////////////////////////////////////////////////////////

RuntimeError::RuntimeError(const char *fmt, ...)
	: DMException(NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


RuntimeError::RuntimeError(class ScopeStack &stack, const char *fmt, ...)
	: DMException(stack, NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


RuntimeError::RuntimeError(
		ILocatable &loc, class ScopeStack &stack, const char *fmt, ...
	)
	: DMException(loc, stack, NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


RuntimeError::RuntimeError(const RuntimeError& copy)
	: DMException(copy)
{}


DMException *RuntimeError::clone()
{
	return new RuntimeError(*this);
}


void RuntimeError::print(IOutputStream &ostr)
{
	if(m_lineno) {
		ostr.writeToStdErr("Runtime error at line %d: %s\n", m_lineno, (m_msg ? m_msg : "(null)"));
	} else {
		ostr.writeToStdErr("Runtime error: %s", (m_msg ? m_msg : "(null)"));
	}
}


///////////////////////////////////////////////////////////////////////////////
// AssertionFailure
///////////////////////////////////////////////////////////////////////////////

AssertionFailure::AssertionFailure(
		ILocatable &loc, class ScopeStack &stack, const char *fmt, ...
	)
	: DMException(loc, stack, NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


AssertionFailure::AssertionFailure(const AssertionFailure& copy)
	: DMException(copy)
{}


DMException *AssertionFailure::clone()
{
	return new AssertionFailure(*this);
}


void AssertionFailure::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		ostr.writeToStdErr("**** Assertion failure at line %d of \"%s\": %s", m_lineno, filename, (m_msg ? m_msg : "(null)"));
	} else {
		ostr.writeToStdErr("**** Assertion failure at line %d: %s", m_lineno, (m_msg ? m_msg : "(null)"));
	}
}


///////////////////////////////////////////////////////////////////////////////
// AbortException
///////////////////////////////////////////////////////////////////////////////

AbortException::AbortException(
		ILocatable &loc, class ScopeStack &stack,
		const char *msg, int stream, int exitCode
	)
	: DMException(loc, stack, msg)
{}


AbortException::AbortException(const AbortException& copy)
	: DMException(copy)
{
	// TODO: copy stream and exitCode
}


DMException *AbortException::clone()
{
	return new AbortException(*this);
}


void AbortException::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		if(m_msg) {
			ostr.writeToStdErr("Aborted at line %d of \"%s\": %s", m_lineno, filename, m_msg);
		} else {
			ostr.writeToStdErr("Aborted at line %d of \"%s\"", m_lineno, filename);
		}
	} else {
		if(m_msg) {
			ostr.writeToStdErr("Aborted at line %d: %s", m_lineno, m_msg);
		} else {
			ostr.writeToStdErr("Aborted at line %d", m_lineno);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// PermissionDeniedException
///////////////////////////////////////////////////////////////////////////////

PermissionDeniedException::PermissionDeniedException(const char *fmt, ...)
	: DMException(NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}

PermissionDeniedException::PermissionDeniedException(
		class ScopeStack &stack, const char *fmt, ...
	)
	: DMException(stack, NULL)
{
	va_list args;
	va_start(args, fmt);
	if (m_msg != NULL)
	 free(m_msg);
	vasprintf(&m_msg,fmt, args);
	va_end(args);
}


PermissionDeniedException::PermissionDeniedException(const PermissionDeniedException& copy)
	: DMException(copy)
{}


DMException *PermissionDeniedException::clone()
{
	return new PermissionDeniedException(*this);
}


void PermissionDeniedException::print(IOutputStream &ostr)
{
	ostr.writeToStdErr("Permission denied: %s", (m_msg ? m_msg : "(null)"));
}


///////////////////////////////////////////////////////////////////////////////
// BreakException
///////////////////////////////////////////////////////////////////////////////

BreakException::BreakException(ILocatable &loc, class ScopeStack &stack)
	: DMException(loc, stack, NULL)
{}


BreakException::BreakException(const BreakException& copy)
	: DMException(copy)
{}


DMException *BreakException::clone()
{
	return new BreakException(*this);
}


void BreakException::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		ostr.writeToStdErr("break outside for/foreach/switch/while at line %d of \"%s\"", m_lineno, filename);
	} else {
		ostr.writeToStdErr("break outside for/foreach/switch/while at line %d", m_lineno);
	}
}


///////////////////////////////////////////////////////////////////////////////
// ContinueException
///////////////////////////////////////////////////////////////////////////////

ContinueException::ContinueException(ILocatable &loc, class ScopeStack &stack)
	: DMException(loc, stack, NULL)
{}


ContinueException::ContinueException(const ContinueException& copy)
	: DMException(copy)
{}


DMException *ContinueException::clone()
{
	return new ContinueException(*this);
}


void ContinueException::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		ostr.writeToStdErr("continue outside for/foreach/while at line %d of \"%s\"", m_lineno, filename);
	} else {
		ostr.writeToStdErr("continue outside for/foreach/while at line %d", m_lineno);
	}
}


///////////////////////////////////////////////////////////////////////////////
// ReturnException
///////////////////////////////////////////////////////////////////////////////

ReturnException::ReturnException(
		ILocatable &loc, class ScopeStack &stack, Expr *expr
	)
	: DMException(loc, stack, NULL), m_expr(expr)
{}


ReturnException::ReturnException(const ReturnException& copy)
	: DMException(copy), m_expr(copy.m_expr ? new Expr(*copy.m_expr) : NULL)
{}


DMException *ReturnException::clone()
{
	return new ReturnException(*this);
}


ReturnException::~ReturnException()
{
	SAFE_DELETE(m_expr);
}


void ReturnException::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		ostr.writeToStdErr("unhandled return at line %d of \"%s\"", m_lineno, filename);
	} else {
		ostr.writeToStdErr("unhandled return at line %d", m_lineno);
	}
}


///////////////////////////////////////////////////////////////////////////////
// NullReferenceException
///////////////////////////////////////////////////////////////////////////////

NullReferenceException::NullReferenceException(class ScopeStack &stack)
	: DMException(stack, strdup("null reference"))
{}


NullReferenceException::NullReferenceException(ILocatable &loc, class ScopeStack &stack)
	: DMException(loc, stack, strdup("null reference"))
{}


NullReferenceException::NullReferenceException(const NullReferenceException& copy)
	: DMException(copy)
{}


DMException *NullReferenceException::clone()
{
	return new NullReferenceException(*this);
}


void NullReferenceException::print(IOutputStream &ostr)
{
	const char *filename = (m_fileno ? FilenameCache::getFilename(m_fileno) : NULL);
	if(filename) {
		ostr.writeToStdErr("null reference at line %d of \"%s\"", m_lineno, filename);
	} else {
		ostr.writeToStdErr("null reference at line %d", m_lineno);
	}
}
