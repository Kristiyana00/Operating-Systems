#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

read -p "Enter a string: " STRING

while [ $# -ne 0 ]; do
	FILE="${1}"

	if [ ! -f "${FILE}" ]; then
		echo "Invalid file name given as argument!"
		exit 2
	elif [ ! -r "${FILE}" ]; then
		echo "File is not readable!"
		exit 3
	else 
		CUR_MATCHES=$(grep -oc "${STRING}" "${FILE}")
		echo "In file \"${FILE}\", ${CUR_MATCHES} rows contains string \"${STRING}\""
	fi
	shift 1
done
