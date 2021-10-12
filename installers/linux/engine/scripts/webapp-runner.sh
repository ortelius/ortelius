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

if [ "$1" = "start" ]; then
 echo Starting Up
 echo java -DTCNODE="$4" $5 -jar /tmp/webapp-runner.jar --port $2 $3
 CWD=`dirname "$3"`
 tfile="/tmp/$2.PID"
 cd $CWD
 java -DTCNODE="$4" $5 -jar /tmp/webapp-runner.jar --port $2 $3 1>/dev/null 2>/dev/null & s_pid=$!;
 echo $s_pid > $tfile; 
 echo -n PID=
 cat $tfile
else
 echo Shutting down
 echo java -DTCNODE="$4" -jar /tmp/webapp-runner.jar --port $2 $3
 PID_FILE=`basename "$3"`
 tfile="/tmp/$2.PID"
 cat $tfile | xargs kill
fi
