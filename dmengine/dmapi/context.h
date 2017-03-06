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
#ifndef __context_h

#define __context_h


#include "list.h"
#include "outputstream.h"
#include "dmapiexport.h"


/**
 * Execution context - passed around with the current state and references to
 * all the core objects that we need.
 * dm      - reference to DM so we can look up new objects etc.
 * servers - the current set of servers - can be narrowed by an if and looped with psloop
 * stack   - the current stack - either global stack or thread stack
 * stream  - the current stream - either DM or the active stream
 * rollup/rollback - flags to indicate whether we should filter components/component items
 */
class DMAPI_API Context : public virtual IOutputStream
{
private:
	class DM &m_dm;
	List<class Server> &m_servers;
	class ScopeStack &m_stack;
	////class DMThreadList *m_threads;
	class IOutputStream *m_stream;
	bool m_isRollback;
	List<class Server> *m_ownedServers;
	long m_threadId;

public:
	// Constructor
	Context(class DM &dm, List<class Server> &servers, class ScopeStack &stack);

	// Specialized copy constructors
	Context(class Context &ctx, List<class Server> &servers);
	Context(class Context &ctx, class ScopeStack &stack);
	Context(class Context &ctx, class IOutputStream *stream);
	Context(class Context &ctx, bool rollback);

	virtual ~Context();

	// Deep copy for new threads that makes and owns private server list
	Context *newThreadCopy(class ScopeStack &stack);
	Context *newThreadCopy(class ScopeStack &stack,class Server &server);

	class DM &dm();
	List<class Server> &servers();
	class ScopeStack &stack();
	bool isRollback();

	////class DMThreadList *threads();
	////void setThreads(class DMThreadList *threads);

	long threadId();
	void setThreadId(long threadId);

	class IOutputStream &stream();
	bool inUsingStream();

	void writeToStdOut(const char *fmt, ...);
	void writevToStdOut(long threadId, const char *buffer);
	void writeBufferToStdOut(long threadId, const char *buffer, int len);

	void writeToStdErr(const char *fmt, ...);
	void writevToStdErr(long threadId, const char *buffer);
	void writeBufferToStdErr(long threadId, const char *buffer, int len);
};


#endif /*__context_h*/
