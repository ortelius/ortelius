@echo off
set basedir=%~dp0
setlocal

set binary=Debug
rem set binary=Release

:run_dmdemo
rem envB
set TRIREASON=SCRIPT
set TRICLIENTUSERNAME=demo
rem set TRIFIELD1=Demo Environment (SSH)
set TRIFIELD1=Demo Environment (FTP)
set TRIFIELD2=
set TRIFIELD3=
rem set TRIFIELD4=DMDemo
rem set TRIFIELD4=DMDemo (Rollback)
rem set TRIFIELD4=DMDemo Graphical
rem set TRIFIELD4=DMDemo AppVer 2.0
set TRIFIELD4=DMDemo P4 Components
set TRILISTBOXSELECTIONS=1
set TRI_PACKAGENAME_1=DMDemo-2.0
set TRI_ASSIGNED_TO_1=demo
set TRI_MODIFIED_ON_1=16:01 17/09/2012
set TRI_CREATED_ON_1=18:03 29/08/2012
set vars=cmdln_harproject="DMDemo" cmdln_harstate="Test" "DMDemo-2.0"

.\%binary%\DeploymentManager.exe -nonotify -home "%basedir%..\distrib" ^
  -fields "environments,username,password,applications,combo1,combo2,combo3" ^
  -pw "harvest" %* cmdln_haruser=harvest cmdln_harpass=harvest %vars%
