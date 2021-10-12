/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
