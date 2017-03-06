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
#ifndef __thread_h

#define __thread_h


#include "list.h"
#include "hashtable.h"
#include "platform.h"


class Runable
{
public:
	virtual void run() = 0;
};


class Thread
{
protected:
	THREAD_ID m_id;		// handle
#ifndef WIN32
	bool			m_joined;
    	unsigned int		m_threadid;
	bool			m_suspended;
    	pthread_mutex_t		m_SuspendMutex;
   	pthread_cond_t		m_ResumeCond;
#else
	DWORD m_threadid;	// threadid
#endif /*WIN32*/
	static class Lock *s_globallock;
	static Hashtable<class Lock> *s_locks;

public:
	Thread();
	virtual ~Thread();

	THREAD_ID id()  { return m_id; };
	virtual void start();
	void stop();
	void waitFor();
#ifndef WIN32
	int waitForWithTimeout();
	void SuspendThread(void *arg);
#endif /*Linux*/
	
	virtual void run();

	static long currentThreadId();
/*
#ifdef WIN32
*/
	static void lock(int line, const char *filename,const char *lockname = 0);
	static void unlock(int line, const char *filename,const char *lockname = 0);
/*
#else
	static void lock(const char *line, const char *filename,const char *lockname = 0);
	static void unlock(const char *line, const char *filename,const char *lockname = 0);
#endif
*/
};


class DMThread : public virtual Thread
{
private:
	class DMThreadList &m_threadList;
	class DMException *m_ex;

protected:
	int m_exitCode;
	class Context *m_ctx;

public:
	DMThread(class DMThreadList &threadList, class Context *ctx);
	virtual ~DMThread();

	void start();

	class DMException *exception();
	int exitCode();

	void run();

	virtual int execute(class Context &ctx);
};


class DMThreadList : public virtual List<DMThread>
{
private:
	int m_threadLimit;
	List<DMThread> *m_activeThreads;
	List<DMThread> *m_pendingThreads;

	bool threadStart(DMThread *thread);
	DMThread *waitForOne();

public:
	DMThreadList(int threadLimit);
	~DMThreadList();

	int waitForAll();

	friend class DMThread;
};

#endif /*__thread_h*/
