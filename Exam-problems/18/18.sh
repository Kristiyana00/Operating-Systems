#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

first="$1"
second="$2"

mkdir a b c

find /home/students/s62372 -type f | while read line; do
	lines=$(wc -l "${line}" | cut -d ' ' -f 1)
	if [ "${lines}" -lt "${first}" ]; then
		cp "${line}" a/
	elif [ "${lines}" -ge "${first}" ] && [ "${lines}" -le "${second}" ]; then
		cp "${line}" b/
	else
		cp "${line}" c/
	fi
done
