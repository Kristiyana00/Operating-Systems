#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [ ! -f "${FILE}" ]; then
	echo "First argument is not a file!"
	exit 2
elif [ ! -r "${FILE}" ]; then
	echo "File is not readable!"
	exit 3
fi

if [ ! -d "${DIR}" ]; then
	echo "Second argument is not a directory!"
	exit 4
elif [ ! -r "${DIR}" ]; then
	echo "Directory is not readable!"
	exit 5
fi

SEARCHED_HASH=$(md5sum "${FILE}" | awk '{print $1}')

CNT=0

while read CUR_HASH FILE_PATH; do
	if [ "${CUR_HASH}" = "${SEARCHED_HASH}" ]; then
		echo "$(basename ${FILE_PATH})"
		CNT=$(( ++CNT ))
	fi
done < <(find "${DIR}" -type f 2>/dev/null -print0 | xargs -0 -I {} md5sum {})

if [ $CNT -eq 0 ]; then
	echo "No copies found!"
else 
	echo "Found total $CNT copies!"
fi
