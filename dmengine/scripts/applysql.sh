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

#!/bin/sh
dbtype=$1
postgreshome="$2"
database="$3"
username="$4"
password="$5"
if [ ! -d "$postgreshome" ]
then
	echo POSTGRES_HOME is not set or does not exist >&2
	exit 1
fi
if [ ! -f "$postgreshome/bin/psql" ]
then
	echo Cannot find psql in POSTGRES_HOME\bin >&2
	exit 1
fi

shift 5

while [ -n "$1" ]
do
	sqlscript=$(basename "$1")
	fileext=$(echo "$1" | sed 's/^.*[.]//')
	sqldir=$(dirname "$1")
	shift
	if [ "$fileext" = "sql" ]
	then
		export PGPASSWORD="$password"
		echo "Running $sqlscript in directory $sqldir"
		cd "$sqldir"
		"$postgreshome/bin/psql" -A -w -U "$username" -f "$sqlscript" "$database" 
	fi
done
