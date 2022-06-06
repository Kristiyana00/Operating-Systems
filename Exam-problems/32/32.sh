#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

f1="${1}"
f2="${2}"

if [ ! -f "${f1}" ]; then
	echo "$f1 is invalid file!"
	exit 2
elif [ ! -r "${f1}" ]; then
	echo "$f1 is not readable!"
	exit 3
fi

if [ -e "${f2}" ]; then
	echo "$2 already exists!"
	exit 4
fi

touch "${f2}"

if [ ! -w "${f2}" ]; then
	echo "$f2 is not writable!"
	exit 5
fi

while read line; do
	CUTLINE="$(echo "${line}" | cut -d ',' -f 2-)"
	if egrep -q ",${CUTLINE}$" "${f2}"; then
		continue
	else
		echo "${line}" >> "${f2}"
	fi
done < <(cat "${f1}" | sort -t ',' -n -k 1)
