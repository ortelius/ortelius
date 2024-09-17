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
