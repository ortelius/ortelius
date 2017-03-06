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
#include "execute.h"
#include "cmdline.h"
#include "outputstream.h"
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <platform.h>

int Fork()
{
	// Like "fork()" but intercepts failures.
	int res=fork();
	if (res==-1)
	{
		perror("fork");
		exit(1);
	}
	return res;
}


extern "C" void ChildProcessDies(int signal)
{
	//switch(signal) {
	//case SIGCHLD:
	//	break;
	//default:
	//	break;
	//}
}

uid_t name_to_uid(char const *name)
{
	if (!name) return -1;
	long const buflen = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (buflen == -1) return -1;
	char buf[buflen];
	struct passwd pwbuf, *pwbufp;
	if (getpwnam_r(name, &pwbuf, buf, buflen, &pwbufp) != 0 || !pwbufp) return -1;
	return pwbufp->pw_uid;
}


int executeAndCapture(
	IOutputStream &ostr, CmdLine &cmd,
	const char *cwd, bool ShowOutput, long threadId,
	int *ChildExitStatus, CapturedData **cd,
	char **EnvPtr)
{
	int StdOut[2];
	int StdErr[2];
	int StdIn[2];
	int ipc[2];
	int ErrNum=0;
	char ReadBuffer[8192];
	//NOT USED: fd_set	ReadDescriptors;
	//NOT USED: fd_set	WriteDescriptors;
	
	signal(SIGCHLD,ChildProcessDies);

	*cd = new CapturedData();

	pipe(StdOut);	// For capturing the standard out stream of the running process.
	pipe(StdErr);	// For capturing the standard error stream of the running process.
	pipe(StdIn);	// For capturing the standard input stream of the running process.
	pipe(ipc);	// For notifying the parent if the child could not launch the specified program image.

	bool ChildProgramIsRunning=true;

	char **argv = cmd.toArgv();

	int pid=Fork();
	if (pid==0)
	{
		// Child Process - run the program.
		close(ipc[0]);		// Close the "read" side of the pipe
		close(StdOut[0]);	// Close the "read" side of the pipe
		close(StdErr[0]);	// Close the "read" side of the pipe
		close(StdIn[1]);	// Close the "write" side of the pipe.
		close(0);		// Close standard in
		dup(StdIn[0]);		// make standard in the read end of the "stdin" pipe
		close(1);		// Close standard out
		dup(StdOut[1]);		// make standard out the "write" end of the "stdout" pipe
		close(2);		// Close standard err
		dup(StdErr[1]);		// make standard err the "write" end of the "stderr" pipe
		// Change the working directory if one has been specified
		if(cwd) {
			if(chdir(cwd) != 0) {
				fprintf(stderr, "Unable to chdir to '%s'\n", cwd);
				exit(1);
			}
		}
		//
		// Now try and run the program.
		//
		if(EnvPtr) {
			execve(argv[0],argv,EnvPtr);
		} else {
			execv(argv[0],argv);
		}
		ErrNum=errno;
		perror("exec failed");	// ### This lets us see why something failed
		//
		// If we got here the program failed to run. Write the value of "errno" to
		// the IPC pipe to allow our parent to determine why we failed to run.
		//
		write(ipc[1],(void *)&ErrNum,sizeof(ErrNum));
		exit(1);
	} else {
		// Parent Process - set up to read stdin/stdout and pass back to client.
		debug3("Parent Process continues, child pid=%d",pid);
		close(ipc[1]);		// Close the "write" side of the pipe
		close(StdOut[1]);	// Close the "write" side of the pipe
		close(StdErr[1]);	// Close the "write" side of the pipe
		close(StdIn[0]);	// Close the "read" side of the pipe

		close(StdIn[1]);	// Close the "write" side of the pipe as we won't be writing anything - RHT 16/04/2012

		//
		// Use poll to block until something is ready
		// to read.
		//
		struct pollfd fds[3];
		//
		// Standard Output
		//
		fds[0].fd = StdOut[0];
		fds[0].events = POLLIN;
		//
		// Standard Error
		//
		fds[1].fd = StdErr[0];
		fds[1].events = POLLIN;
		//
		// InterProcess Communication
		//
		fds[2].fd = ipc[0];
		fds[2].events = POLLIN;
		//
		// Now loop, waiting for data from the child...
		//
		int Event;
		do
		{
			fds[0].revents = 0;
			fds[1].revents = 0;
			fds[2].revents = 0;
			int BytesRead;
			Event=0;
			debug3("Before poll");
			int s = poll(fds,3,-1);
			debug3("After poll; s=%d, fds[0].revents=%d, fds[1].revents=%d, fds[2].revents=%d", s, fds[0].revents, fds[1].revents, fds[2].revents);
			if (s == -1)
			{
				if (errno!=EINTR)
				{
					// poll has failed in some way (other than being interrupted
					// by SIGCHLD which we expect and can allow for...
					ErrNum=errno;
					debug3("poll failed - ErrNum = %d", ErrNum);
				}
				else
				{
					// RHT 13/04/2012 - EINTR means interupted, no error and try again with continue
					// poll was interrupted by a signal. Was it SIGCHLD? If so,
					// then "ChildProgramIsRunning" will have been cleared down
					// by the signal handler...
					//
					//if (ChildProgramIsRunning==true)
					//{
					//	// Couldn't have been SIGCHLD then...
					//	ErrNum=errno;
					//}
					debug3("poll failed EINTR - ErrNum = %d", ErrNum);
					continue;
				}
			}
			if (fds[0].revents & POLLIN)
			{
				// Something arriving on standard output
				debug3("Data arriving on Standard Output (channel 0)");
				BytesRead=read(StdOut[0],ReadBuffer,sizeof(ReadBuffer));
				debug3("Bytes read = %d",BytesRead);
				if (BytesRead > 0)
				{
					Event=1;
					(*cd)->appendStandardOut(ReadBuffer, BytesRead);
					
					// RHT 10/04/2012 - allow DM to show progress
					if(ShowOutput) {
						ostr.writeBufferToStdOut(threadId, ReadBuffer, BytesRead);
					}
				}
				else if ((BytesRead == -1) && (errno == EINTR))
				{
					debug3("EINTR caught during read");
					continue;
				}
			}
			if (fds[2].revents & POLLIN)
			{
				debug3("Data arriving on IPC Error Channel (channel 2)");
				// Something arriving on the ipc error channel.
				// This can be a genuine message from the child process if the
				// exec failed OR it can be spurious caused by the other end of
				// the pipe closing. Read the data and, if no bytes are read,
				// set ErrNum back to 0. Either way, stop the loop since there'll
				// be no more data to read.
				//
				int p = read(ipc[0],(void *)&ErrNum,sizeof(ErrNum));
				if (!p)
				{
					// Nothing read
					ErrNum=0;
				} else { 
					// Read genuine error code
					Event=1;
					ChildProgramIsRunning=false;
				}
			}
			if (fds[1].revents & POLLIN)
			{
				debug3("Data arriving on Standard Error (channel 1)");
				// Something arriving on standard error
				BytesRead=read(StdErr[0],ReadBuffer,sizeof(ReadBuffer));
				debug3("Bytes read = %d",BytesRead);
				if (BytesRead > 0)
				{
					Event=1;
					(*cd)->appendStandardErr(ReadBuffer, BytesRead);
					
					// RHT 10/04/2012 - allow DM to show progress
					if(ShowOutput) {
						ostr.writeBufferToStdErr(threadId, ReadBuffer, BytesRead);
					}
				}
				else if ((BytesRead == -1) && (errno == EINTR))
				{
					debug3("EINTR caught during read");
					continue;
				}
			}
			if ((fds[0].revents & POLLHUP) && (fds[1].revents & POLLHUP) && (fds[2].revents & POLLHUP)) {
				debug3("All 3 channels reporting POLLHUP");
				Event=0;
			}
			if (!Event && ChildProgramIsRunning)
			{
				debug3("No event read and ChildProgramIsRunning waitforpid(%d)",pid);
				int pid2 = waitpid(pid,ChildExitStatus,WNOHANG);
				debug3("pid2=%d",pid2);
				if (pid2 == pid || pid2 == -1)
				{
					//
					// Child has exited - loop a few more times, to ensure we get
					// all the data
					//
					debug3("pid2 == pid ... getting ready to exit");
					ChildProgramIsRunning=false;
				}
			}
		}
		while (ChildProgramIsRunning);
		debug3("End of loop, ChildProgramIsRunning=false");
	}

	if (WIFEXITED(*ChildExitStatus)) {
		*ChildExitStatus = WEXITSTATUS(*ChildExitStatus);
		debug3("WIFEXITED is true: ChildExitStatus=%d",*ChildExitStatus);
	} else {
		debug3("WIFEXITED is false");
		*ChildExitStatus = 1;
	}

	return ErrNum;
}
