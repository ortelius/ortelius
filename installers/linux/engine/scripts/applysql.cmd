@echo off
set dbtype=%~1
set postgreshome=%~2
set database=%~3
set username=%~4
set password=%~5
if exist "%postgreshome%" goto homeok
echo POSTGRES_HOME is not set or does not exist
exit 1
:homeok
if exist "%postgreshome%\bin\psql.exe" goto psqlok
echo Cannot find psql.exe in POSTGRES_HOME\bin
exit 1
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
"%postgreshome%\bin\psql.exe" -A -w -U %username% -f "%sqlscript%" "%database%" 
goto :loop

:end_loop
echo.done.
