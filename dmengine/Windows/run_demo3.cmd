@echo off
setlocal

set binary=Debug
rem set binary=Release

set TRIREASON=SCRIPT
set TRICLIENTUSERNAME=robert
set TRIFIELD1=envA
set TRIFIELD2=Robert
set TRIFIELD3=password
set TRIFIELD4=demo3

.\%binary%\DeploymentManager.exe ^
  -fields "environments,username,password,applications" ^
  -pw "harvest" %*
