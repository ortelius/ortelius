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
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#if defined(__LINUX__) || defined(__linux__)
#include <errno.h>
#endif /*Linux*/
#endif /*WIN32*/

#include <stdio.h>

#include "thread.h"
#include "context.h"
#include "dm.h"
#include "exceptions.h"
#include "autoptr.h"


THREAD_RETURN_TYPE_DECL ThreadStartFunc(void *arg)
{
	debug1("ThreadStartFunc called");
#ifdef WIN32
	((Thread*) arg)->run();
#else
	((Thread*) arg)->SuspendThread(arg);
#endif
	return 0;
}



///////////////////////////////////////////////////////////////////////////////
// Lock
///////////////////////////////////////////////////////////////////////////////

class Lock
{
private:
#ifdef WIN32
	CRITICAL_SECTION *m_lock;
#else
	pthread_mutex_t *m_lock;
#endif /*WIN32*/

public:
	char lockname[80];
	Lock(char *lockname);
	virtual ~Lock();

	void lock();
	void unlock();
};


Lock::Lock(char *lname)
{
	lockname[0]=(char)0;
#ifdef WIN32
	if (lname != NULL)
	   strcpy(lockname,lname);
	m_lock = (CRITICAL_SECTION*) malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(m_lock);
#else
	if (lname != NULL)
	  strcpy(lockname,lname);
	m_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m_lock, NULL);
#endif /*WIN32*/
}


/*virtual*/ Lock::~Lock()
{
	SAFE_FREE(m_lock);
}


void Lock::lock()
{
#ifdef WIN32
	EnterCriticalSection(m_lock);
#else
	pthread_mutex_lock(m_lock);
#endif /*WIN32*/
}


void Lock::unlock()
{
#ifdef WIN32
	LeaveCriticalSection(m_lock);
#else
	pthread_mutex_unlock(m_lock);
#endif /*WIN32*/
}


///////////////////////////////////////////////////////////////////////////////
// Runnable
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Thread
///////////////////////////////////////////////////////////////////////////////

Lock *Thread::s_globallock = NULL;
Hashtable<Lock> *Thread::s_locks = NULL;
// Added access_lock to control access to named locks
#ifdef WIN32
CRITICAL_SECTION *access_lock = NULL;
#else
pthread_mutex_t *access_lock = NULL;
// For Linux Servers, when a thread is first created, it calls this function
// This will immediately suspend the thread. When the thread is "woken", it
// is signalled which will cause if to drop out of pthread_cond_wait and call
// run() in the usual way.
void Thread::SuspendThread(void *arg)
{
	debug1("SuspendThread called m_suspended=%s m_SuspendMutex=0x%lx",m_suspended?"true":"false",&m_SuspendMutex);
	pthread_mutex_lock(&m_SuspendMutex);
	while (m_suspended) {
		// This will unlock m_SuspendMutex and wait on m_ResumeCond
		debug1("Unlocking m_SuspendMutex=0x%lx, m_ResumeCond=0x%lx",&m_SuspendMutex,&m_ResumeCond);
		pthread_cond_wait(&m_ResumeCond, &m_SuspendMutex);
		debug1("pthread_cond_wait returns");
	} 
	debug1("unlocking 0x%lx",&m_SuspendMutex);
	pthread_mutex_unlock(&m_SuspendMutex);
	((Thread *) arg)->run();
	return;
}
#endif

Thread::Thread()
{
#ifdef WIN32
	m_id = CreateThread(NULL, 0, ThreadStartFunc, (LPVOID) this, CREATE_SUSPENDED, &m_threadid);
#else
	m_joined = false;
	m_suspended = true;
	pthread_mutex_init(&m_SuspendMutex, NULL);
	pthread_cond_init(&m_ResumeCond, NULL);
	pthread_create(&m_id, NULL, ThreadStartFunc, this);	// Thread will be started in a suspended state (awaiting signal)
#endif /*WIN32*/
}


/*vritual*/ Thread::~Thread()
{}


void Thread::start()
{
#ifdef WIN32
	debug1("Resuming thread with handle %x (id %x)",m_id,m_threadid);
	DWORD a = ResumeThread(m_id);
	debug1("Done Resuming thread with id %x return %d",m_id,a);
#else
	// pthread_create(&m_id, NULL, ThreadStartFunc, this);
	// Signal the thread to make it call run()
	debug1("Resuming thread - grabbing lock m_SuspendMutex=0x%lx",&m_SuspendMutex);
	pthread_mutex_lock(&m_SuspendMutex);
	m_suspended = false;
	debug1("Sending signal to m_ResumeCond=0x%lx",&m_ResumeCond);
	pthread_cond_signal(&m_ResumeCond);
	debug1("Unlocking m_SuspendMutex=0x%lx",&m_SuspendMutex);
	pthread_mutex_unlock(&m_SuspendMutex);
	debug1("done resuming");
#endif /*WIN32*/
}


void Thread::stop()
{
#ifdef WIN32
	TerminateThread(m_id,0);
#else
	pthread_cancel(m_id);
#endif
}


void Thread::waitFor()
{
#ifdef WIN32
	WaitForSingleObject(m_id, INFINITE);
#else
	if(!m_joined) {
		// if (m_id == 0) start();
		pthread_join(m_id, NULL);
		m_joined = true;
	} else {
		debug1("Thread %p id %x already joined", this, m_id);
	}
#endif /*WIN32*/
}


#if defined(__LINUX__) || defined(__linux__)
int Thread::waitForWithTimeout()
{
	struct timespec ts;
	if(clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		debug1("clock_gettime failed");
		return -1;
	} 
	ts.tv_nsec += 50000;	// 50 milliseconds - 1/20th of a second
	int s = pthread_timedjoin_np(m_id, NULL, &ts);
	switch(s) {
	case 0:
		// Thread has exited within the timeout
		m_joined = true;
		return 0;
	case ETIMEDOUT:
		// Thread still busy after timeout
		return 1;
	default:
		debug1("pthread_timedjoin_np failed - %d", s);
		return -2;
	}
}
#endif /*Linux*/


/*virtual*/ void Thread::run()
{}


/*static*/ long Thread::currentThreadId()
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return (long)pthread_self();
#endif /*WIN32*/
}
/*
#ifdef WIN32
*/
void Thread::lock(int line, const char *filename,const char *lockname)
/*
#else
void Thread::lock(const char *line, const char *filename,const char *lockname)
#endif
*/
{
	//printf("lock on %s\n",lockname?lockname:"global");
	if (!access_lock) {
		// Access lock not yet defined.
#ifdef WIN32
		access_lock = (CRITICAL_SECTION *)malloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(access_lock);
#else
		access_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(access_lock, NULL);
#endif
	}
	//printf("lock(%s)\n",lockname?lockname:"NULL");

	Lock *lock = s_globallock;
	if(lockname) {
#ifdef WIN32
		EnterCriticalSection(access_lock);
#else
		pthread_mutex_lock(access_lock);
#endif /*WIN32*/
		if(!s_locks) {
			// We use this for hostnames which are NOT case-sensitive
			s_locks = new Hashtable<Lock>(true, true);
		}
		lock = s_locks->get(lockname);
		if(!lock) {
			lock = new Lock((char *)lockname);
			s_locks->put(lockname, lock);
		}
#ifdef WIN32
		LeaveCriticalSection(access_lock);
#else
		pthread_mutex_unlock(access_lock);
#endif /*WIN32*/
		debug1("Thread %x locking on '%s'", currentThreadId(), lockname);
	} else if(!lock) {
		s_globallock = lock = new Lock("global");
	}

    if (lock->lockname != NULL)
       debug3("%s:%d Thread %x LOCKING %s\n",filename,line,currentThreadId(),lock->lockname);
	lock->lock();
	//printf("got lock\n");

	//printf("got lock\n");
}
/*
#ifdef WIN32
*/
void Thread::unlock(int line, const char *filename,const char *lockname)
/*
#else
void Thread::unlock(const char *line, const char *filename,const char *lockname)
#endif
*/
{
	// printf("unlock(%s)\n",lockname?lockname:"NULL");
	//printf("unlock on %s\n",lockname?lockname:"global");
	Lock *lock = s_globallock;
	if(lockname) {
		lock = s_locks ? s_locks->get(lockname) : NULL;
		debug1("Thread %x unlocking on '%s'", currentThreadId(), lockname);
	}

	if(lock) {
		if (lock->lockname != NULL)
		   debug3("%s:%d Thread %x UNLOCKING %s\n",filename,line,currentThreadId(),lock->lockname);
		lock->unlock();
	}
	//printf("unlocked\n");
}



///////////////////////////////////////////////////////////////////////////////
// DMThread
///////////////////////////////////////////////////////////////////////////////

DMThread::DMThread(DMThreadList &threadList, class Context *ctx)
	: Thread(), m_threadList(threadList), m_ex(NULL), m_exitCode(0), m_ctx(ctx)
{
	m_threadList.add(this);
}


/*virtual*/ DMThread::~DMThread()
{}


/**
 * Asks the thread list if the thread is allowed to start and if it is, starts
 * the thread.  If the thread was not allowed to start, the thread list will
 * start it for us later.
 */
void DMThread::start()
{
	if(m_threadList.threadStart(this)) {
		Thread::start();
		debug1("Thread %p started with id %x", this, m_id);
	}
}


class DMException *DMThread::exception()
{
	return m_ex;
}


int DMThread::exitCode()
{
	return m_exitCode;
}


void DMThread::run()
{
	debug1("DMThread::run called, m_id=%x",m_id);
	m_ctx->setThreadId((long) m_id);

	// Seed random number generator. Otherwise "random()" will generate
	// the same sequence for each server.
	//
	srand((long)m_id);
	try {
		m_ctx->dm().writeToLogFile("Thread %x running", m_id);
		debug1("About to call execute()");
		m_exitCode = execute(*m_ctx);
		debug1("Execute called m_exitCode=%d",m_exitCode);
		m_ctx->dm().writeToLogFile("Thread %x finished", m_id);
	} catch(DMException &ex) {
		debug3("CAUGHT EXCEPTION in thread %x DMThread::run()",m_id);
		debug1("Thread %x threw exception (%s)", m_id, (ex.getMessage() ? ex.getMessage() : "(null)"));
		m_ctx->dm().writeToLogFile("Thread %x threw exception (%s)", m_id, (ex.getMessage() ? ex.getMessage() : "(null)"));
		m_ex = ex.clone();
		m_exitCode = -1;
		debug3("leaving exception handler");
	} catch(...) {
		debug1("unhandled exception");
		throw;
	}
}


/* virtual */ int DMThread::execute(class Context &ctx)
{
	debug1("******* DMThread::execute ( SHOULD NOT BE HERE ) **************");
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// DMThreadList
///////////////////////////////////////////////////////////////////////////////

DMThreadList::DMThreadList(int threadLimit)
	: List<DMThread>(true), m_threadLimit(threadLimit),
	  m_activeThreads(new List<DMThread>()), m_pendingThreads(NULL)
{}


DMThreadList::~DMThreadList()
{
	SAFE_DELETE(m_activeThreads);
	SAFE_DELETE(m_pendingThreads);
}


/**
 * Records that a thread wants to start.  If we are below the current thread
 * limit, then we record it as an active thread and return true, otherwise we
 * put it in the pending list and waitForAll() will start it when one of the
 * active threads finishes.
 */
bool DMThreadList::threadStart(DMThread *t)
{
	if(m_activeThreads->size() < m_threadLimit) {
		debug1("%p: Thread %p allowed to start - #active was %d",
			this, t, m_activeThreads->size());
		m_activeThreads->add(t);
		return true;
	}

	if(!m_pendingThreads) {
		m_pendingThreads = new List<DMThread>();
	}
	debug1("%p: Thread %p queued - #active was %d, #pending was %d",
		this, t, m_activeThreads->size(), m_pendingThreads->size());
	m_pendingThreads->add(t);
	return false;
}


/**
 * Waits for one of the active threads to finish, removes it from the active
 * list and returns it to the caller.  Windows allows us to wait for multiple
 * threads without blocking and detect when one of them exits.  On Linux we
 * have a version of pthread_join with a timeout which we can use to find
 * threads that have exited.  On other flavours of Unix we use a dumb algorithm
 * that waits for just the first active thead to exit and return that - this
 * will result in some gaps where we could have started a thread, but didn't.
 */
DMThread* DMThreadList::waitForOne()
{
	if(m_activeThreads->size() > 0) {
#ifdef WIN32
		AutoPtr<HANDLE> handles = (HANDLE*) malloc(m_activeThreads->size() * sizeof(HANDLE));
		int count = 0;

		ListIterator<DMThread> iter(*m_activeThreads);
		for(DMThread *t = iter.first(); t; t = iter.next()) {
			debug1("waitForOne adding thread %p id %x", t, t->id());
			handles[count] = t->id();
			count++;
		}

		// Wait indefinitely for one of the list of active threads to exit
		DWORD res = WaitForMultipleObjects(count, handles, FALSE, INFINITE);
		if((res >= WAIT_OBJECT_0) && (res < (WAIT_OBJECT_0 + count))) {
			// Success - find the thread object matching the handle at the given index
			int index = res - WAIT_OBJECT_0;
			HANDLE handle = handles[index];
			for(DMThread *t = iter.first(); t; t = iter.next()) {
				if(t->id() == handle) {
					debug1("%p: Thread %p id %x has exited - found and removed", this, t, t->id());
					m_activeThreads->remove(t);
					return t;
				}
			}

			debug1("%p: Thread id %x has exited - not found in list!", this, (int) handle);
			return NULL;
		}

		debug1("%p: Wait returned %ld", this, res);
		return NULL;
#else
#if defined(__LINUX__) || defined(__linux__)
		// Linux-specific version - use non-blocking join
		ListIterator<DMThread> active(*m_activeThreads);
		while(true) {
			for(DMThread *t = active.first(); t; t = active.next()) {
				int n = t->waitForWithTimeout();
				if(n > 0) {
					// Thread exited within timeout
					debug1("%p: Thread %p id %x has exited - removed", this, t, t->id());
					m_activeThreads->remove(t);
					return t;
				} else if(n < 0) {
					// An error occurred
					return NULL;
				}
			}
		}
#else
		// Dumb version - wait for the first thread to exit and return it
		ListIterator<DMThread> active(*m_activeThreads);
		DMThread *t = active.first();
		debug1("%p: Waiting for thread %p id %x", this, t, t->id());
		t->waitFor();
		debug1("%p: Thread %p id %x has exited - removed", this, t, t->id());
		m_activeThreads->remove(t);
		return t;
#endif /*Linux*/
#endif /*WIN32*/
	}

	return NULL;
}

/**
 * Returns the exit code of any thread with non-zero exit code, or throws
 * an exception if any of the threads raises an exception.
 */
int DMThreadList::waitForAll()
{
	if(m_pendingThreads) {
		// Still some threads to start - start a new thread when an active one finishes
		debug1("%p: Starting pending threads", this);
		for(DMThread *tfinish = waitForOne(); (m_pendingThreads->size() > 0) && tfinish; tfinish = waitForOne()) {
			ListIterator<DMThread> pending(*m_pendingThreads);
			DMThread *tstart = pending.first();
			m_pendingThreads->remove(tstart);
			debug1("%p: Starting pending thread %p - #pending now %d, #active was %d",
				this, tstart, m_pendingThreads->size(), m_activeThreads->size());
			tstart->start();	// This will call us back to add it to the active list
		}

		debug1("%p: All pending threads started - resume normal waitForAll", this);
	}

	// Wait for all threads in the list to finish
	ListIterator<DMThread> iter(*this);
	for(DMThread *t = iter.first(); t; t = iter.next()) {
		debug1("----- %p: Waiting for thread %p id %x", this, t, t->id());
		t->waitFor();
		debug1("----- %p: Done waiting for thread %p id %x: removing from active threads", this, t, t->id());
		m_activeThreads->remove(t);
		if (m_pendingThreads) {
			// Now start any pending thread
			ListIterator<DMThread> pending(*m_pendingThreads);
			DMThread *tstart = pending.first();
			if (tstart) {
				m_pendingThreads->remove(tstart);
				tstart->start();
			}
		}
	}

	debug1("%p: All threads have finished - #active is %d", this, m_activeThreads->size());

	// Now look for any exceptions raised by the threads and rethrow
	DMException *ex = NULL;
	int exitcode = 0;

	for(DMThread *t = iter.first(); t; t = iter.next()) {
		if(t->exitCode() != 0) {
			exitcode = t->exitCode();
		}
		if(t->exception()) {
			if(!ex) {
				ex = t->exception();
			} else {
				// more than one exception raised
			}
		}
	}

	// rethrow the first caught exception
	if(ex) {
		/*ex->rethrow();*/
		throw *ex;
	}

	return exitcode;
}
