#!/bin/sh
cd "$1"
case "$2" in
	*.zip)	unzip -o "$2";;
	*.tar)	tar xvf "$2";;
		*)	echo "Unknown file extension for $2">&2;;
esac