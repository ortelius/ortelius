@echo off
set basedir=%~dp0
setlocal

set binary=Debug
rem set binary=Debugx64
rem set binary=Release

rem goto :populate_envs
rem goto :populate_combo1
rem goto :populate_listbox
rem goto :run_dmdemo
goto :run_dmdemo_binrepo
rem goto :run_sieve
rem goto :parse_script
rem goto :encrypt
rem goto :test_repository
rem goto :test_notify
rem goto :test_datasource
rem goto :do_notify
rem goto :do_runtask


:run_script
rem envB
set TRIREASON=SCRIPT
set TRIDM_USER=admin
set TRICLIENTUSERNAME=robert
set TRIFIELD1=envA
rem set TRIFIELD1=envLocal
rem set TRIFIELD1=DevEnv
set TRIFIELD2=Robert
set TRIFIELD3=password
set TRIFIELD4=app2
rem set TRIFIELD4=macys
rem set TRIFIELD4=CreditScore;3
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
set TRIDM_USER=admin
set TRICLIENTUSERNAME=demo
rem set TRIFIELD1=Demo Environment
set TRIFIELD1=Demo Environment (WIN)
rem set TRIFIELD1=Demo Environment (SSH)
set TRIFIELD2=
set TRIFIELD3=
rem set TRIFIELD4=DMDemo
rem set TRIFIELD4=DMDemo (Rollback)
rem set TRIFIELD4=DMDemo AppVer 2.0
rem set TRIFIELD4=DMDemo P4 Components
rem set TRIFIELD4=DMDemo BinaryRepo
rem set TRIFIELD4=DMDemo Graphical
set TRIFIELD4=ComponentTest;2
set TRILISTBOXSELECTIONS=1
set TRI_PACKAGENAME_1=DMDemo-2.0
set TRI_ASSIGNED_TO_1=demo
set TRI_MODIFIED_ON_1=16:01 17/09/2012
set TRI_CREATED_ON_1=18:03 29/08/2012
rem set vars=cmdln_harstate="Test" cmdln_harpackages="'DMDemo-2.0'"
set vars=cmdln_harproject="DMDemo" cmdln_harstate="Test" "DMDemo-2.0"
rem "DMDemo-1.0"
goto :runit

:run_dmdemo_binrepo
set TRIREASON=SCRIPT
set TRIDM_USER=admin
set TRICLIENTUSERNAME=admin
set TRIFIELD1=Demo Environment (WIN)
set TRIFIELD2=
set TRIFIELD3=
set TRIFIELD4=DMDemo BinaryRepo;2
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

:encrypt
echo.Hello>testinput.txt
.\%binary%\DeploymentManager.exe -home "%basedir%..\distrib" -usr robert -encrypt <testinput.txt
echo %ERRORLEVEL%
goto :EOF

:test_repository
rem .\%binary%\DeploymentManager.exe -home "%basedir%..\distrib" -usr robert -providertest repository 4
.\%binary%\DeploymentManager.exe -home "%basedir%..\distrib" -usr demo -providertest repository 100
echo %ERRORLEVEL%
goto :EOF

:test_notify
rem robert 1 - trinem email
rem admin 5  - oms email
.\%binary%\DeploymentManager.exe -home "%basedir%..\distrib" -usr admin -nonotify -providertest notify 2
echo %ERRORLEVEL%
goto :EOF

:test_datasource
.\%binary%\DeploymentManager.exe -home "%basedir%..\distrib" -usr robert -providertest datasource 1
echo %ERRORLEVEL%
goto :EOF

:do_notify
rem echo.Hello>testmsg.txt
rem echo.>>testmsg.txt
rem echo.This is a test email.>>testmsg.txt
rem echo.>>testmsg.txt
rem echo.Sincerely,>>testmsg.txt
rem echo.>>testmsg.txt
rem echo.     DM2>>testmsg.txt
.\%binary%\DeploymentManager.exe -home "%basedir%..\distrib" -usr admin ^
  -notify 1 -subject "This is a test" -from "robert@trinem.com" -template 1 ^
  -appid 305 approval_status=approved target_domain=SIT us1 us4
rem <testmsg.txt
echo %ERRORLEVEL%
goto :EOF

:do_runtask
rem approve        = 14 -appid 305
rem create version = 22 -appid 301 -app2id 305
rem v3 = 305; v6 = 308
.\%binary%\DeploymentManager.exe -home "%basedir%..\distrib" -usr admin ^
  -runtask 22 -appid 301 -app2id +1
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
.\%binary%\DeploymentManager.exe -nonotify -home "%basedir%..\distrib" ^
  -fields "environments,username,password,applications,combo1,combo2,combo3" ^
  -pw "harvest" %* cmdln_haruser=harvest cmdln_harpass=harvest %vars%
goto :EOF
