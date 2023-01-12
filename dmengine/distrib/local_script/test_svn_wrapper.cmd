@echo off
rem .\svn_wrapper.cmd "--non-interactive" "--trust-server-cert" ^
rem   "--no-auth-cache" "-r" "1" "--username" "harvest" "--password" "harvest" ^
rem  "checkout" "svn://linux-gnh2/dmdemo/target" "C:\Users\Robert\AppData\Local\Temp\tdm.0.fred"

"C:\Shared\DM2\DeploymentManager\local_script\svn_wrapper.cmd" "--non-interactive" ^
  "--trust-server-cert" "--no-auth-cache" "--username" "harvest" "--password" ^
  "harvest" "checkout" "svn://linux-gnh2/dmdemo/sql" "C:\Users\Robert\AppData\Local\Temp\tdm.0.fred\sql_files"
