@echo off
set DIR=%~dp0
set PATH=C:\Cygwin\bin;%PATH%
C:\Cygwin\bin\bash.exe "%DIR%\svn_wrapper.sh" %*
