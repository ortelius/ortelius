#!/bin/sh
# ""C:\Cygwin\bin\svn.exe" "--non-interactive" "--trust-server-cert"
#     "--no-auth-cache" "-r" "1" "--username" "harvest" "--password" "harvest"
#     "checkout" "svn://linux-gnh2/dmdemo/target" "/tmp/tdm.0.fred""

cmd="/bin/svn"

dz=
while [[ $# -gt 0 ]]
do
  case "$1" in
  *tdm.*) val=$(cygpath -u "$1")
         wdz=$(echo "$1" | sed 's|\\|\\\\|g');
         udz="$val"
         ;;
  *)     val="$1"
         ;;
  esac
  
  cmd="${cmd} \"${val}\""
  shift
done

echo "$cmd"
eval "$cmd" | sed 's|'$udz'|'$wdz'|g;s|/|\\|g'
