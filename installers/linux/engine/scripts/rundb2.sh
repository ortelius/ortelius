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

#!/bin/ksh
. /home/db2inst4/sqllib/db2profile
dbname="$1"
user="$2"
password="$3"
dbfile="$4"
echo "Params are $*"
# create temp response file
echo "file is [$dbfile]"
(
echo "connect to ${dbname} user ${user} USING ${password};"
while read t
do
s=$(echo $t | sed 's/[ 	]//g')
if [[ -n "$s" ]]
then
	echo $t
	case $t in
		*\;)	el=Y;;
		*)	el=N;;
	esac
fi
done < "$dbfile"
[[ "$el" = "N" ]] && echo ";" # terminate sql if the file didn't end with ;
echo "terminate;"
) > /tmp/dbip$$
db2 -tf /tmp/dbip$$
# uncomment following line to output the response file to stdout
cat /tmp/dbip$$ 
rm -f /tmp/dbip$$
