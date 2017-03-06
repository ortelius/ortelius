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
	$Date: 2006/11/03 09:00:00 $ 		
	$Version History: $			- Added OpenProcessToken to get the preocess token.
								- Fixed the bug for Windows 2000 where Calling LogonUser API needs an SE_TCB_NAME privilege which is not enabled by default
	$TODO:						- Code Cleanup. - Move the class as UniCode implementation to the RemCom project
	$Description: $				- ProcCom binary is contained in the parent binary as a local resource which is extracted at runtime from itself
								  for local prorcess and user impersonation
	$Workfile: $				- ProcComs.h
 */


#if !defined(AFX_STDAFX_H__80E97CF6_A64A_4F6A_B036_7E512D741A73__INCLUDED_)
#define AFX_STDAFX_H__80E97CF6_A64A_4F6A_B036_7E512D741A73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// TODO: reference additional headers your program requires here
#define UNICODE
// Works only on windows 2000 and above
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stdio.h>
#include <userenv.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__80E97CF6_A64A_4F6A_B036_7E512D741A73__INCLUDED_)
