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
   merchantibility or fitness for a particular purpose. The entire 
   risk arising out of use or performance of the software remains 
   with you. 
   
*/

#ifndef xProcFunctions_H_INCLUDED
#define xProcFunctions_H_INCLUDED


#define UNICODE
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stdio.h>
#include <userenv.h>

class ProcessHandler{

public:
	void DisplayError(LPWSTR pszAPI);
	void CreatProc(int argc, WCHAR *argv[]);

};
#endif