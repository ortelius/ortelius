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
