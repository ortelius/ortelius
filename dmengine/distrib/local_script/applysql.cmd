@echo off
echo.applysql.bat %*

:loop
if "x%~1" == "x" goto :end_loop
set sqlscript=%~nx1
set fileext=%~x1
set sqldir=%~dp1
shift

if NOT "%fileext%" == ".sql" goto :loop

echo.Running %sqlscript%
cd /d "%sqldir%"
mysql -u root -proot mysql <%sqlscript%
goto :loop

:end_loop
echo.done.
