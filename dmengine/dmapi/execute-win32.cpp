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
#include <windows.h>
#include <process.h>
#include <io.h>
#include <stdio.h>

#include "execute.h"
#include "cmdline.h"
#include "outputstream.h"
#include "platform.h"

typedef unsigned long uint32_t;
#define CLIENT_COMMS_FAILURE 10000

#if 0
#define TRIDEBUG(x) printf ##x; printf("\n")
#else
#define TRIDEBUG(x) 
#endif
#define TRIERROR(x) printf ##x; printf("\n")


typedef struct _StdThreadInfo
{
	CapturedData	*cd;
	HANDLE			Handle;
	int				Client;
	bool			ShowOutput;	// RHT 02/04/2012 - flag for DM
	IOutputStream	*ostr;		// RHT 02/04/2012 - for logging
	long			threadId;	// RHT 08/08/2013 - for logging
} StdThreadInfo;


int ReadAll(int Handle,void *b,long BytesToRead)
{
	long BytesRead;
	long n=0;
	char *Buffer=(char *)b;
	while (BytesToRead)
	{
		BytesRead = recv(Handle,&(Buffer[n]),BytesToRead,0);
		if (BytesRead <= 0)
		{
			TRIDEBUG(("BytesRead = %d",BytesRead));
			break;
		}
		n=n+BytesRead;
		BytesToRead=BytesToRead-BytesRead;
	}
	return (BytesRead <= 0)?-1:0;	// 0 if success, -1 means we bitched out
}


unsigned int WINAPI StdInThread(PVOID arg)
{
	TRIDEBUG(("StdInThread - Enter"));
	StdThreadInfo* info=(StdThreadInfo*)arg;
	DWORD bytesWritten;
	DWORD writeError;
	BOOL writeResult;
	int ErrorCondition=0;

	CapturedData* cd = info->cd;
	HANDLE handle=info->Handle;
	int client=info->Client;

	if(cd)
	{
		/*while(cd->bytesOfStandardIn() > 0)
		{
			TRIDEBUG(("StdInThread - Starting write ..."));

			writeResult=WriteFile(handle,
				cd->StandardIn,
				cd->BytesOfStandardIn,
				&bytesWritten,
				NULL);

			if(!writeResult) {
				writeError=GetLastError();
				TRIDEBUG(("StdInThread - Write failed, error=%ld",writeError));
				if(writeError==ERROR_BROKEN_PIPE) {
					TRIDEBUG(("StdInThread - Pipe closed"));
					break;
				}
			} else {
				TRIDEBUG(("StdInThread - Write suceeded"));
				TRIDEBUG(("StdInThread - bytesWritten=%ld",bytesWritten));
				cd->BytesOfStandardIn=cd->BytesOfStandardIn - bytesWritten;
			}
		}*/

TRIDEBUG(("StdInThread - Closing handle"));
		CloseHandle(handle);	// RHT 30/03/2012 - was missing
	}
	else
	{
		uint32_t br,BytesOfInput;
		TRIDEBUG(("Calling \"ReadAll\" to read data from client"));
		if (ReadAll(client,(char *)&br,sizeof(br)) == 0)
		{
			TRIDEBUG(("br[0]=0x%02x",((char *)&br)[0]));
			TRIDEBUG(("br[1]=0x%02x",((char *)&br)[1]));
			TRIDEBUG(("br[2]=0x%02x",((char *)&br)[2]));
			TRIDEBUG(("br[3]=0x%02x",((char *)&br)[3]));
			BytesOfInput=ntohl(br);
			TRIDEBUG(("Bytes of Input=%d\n",BytesOfInput));
		}
		else
		{
			// Error of some sort
			TRIDEBUG(("Setting BytesOfInput to 0"));
			BytesOfInput = 0;
			ErrorCondition = 1;
		}
		while (BytesOfInput>0)
		{
			// Something to write to standard input
			TRIDEBUG(("malloc(%d)",BytesOfInput));
			char *buffer=(char *)malloc(BytesOfInput);
			TRIDEBUG(("About to read %d Bytes",BytesOfInput));
			if (ReadAll(client,buffer,BytesOfInput) == 0)
			{
				//
				// Successfully read all the data
				writeResult=WriteFile(handle,
					buffer,
					BytesOfInput,
					&bytesWritten,
					NULL);
				TRIDEBUG(("writeResult=%d",writeResult));

				if(!writeResult)
				{
					writeError=GetLastError();
					TRIDEBUG(("StdInThread - Write failed, error=%ld",writeError));
					if(writeError==ERROR_BROKEN_PIPE) 
					{
						TRIDEBUG(("StdInThread - Pipe closed"));
						ErrorCondition=1;
						break;
					}
				}
				if (ReadAll(client,(char *)&br,sizeof(br)) == 0)
				{
					TRIDEBUG(("br[0]=0x%02x",((char *)&br)[0]));
					TRIDEBUG(("br[1]=0x%02x",((char *)&br)[1]));
					TRIDEBUG(("br[2]=0x%02x",((char *)&br)[2]));
					TRIDEBUG(("br[3]=0x%02x",((char *)&br)[3]));
					BytesOfInput=ntohl(br);
				}
				else
				{
					BytesOfInput = 0;	// error
					ErrorCondition = 1;
				}
			}
			else
			{
				// error in reading the standard input stream - just exit
				BytesOfInput = 0;
				ErrorCondition = 1;
			}
			free(buffer);
		}
		
		// end of standard input
		if (BytesOfInput != 0)
		{
			// Something has gone wrong
			ErrorCondition = 1;
		}
TRIDEBUG(("StdInThread - Closing handle"));
		CloseHandle(handle);
	}

	TRIDEBUG(("StdInThread - Exit"));
	return ErrorCondition;
}


int SendAll(int Handle,void *b,long BytesToWrite,int flags)
{
	long BytesWritten;
	long n=0;
	int res=0;
	char *Buffer=(char *)b;
	while (BytesToWrite)
	{
		BytesWritten = send(Handle,&(Buffer[n]),BytesToWrite,flags);
		if (BytesWritten < 0)
		{
			BytesToWrite=-1;
			break;
		}
		n=n+BytesWritten;
		BytesToWrite=BytesToWrite-BytesWritten;
	}
	return BytesToWrite;
}


unsigned int WINAPI StdOutThread(PVOID arg)
{
	// printf("StdOutThread - Enter\n");
	StdThreadInfo* info = (StdThreadInfo*) arg;
	DWORD readError;

	int ErrorCondition=0;
	
	CapturedData* cd = info->cd;
	HANDLE handle=info->Handle;
	int client=info->Client;

	if(cd)
	{
		char *bufPtr = (char*) malloc(STD_BUFFER_SIZE_INCREMENT);
		DWORD bytesRead = 0;

		TRIDEBUG(("StdOutThread - Starting read ..."));

		while(ReadFile(handle, bufPtr, STD_BUFFER_SIZE_INCREMENT, &bytesRead, NULL))
		{
			TRIDEBUG(("StdOutThread - Read data, bytesRead=%ld", bytesRead));

			DWORD readError = GetLastError();

			TRIDEBUG(("StdOutThread - Get last error=%ld", readError));

			if(bytesRead == 0) {
				printf("StdOutThread - Read zero bytes, EOF\n");
				break;
			}

			// RHT 02/04/2012 - allow DM to show progress
			if(info->ostr && info->ShowOutput) {
				debug1("STDOUT: %d bytes", bytesRead);
				info->ostr->writeBufferToStdOut(info->threadId, bufPtr, bytesRead);
			}

			// Add to the capture buffer
			cd->appendStandardOut(bufPtr, bytesRead);

			TRIDEBUG(("StdOutThread - Going round loop again"));
		}

		free(bufPtr);
	}
	else
	{
		char ReadBuffer[1024];
		//
		// Num Bytes and stream types for comms back to client
		//
		uint32_t nb,*NumBytes,*StreamType;
		NumBytes=(uint32_t *)&(ReadBuffer[0]);
		StreamType=(uint32_t *)&(ReadBuffer[4]);
		while(ReadFile(handle,&(ReadBuffer[8]),sizeof(ReadBuffer)-8,&nb,NULL))
		{
			*StreamType = htonl((uint32_t)0);
			*NumBytes = htonl((uint32_t)nb);
			TRIDEBUG(("*NumBytes=%d\n",*NumBytes));
			TRIDEBUG(("*StreamType=%d\n",*StreamType));
			if (getenv("TRITRACE"))
			{
				printf("NumBytes=%d\n",nb);
			}
			if (SendAll(client,ReadBuffer,(nb)+8,0) == -1)
			{
				TRIDEBUG(("StdOutThread - Setting ErrorCondition and exiting"));
				ErrorCondition = 1;
				break;
			}
		}
	}


	readError=GetLastError();
	// printf("StdOutThread - Reading finished, last error=%ld\n", readError);

	// printf("StdOutThread - Exit\n");
	return ErrorCondition;
}


unsigned int WINAPI StdErrThread(PVOID arg)
{
	TRIDEBUG(("StdErrThread - Enter"));
	StdThreadInfo* info=(StdThreadInfo*)arg;
	//DWORD bytesRead;
	//DWORD bytesToRead;
	//DWORD bytesInBuffer;
	DWORD readError;
	//DWORD tmpSize;
	//char* tmpPtr;
	//char* bufPtr;

	int ErrorCondition=0;
	
	CapturedData* cd = info->cd;
	HANDLE handle=info->Handle;
	int client=info->Client;

	if (cd)
	{
		char *bufPtr = (char*) malloc(STD_BUFFER_SIZE_INCREMENT);
		DWORD bytesRead = 0;

		TRIDEBUG(("StdErrThread - Starting read ..."));

		while(ReadFile(handle, bufPtr, STD_BUFFER_SIZE_INCREMENT, &bytesRead, NULL))
		{
			TRIDEBUG(("StdErrThread - Read data, bytesRead=%ld",bytesRead));

			readError = GetLastError();

			TRIDEBUG(("StdErrThread - Get last error=%ld",readError));

			if(bytesRead == 0) {
				TRIDEBUG(("StdErrThread - Read zero bytes, EOF"));
				break;
			}

			// RHT 02/04/2012 - allow DM to show progress
			if(info->ostr && info->ShowOutput) {
				//debug1("STDERR: %d bytes", bytesRead);
				info->ostr->writeBufferToStdErr(info->threadId, bufPtr, bytesRead);
			}

			// Add to the capture buffer
			cd->appendStandardErr(bufPtr, bytesRead);

			TRIDEBUG(("StdErrThread - Going round loop again"));
		}

		free(bufPtr);
	}
	else
	{
		char ReadBuffer[1024];
		//
		// Num Bytes and stream types for comms back to client
		//
		uint32_t nb,*NumBytes,*StreamType;
		NumBytes=(uint32_t *)&(ReadBuffer[0]);
		StreamType=(uint32_t *)&(ReadBuffer[4]);
		while(ReadFile(handle,&(ReadBuffer[8]),sizeof(ReadBuffer)-8,&nb,NULL))
		{
			*NumBytes = htonl((uint32_t)nb);
			*StreamType = htonl((uint32_t)1);
			if (SendAll(client,ReadBuffer,(nb)+8,0) == -1)
			{
				TRIDEBUG(("StdErrThread - Setting ErrorCondition and exiting"));
				ErrorCondition = 1;
				break;
			}
		}
	}

	readError=GetLastError();
	TRIDEBUG(("StdErrThread - Reading finished, last error=%ld",readError));

	TRIDEBUG(("StdErrThread - Exit"));

	return ErrorCondition;
}


int executeAndCapture(
	IOutputStream &ostr, CmdLine &cmd, const char *cwd,
	bool ShowOutput, long threadId, int *ChildExitStatus,
	CapturedData **cd, char **EnvPtr)
{
	HANDLE StdOut[2];
	HANDLE StdErr[2];
	HANDLE StdIn[2];

	HANDLE ChildStdIn;
	HANDLE ChildStdOut;
	HANDLE ChildStdErr;

	unsigned long InThread;
	unsigned long OutThread;
	unsigned long ErrThread;

	SECURITY_ATTRIBUTES saErr;
	SECURITY_ATTRIBUTES saOut;
	SECURITY_ATTRIBUTES saIn;
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	int ErrNum=0;

	
	TRIDEBUG(("ExecuteAndCapture - Enter"));

	// Create new capture object 
	*cd = new CapturedData();

	saIn.nLength=sizeof(SECURITY_ATTRIBUTES);
	saIn.lpSecurityDescriptor=NULL;
	saIn.bInheritHandle=TRUE;

	TRIDEBUG(("Execute - Creating StdIn Pipe ..."));

	if(!CreatePipe(&StdIn[0],&StdIn[1],&saIn,0)) 
	{
		TRIERROR(("Execute - Couldn't create StdIn pipe"));
		return -1;
	}

	TRIDEBUG(("Execute - Duplicating StdIn Handle ..."));

	if(!DuplicateHandle(
		GetCurrentProcess(),
		StdIn[1],
		GetCurrentProcess(),
		&ChildStdIn,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS)) 
	{
		TRIERROR(("Execute - Can't duplicate StdIn write handle!"));
		return -1;
	}

	// Close the write end of StdIn since we don't want to 
	// pass this to the child. 
	CloseHandle(StdIn[1]);

	saOut.nLength=sizeof(SECURITY_ATTRIBUTES);
	saOut.lpSecurityDescriptor=NULL;
	saOut.bInheritHandle=TRUE;

	TRIDEBUG(("Execute - Creating StdOut Pipe ..."));

	if(!CreatePipe(&StdOut[0],&StdOut[1],&saOut,0)) 
	{
		TRIERROR(("Execute - Couldn't create StdOut pipe"));
		return -1;
	}

	TRIDEBUG(("Execute - Duplicating StdOut Handle ..."));

	if(!DuplicateHandle(
		GetCurrentProcess(),
		StdOut[0],
		GetCurrentProcess(),
		&ChildStdOut,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS)) 
	{
		TRIERROR(("Execute - Can't duplicate StdOut read handle!"));
		return -1;
	}

	// This shouldn't be inherited as the child shouldn't read from 
	// standard OUT
	CloseHandle(StdOut[0]);

	saErr.nLength=sizeof(SECURITY_ATTRIBUTES);
	saErr.lpSecurityDescriptor=NULL;
	saErr.bInheritHandle=TRUE;

	TRIDEBUG(("Execute - Creating StdErr Pipe ..."));

	if(!CreatePipe(&StdErr[0],&StdErr[1],&saErr,0)) 
	{
		TRIERROR(("Execute - Couldn't create StdErr pipe"));
		return -1;
	}

	TRIDEBUG(("Execute - Duplicating StdErr Handle ..."));

	if(!DuplicateHandle(
		GetCurrentProcess(),
		StdErr[0],
		GetCurrentProcess(),
		&ChildStdErr,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS)) 
	{
		TRIERROR(("Execute - Can't duplicate StdErr read handle!"));
		return -1;
	}

	// This shouldn't be inherited as the child shouldn't read from 
	// standard Err
	CloseHandle(StdErr[0]);


	GetStartupInfoA(&si);
	si.dwFlags=STARTF_USESTDHANDLES;
	si.hStdOutput=StdOut[1]; // Write end of pipe for child StdOut
	si.hStdError=StdErr[1]; // Write end of pipe for child StdErr
	si.hStdInput=StdIn[0];  // Read end of pipe for child StdIn
	
	TRIDEBUG(("Execute - Running process ..."));

	// Transform EnvPtr's array of discrete strings into a single block
	char *ep = NULL;
	if(EnvPtr) {
		int size = 0;
		char **e;
		for(e = EnvPtr; e && *e; e++) {
			size += strlen(*e) + 1;
		}
		size++;

		ep = (char*) malloc(size);
		char *x = ep;
		for(e = EnvPtr; e && *e; e++) {
			strcpy(x, *e);
			x += strlen(*e) + 1;
		}
		*x = '\0';
	}

	// This creates a flat string with quoted args and extra quotes for Windows
	char *CommandLine = cmd.toCommandString();

	BOOL createRet = CreateProcessA(NULL, 	// applicaion name
      CommandLine,				// command line 
      NULL,					// process security attributes 
      NULL,					// primary thread security attributes 
      TRUE,					// handles are inherited 
      0,					// creation flags 
      ep,					// use passed environment 
      cwd,					// cwd or use parent's current directory if NULL
      &si,					// STARTUPINFO pointer 
      &pi);					// receives PROCESS_INFORMATION 

	if(ep) {
		free(ep);
		ep = NULL;
	}

	if(!createRet) 
	{
		int res=GetLastError();
#if defined(WIN32) && defined(_DEBUG)
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		debug1("CreateProcessA failed: %s", lpMsgBuf);
		LocalFree(lpMsgBuf);
#endif /*WIN32 && _DEBUG*/
		printf("Execute - Couldn't create process, %s\n", CommandLine);
		free(CommandLine);
		return res;
	} else {
		// printf("Execute - Created process sucessfully\n");
	}

	free(CommandLine);

	// Close process thread handle and other file handles to make
	// sure everything gets tidied up properly.
	CloseHandle(pi.hThread);
	CloseHandle(StdOut[1]);
	CloseHandle(StdErr[1]);
	CloseHandle(StdIn[0]);

	StdThreadInfo inThreadInfo;
	StdThreadInfo outThreadInfo;
	StdThreadInfo errThreadInfo;
	DWORD ThreadError;

	inThreadInfo.cd=*cd;
	outThreadInfo.cd=*cd;
	errThreadInfo.cd=*cd;

	inThreadInfo.Handle=ChildStdIn;
	outThreadInfo.Handle=ChildStdOut;
	errThreadInfo.Handle=ChildStdErr;

	// RHT 02/04/2012 - pass through ShowOutput flag
	inThreadInfo.ShowOutput = false;
	outThreadInfo.ShowOutput = ShowOutput;
	errThreadInfo.ShowOutput = ShowOutput;

	// RHT 02/04/2012 - DM handle for logging
	inThreadInfo.ostr = outThreadInfo.ostr = errThreadInfo.ostr = &ostr;

	// RHT 08/08/2013 - set the threadId
	inThreadInfo.threadId = threadId;
	outThreadInfo.threadId = threadId;
	errThreadInfo.threadId = threadId;
	
	InThread=_beginthreadex(NULL,
						0,
						StdInThread,
						(LPVOID)&inThreadInfo,
						0,
						NULL);

	ThreadError=GetLastError();

	if(InThread==NULL) {
		TRIERROR(("Execute - Error creating InThread, ThreadError=%ld",ThreadError));
	} else {
		TRIDEBUG(("Execute - InThread is up and running"));
	}


	OutThread=_beginthreadex(NULL,
						0,
						StdOutThread,
						(LPVOID)&outThreadInfo,
						0,
						NULL);

	ThreadError=GetLastError();

	if(OutThread==NULL) {
		debug3("Execute - Error creating OutThread, ThreadError=%ld",ThreadError);
	} else {
		debug3("Execute - OutThread is up and running");
	}


	ErrThread=_beginthreadex(NULL,
						0,
						StdErrThread,
						(LPVOID)&errThreadInfo,
						0,
						NULL);

	ThreadError=GetLastError();

	if(ErrThread==NULL) {
		debug3("Execute - Error creating ErrThread, ThreadError=%ld",ThreadError);
	} else {
		debug3("Execute - ErrThread is up and running");
	}

	
	// Now loop round waiting for the read/write events to occur.
	// Once these have been serviced then we just check the state of play
	// and carry on until the process terminates.
	DWORD WaitResult;
	DWORD WaitError;


	debug3("Execute - Wait for process to finish");

	WaitResult=WaitForSingleObject(pi.hProcess, INFINITE);
	WaitError=GetLastError();
	debug3("Execute - WaitResult=%ld, LastError=%ld",WaitResult, WaitError);

	if(WaitResult==WAIT_TIMEOUT) 
	{
		debug3("Execute - Waiting timed out");
	} else if(WaitResult==WAIT_ABANDONED) {
		debug3("Execute - Waiting abandoned on process");
	}
	
	debug3("Execute - Getting exit status ...");

	DWORD ExitStatus;
	if (GetExitCodeProcess(pi.hProcess,&ExitStatus)==0)
	{
		debug3("GetExitCodeProcess fails\n");		
	}
	else
	{
		debug3("GetExitCodeProcess succeeds, ExitStatus=%d\n",ExitStatus);

	}
	*ChildExitStatus=ExitStatus;

	debug3("Execute - Got exit status=%ld",ExitStatus);

	// Now Wait for StdIn thread
	WaitForSingleObject((void *)InThread,INFINITE);

	debug3("Execute - Finished waiting for InThread");

	// Now Wait for StdOut thread
	WaitForSingleObject((void *)OutThread,INFINITE);

	debug3("Execute - Finished waiting for OutThread");

	// Now Wait for StdErr thread
	WaitForSingleObject((void *)ErrThread,INFINITE);

	debug3("Execute - Finished waiting for ErrThread");

	// if any threads exited with an error, find out here
	DWORD ThreadExitCode;
	if (GetExitCodeThread((void *)InThread,&ThreadExitCode))
	{
		if (ThreadExitCode == 1) ErrNum=CLIENT_COMMS_FAILURE;
	}
	if (GetExitCodeThread((void *)OutThread,&ThreadExitCode))
	{
		if (ThreadExitCode == 1) ErrNum=CLIENT_COMMS_FAILURE;
	}
	if (GetExitCodeThread((void *)ErrThread,&ThreadExitCode))
	{
		if (ThreadExitCode == 1) ErrNum=CLIENT_COMMS_FAILURE;
	}

	// Now close the handles
	debug3(("Execute - Closing Handles"));

	// CloseHandle(ChildStdIn);
	CloseHandle(ChildStdOut);
	CloseHandle(ChildStdErr);
	// CloseHandle(InThread);
	// CloseHandle(OutThread);
	// CloseHandle(ErrThread);
	CloseHandle(pi.hProcess);

	//
	// Free up the sorted environment
	//
//	free(e);


	debug3("Execute - Exit, rc=%d",ErrNum);

	return ErrNum;

}
