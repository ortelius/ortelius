@echo off
set dp=%~dp0
rem setlocal
rem set TRIREASON=SCRIPT
rem set TRIFIELD1=Demo Environment (FTP)
rem set TRIFIELD4=DMDemo AppVer 2.0
rem "C:\Shared\DM2\DeploymentManager\Windows\Debug\DeploymentManager.exe" ^
rem  "-home" "C:\Shared\DM2\DeploymentManager\distrib" "-usr" "robert" ^
rem  "cmdln_haruser=harvest" "cmdln_harpass=harvest" ^
rem  "cmdln_harproject="DMDemo"" "cmdln_harstate="Test"" "DMDemo-2.0" 

set >%dp%/env.log
echo.%* >>%dp%/env.log
"C:\Shared\DM2\DeploymentManager\Windows\Debug\DeploymentManager.exe" %* >%dp%/out.log 2>&1