#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Incorrect number of arguments!"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "The given argument is not a directory!"
	exit 2
fi

echo "1)"
find -L "$1" -type l

echo "2)"
find "$1" -type l -exec file {} \; | grep ": broken symbolic" | cut -f 1 -d :

echo "3)"
find "$1" -type l -exec cat {} \; 2>&1 1>/dev/null | grep "No such file or directory" | cut -d : -f 2 | cut -d ' ' -f 2

echo "4)"
find "$1" -type l -exec stat -c "%N" {} \; | while read line; do 
	link_name="$(echo $line | cut -d " " -f 2)"
	link_taget="$(echo $line | cut -d " " -f 4)"

	if [ ! -e "$(dirname "$link_name")/$link_target" ]; then
		echo "$link_name"
	fi
done

exit 0
