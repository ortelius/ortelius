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
		"$postgreshome/bin/psql" -A -w -U "$username" -f "$sqlscript" $database 2>&1
	fi
done
