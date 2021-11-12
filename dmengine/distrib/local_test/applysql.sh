#!/bin/sh

# Copyright (c) 2021 Linux Foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
