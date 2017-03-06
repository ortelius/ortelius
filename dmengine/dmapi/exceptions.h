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
#ifndef __exceptions_h

#define __exceptions_h


#include "dmapiexport.h"
#include "modelbase.h"


class DMAPI_API DMException : public ILocatable
{
protected:
	int m_fileno;
	int m_lineno;
	int m_fragno;
	char *m_msg;
	char *m_stacktrace;

	DMException(const char *msg);
	DMException(class ScopeStack &stack, const char *msg);
	DMException(class ILocatable &loc, const char *msg);
	DMException(class ILocatable &loc, class ScopeStack &stack, const char *msg);

public:
	DMException(const DMException& copy);
	/*virtual*/ ~DMException();

	virtual const char *getKind()	{ return "DMException"; }

	// ILocatable
	int fileno()  { return m_fileno; }
	int lineno()  { return m_lineno; }
	int fragno()  { return m_fragno; }

	const char *getMessage();
	const char *getStacktrace();
	virtual DMException *clone();
	virtual void print(class IOutputStream &ostr);
};


class DMAPI_API SyntaxError
	: public DMException
{
public:
	SyntaxError(const char *fmt, ...);
	SyntaxError(class ScopeStack &stack, const char *fmt, ...);
	SyntaxError(class ILocatable &loc, const char *fmt, ...);
	SyntaxError(class ILocatable &loc, class ScopeStack &stack, const char *fmt, ...);
	SyntaxError(const SyntaxError& copy);

	const char *getKind()	{ return "SyntaxError"; }
	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API RuntimeError
	: public DMException
{
public:
	RuntimeError(const char *fmt, ...);
	RuntimeError(class ScopeStack &stack, const char *fmt, ...);
	RuntimeError(class ILocatable &loc, class ScopeStack &stack, const char *fmt, ...);
	RuntimeError(const RuntimeError& copy);

	const char *getKind()	{ return "RuntimeError"; }
	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API AssertionFailure
	: public DMException
{
public:
	AssertionFailure(class ILocatable &loc, class ScopeStack &stack, const char *fmt, ...);
	AssertionFailure(const AssertionFailure& copy);

	const char *getKind()	{ return "AssertionFailure"; }
	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API AbortException
	: public DMException
{
public:
	AbortException(
		class ILocatable &loc, class ScopeStack &stack,
		const char *msg, int stream, int exitCode);
	AbortException(const AbortException& copy);

	const char *getKind()	{ return "AbortException"; }
	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API PermissionDeniedException
	: public DMException
{
public:
	PermissionDeniedException(const char *fmt, ...);
	PermissionDeniedException(class ScopeStack &stack, const char *fmt, ...);
	PermissionDeniedException(const PermissionDeniedException& copy);

	const char *getKind()	{ return "PermissionDeniedException"; }
	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API BreakException
	: public DMException
{
public:
	BreakException(class ILocatable &loc, class ScopeStack &stack);
	BreakException(const BreakException& copy);

	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API ContinueException
	: public DMException
{
public:
	ContinueException(class ILocatable &loc, class ScopeStack &stack);
	ContinueException(const ContinueException& copy);

	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API ReturnException
	: public DMException
{
private:
	class Expr *m_expr;

public:
	ReturnException(class ILocatable &loc, class ScopeStack &stack, class Expr *expr);
	ReturnException(const ReturnException& copy);
	~ReturnException();

	class Expr *expr()    { return m_expr; }
	DMException *clone();
	void print(class IOutputStream &ostr);
};


class DMAPI_API NullReferenceException
	: public DMException
{
public:
	NullReferenceException(class ScopeStack &stack);
	NullReferenceException(class ILocatable &loc, class ScopeStack &stack);
	NullReferenceException(const NullReferenceException& copy);

	DMException *clone();
	void print(class IOutputStream &ostr);
};


#endif /*__exceptions_h*/
