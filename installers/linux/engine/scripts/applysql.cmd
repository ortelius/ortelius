@echo off
set dbtype=%~1
set dbhome=%~2
set database=%~3
set username=%~4
set password=%~5

REM POSTGRES SQL
if NOT "%dbtype" == "postgres" goto :oracle

if exist "%dbhome%" goto homeok
echo POSTGRES_HOME is not set or does not exist
exit /B 1
:homeok
if exist "%dbhome%\bin\psql.exe" goto psqlok
echo Cannot find psql.exe in POSTGRES_HOME\bin
exit /B 1
:psqlok 

shift /5

:loop
if "x%~1" == "x" goto :end_loop
set sqlscript=%~nx1
set fileext=%~x1
set sqldir=%~dp1
shift

if NOT "%fileext%" == ".sql" goto :loop

set PGPASSWORD=%password%

echo.Running %sqlscript% in directory %sqldir%
cd /d "%sqldir%"
"%dbhome%\bin\psql.exe" -A -w -U %username% -f "%sqlscript%" "%database%" 
goto :loop

:end_loop

REM ORACLE SQL
if NOT "%dbtype" == "oracle" goto :sqlserver

:oracle
if exist "%dbhome%" goto oraclehomeok
echo ORACLE_HOME is not set or does not exist
exit /B 1
:oraclehomeok
if exist "%dbhome%\bin\sqlplus.exe" goto sqlplusok
echo Cannot find sqlplus.exe in ORACLE_HOME\bin
exit /B 1
:sqlplusok 

shift /5

:loop_ora
if "x%~1" == "x" goto :end_loop_ora
set sqlscript=%~nx1
set fileext=%~x1
set sqldir=%~dp1
shift

if NOT "%fileext%" == ".sql" goto :loop_ora

echo.Running %sqlscript% in directory %sqldir%
cd /d "%sqldir%"
"%dbhome%\bin\sqlplus.exe" "%username%\%password%@%database%" @"%sqlscript%" 
goto :loop_ora

:end_loop_ora

REM SQLSERVER SQL

:sqlserver
if exist "%dbhome%" goto sqlserverhomeok
echo SQLSERVER_HOME is not set or does not exist
exit /B 1
:sqlserverhomeok
if exist "%dbhome%\bin\sqlcmd.exe" goto sqlserverok
echo Cannot find sqlcmd.exe in SQLSERVER_HOME\bin
exit /B 1
:sqlserverok 

shift /5

:loop_sqlserver
if "x%~1" == "x" goto :end_loop_sqlserver
set sqlscript=%~nx1
set fileext=%~x1
set sqldir=%~dp1
shift

if NOT "%fileext%" == ".sql" goto :loop_sqlserver

set SQLCMDPASSWORD=%password%

echo.Running %sqlscript% in directory %sqldir%
cd /d "%sqldir%"
"%dbhome%\bin\sqlcmd.exe" -S "%database%" -U %username% -i "%sqlscript%" 
goto :loop_sqlserver

:end_loop_sqlserver
echo.done.
EXIT /B 0