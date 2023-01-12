@echo off
setlocal

set binary=bin
rem set binary=Debug
rem set binary=Debugx64
rem set binary=Release

rem goto :populate_envs
rem goto :populate_combo1
rem goto :populate_listbox
rem goto :run_dmdemo
rem goto :run_sieve
rem goto :parse_script


:run_script
rem envB
set TRIREASON=SCRIPT
set TRICLIENTUSERNAME=robert
set TRIFIELD1=envA
set TRIFIELD2=Robert
set TRIFIELD3=password
set TRIFIELD4=app2
set TRILISTBOXSELECTIONS=2
set TRI_PACKAGENAME_1=MyPackage1
set TRI_CREATED_BY_1=fred
set TRI_CREATED_ON_1=18:03 29/08/2012
set TRI_PACKAGENAME_2=MyPackage2
set TRI_CREATED_BY_2=jim
set TRI_CREATED_ON_2=18:04 29/08/2012
set vars=cmdln_var=testvalue argv1 argv2
goto :runit

:run_dmdemo
rem envB
set TRIREASON=SCRIPT
set TRICLIENTUSERNAME=demo
set TRIFIELD1=Demo Environment
set TRIFIELD2=
set TRIFIELD3=
set TRIFIELD4=DMDemo
rem set TRIFIELD4=DMDemo (Rollback)
set TRILISTBOXSELECTIONS=1
set TRI_PACKAGENAME_1=DMDemo-2.0
set TRI_ASSIGNED_TO_1=demo
set TRI_MODIFIED_ON_1=16:01 17/09/2012
set TRI_CREATED_ON_1=18:03 29/08/2012
rem set vars=cmdln_harstate="Test" cmdln_harpackages="'DMDemo-2.0'"
set vars=cmdln_harproject="DMDemo" cmdln_harstate="Test" "DMDemo-2.0"
rem "DMDemo-1.0"
goto :runit

:run_sieve
set TRIREASON=SCRIPT
set TRICLIENTUSERNAME=robert
set TRIFIELD1=envA
set TRIFIELD2=Robert
set TRIFIELD3=password
set TRIFIELD4=Sieve
goto :runit

:parse_script
.\%binary%\DeploymentManager.exe -parse ..\input.dm
echo %ERRORLEVEL%
goto :EOF

:populate_envs
set TRIREASON=POPULATE
set TRICLIENTUSERNAME=jamie
set TRICURRENTFIELD=1
echo.Environments for 'jamie':
call :runit

set TRICLIENTUSERNAME=robert
echo.Environments for 'robert':
call :runit
rem goto :EOF

:populate_apps
set TRIREASON=POPULATE
set TRICLIENTUSERNAME=jamie
set TRICURRENTFIELD=4
echo.Applications for 'jamie':
call :runit

set TRICLIENTUSERNAME=robert
echo.Applications for 'robert':
call :runit

set TRICLIENTUSERNAME=robert
set TRICHANGEDFIELD=1
set TRIFIELD1=envA
echo.Applications for 'robert' and 'envA':
call :runit

:populate_username
set TRICLIENTUSERNAME=robert
set TRICHANGEDFIELD=1
set TRICURRENTFIELD=2

set TRIFIELD1=envA
echo.Username for 'envA':
call :runit

set TRIFIELD1=envB
echo.Username for 'envB':
call :runit

set TRIFIELD1=envC
echo.Username for 'envC':
call :runit

:populate_password
set TRICLIENTUSERNAME=robert
set TRICHANGEDFIELD=1
set TRICURRENTFIELD=3

set TRIFIELD1=envA
echo.Password for 'envA':
call :runit

set TRIFIELD1=envB
echo.Password for 'envB':
call :runit

set TRIFIELD1=envC
echo.Password for 'envC':
call :runit

:populate_combo1
set TRIREASON=POPULATE
set TRICLIENTUSERNAME=robert
set TRICHANGEDFIELD=
set TRICURRENTFIELD=5
echo.Combo1:
call :runit

:populate_combo2
set TRICHANGEDFIELD=
set TRICURRENTFIELD=6
echo.Combo2:
call :runit

:populate_combo3
set TRICHANGEDFIELD=
set TRICURRENTFIELD=7
echo.Combo3:
call :runit

:populate_validate
set TRIREASON=VALIDATION
set TRIFIELD1=envA
set TRIFIELD2=
set TRIFIELD3=
set TRIFIELD4=app1
call :runit
goto :EOF

:populate_listbox
set TRIREASON=LISTBOX
set TRICLIENTUSERNAME=robert
set vars=cmdln_harstate="Test" cmdln_harpackages="'DMDemo-1.0' 'DMDemo-2.0'"
echo.Listbox:
call :runit
goto :EOF

:runit
.\%binary%\DeploymentManager.exe -nonotify ^
  -fields "environments,username,password,applications,combo1,combo2,combo3" ^
  -pw "harvest" %* cmdln_haruser=harvest cmdln_harpass=harvest %vars%
goto :EOF
