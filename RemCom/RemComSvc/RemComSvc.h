/*
	Copyright (c) 2006 Talha Tariq [ talha.tariq@gmail.com ] 
	All rights are reserved.

	Permission to use, copy, modify, and distribute this software 
	for any purpose and without any fee is hereby granted, 
	provided this notice is included in its entirety in the 
	documentation and in the source files.

	This software and any related documentation is provided "as is" 
	without any warranty of any kind, either express or implied, 
	including, without limitation, the implied warranties of 
	merchantability or fitness for a particular purpose. The entire 
	risk arising out of use or performance of the software remains 
	with you. 

 	$Author:	Talha Tariq [ talha.tariq@gmail.com ] 
	$Revision:	Talha Tariq [ talha.tariq@gmail.com ] 	
	$Date: 2006/10/03 09:00:00 $ 		
	$Version History: $			- 
	$TODO:						- Implement Delete Service
	$Description: $				- RemCom Service is contained in the parent binary as a local resource which is extracted at runtime from itself
								  pushed to admin$, installed to the remote service control manager which interacts remotely for local process invocation
								
	$Workfile: $				- RemComSvc.h
 */
#ifndef RemComSVC_H_INCLUDED
#define RemComSVC_H_INCLUDED

extern HANDLE hStopServiceEvent;

void _ServiceMain(void*);
void DeleteSvc();

#endif