#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

dir="${1}"

if [ ! -d "${dir}" ]; then
	echo "Invalid directory name!"
	exit 2
elif [ ! -r "${dir}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

cnt=0

while read -d $'\n' line; do
	if [ -x "${line}" ]; then
		(( cnt+=1 ))
	fi
done < <(find "${dir}" -type f 2>/dev/null)

echo "Number of executable files is ${cnt}"
