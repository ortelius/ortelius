#include <stdio.h>
#include <stdarg.h>

#include "context.h"
#include "outputstream.h"
#include "dm.h"
#include "model.h"


///////////////////////////////////////////////////////////////////////////////
// Context
///////////////////////////////////////////////////////////////////////////////


Context::Context(DM &dm, List<Server> &servers, ScopeStack &stack)
	: m_dm(dm), m_servers(servers), m_stack(stack),
	  /*m_threads(NULL),*/ m_stream(NULL), m_isRollback(false),
	  m_ownedServers(NULL), m_threadId(0)
{}


/**
 * Specialized copy constructor - preserves dm, stack and stream, but sets new server list
 */
Context::Context(Context &ctx, List<Server> &servers)
	: m_dm(ctx.m_dm), m_servers(servers), m_stack(ctx.m_stack),
	  /*m_threads(ctx.m_threads),*/ m_stream(ctx.m_stream),
	  m_isRollback(ctx.m_isRollback), m_ownedServers(NULL), m_threadId(0)
{}


/**
 * Specialized copy constructor - preserves dm, server list and stream, but sets new stack
 */
Context::Context(Context &ctx, ScopeStack &stack)
	: m_dm(ctx.m_dm), m_servers(ctx.m_servers), m_stack(stack),
	  /*m_threads(ctx.m_threads),*/ m_stream(ctx.m_stream),
	  m_isRollback(ctx.m_isRollback), m_ownedServers(NULL), m_threadId(0)
{}


/**
 * Specialized copy constructor - preserves dm, stack and server list, but sets new stream
 */
Context::Context(Context &ctx, IOutputStream *stream)
	: m_dm(ctx.m_dm), m_servers(ctx.m_servers), m_stack(ctx.m_stack),
	  /*m_threads(ctx.m_threads),*/ m_stream(stream),
	  m_isRollback(ctx.m_isRollback), m_ownedServers(NULL), m_threadId(0)
{}


/**
 * Specialized copy constructor - preserves dm, stack, server list and stream, but sets rollback
 */
Context::Context(Context &ctx, bool rollback)
	: m_dm(ctx.m_dm), m_servers(ctx.m_servers), m_stack(ctx.m_stack),
	  /*m_threads(ctx.m_threads),*/ m_stream(ctx.m_stream),
	  m_isRollback(rollback), m_ownedServers(NULL), m_threadId(0)
{}



Context::~Context()
{
	SAFE_DELETE(m_ownedServers);
}


/**
 * Method to create a new context for a worker thread.  The server list is
 * copied (using the original server objects).  The stack will be the stack
 * that is passed in.  The thread list is nulled so that the thread will own
 * any threads it creates and wait for them to complete.  The stream is
 * currently set to the same object that the parent context is using.
 */
Context *Context::newThreadCopy(class ScopeStack &stack)
{
	//debug1("Context::newThreadCopy called");
	List<Server> *servers = new List<Server>(false);
	ListIterator<Server> iter(m_servers);
	for(Server *s = iter.first(); s; s = iter.next()) {
		servers->add(s);
	}
	Context *ret = new Context(m_dm, *servers, stack);
	ret->m_ownedServers = servers;
	ret->m_stream = m_stream;
	ret->m_isRollback = m_isRollback;	// PAG MOD 03/06/2015 -- added rollback flag
	return ret;
}

Context *Context::newThreadCopy(class ScopeStack &stack,class Server &server)
{
	//debug1("Context::newThreadCopy called");
	List<Server> *servers = new List<Server>(false);
	ListIterator<Server> iter(m_servers);
	for(Server *s = iter.first(); s; s = iter.next()) {
		if (s->id() == server.id()) {
			// bit clunky but avoids all sorts of reference hassles.
			// Note, this ensures only a single server is in context
			servers->add(s);
		}
	}
	Context *ret = new Context(m_dm, *servers, stack);
	ret->m_ownedServers = servers;
	ret->m_stream = m_stream;
	return ret;
}

DM &Context::dm()
{
	return m_dm;
}


List<Server> &Context::servers()
{
	return m_servers;
}


ScopeStack &Context::stack()
{
	return m_stack;
}


bool Context::isRollback()
{
	return m_isRollback;
}


////class DMThreadList *Context::threads()
////{
////	return m_threads;
////}


////void Context::setThreads(class DMThreadList *threads)
////{
////	m_threads = threads;
////}


long Context::threadId()
{
	return m_threadId;
}

void Context::writeVars2File()
{
 m_stack.dump2File(*this);	
}

void Context::setThreadId(long threadId)
{
	m_threadId = threadId;
}


class IOutputStream &Context::stream()
{
	return m_stream ? *m_stream : m_dm;
}

bool Context::inUsingStream()
{
	return m_stream ? true : false;
}


void Context::writeToStdOut(const char *fmt, ...)
{
	char *temp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&temp,fmt,args);
	va_end(args);
	
	writevToStdOut(m_threadId, temp);
	free(temp);
}


void Context::writevToStdOut(long threadId, const char *buffer)
{
	if(m_stream) {
		m_stream->writevToStdOut(threadId, buffer);
	} else {
		m_dm.writevToStdOut(threadId, buffer);
	}
}


void Context::writeBufferToStdOut(long threadId, const char *buffer, int len)
{
	if(m_stream) {
		m_stream->writeBufferToStdOut(threadId, buffer, len);
	} else {
		m_dm.writeBufferToStdOut(threadId, buffer, len);
	}
}


void Context::writeToStdErr(const char *fmt, ...)
{
	char *temp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&temp,fmt,args);
	va_end(args);
	
	if (temp) writevToStdErr(m_threadId, temp);
    free(temp);
}


void Context::writevToStdErr(long threadId, const char *buffer)
{
	if(m_stream) {
		m_stream->writevToStdErr(threadId, buffer);
	} else {
		m_dm.writevToStdErr(threadId, buffer);
	}
}


void Context::writeBufferToStdErr(long threadId, const char *buffer, int len)
{
	if(m_stream) {
		m_stream->writeBufferToStdErr(threadId, buffer, len);
	} else {
		m_dm.writeBufferToStdErr(threadId, buffer, len);
	}
}
