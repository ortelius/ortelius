#!/bin/sh
echo "applysql.bat $*"
action=$1
shift

while [[ $# -gt 0 ]]
do
	sqlscript=$(basename $1)
	fileext=${sqlscript##*.}
	sqldir=$(dirname $1)
	shift

	case ${action} in
	APPLY)
		[[ "${sqlscript:0:9}" == "rollback_" ]] && continue
		;;
	ROLLBACK)
		[[ "${sqlscript:0:9}" == "rollback_" ]] || continue
		;;
	*)
		echo "Unrecognised action ${action}."
		exit 1
		;;
	esac

	[[ "${fileext}" == "sql" ]] || continue

	echo "Running ${sqlscript}"
	echo "sqlplus dmdemo/dmdemo @${sqldir}/${sqlscript}"
	sqlplus -logon dmdemo/dmdemo @${sqldir}/${sqlscript}
done

echo "done."
