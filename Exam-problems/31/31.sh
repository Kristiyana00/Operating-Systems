#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [ ! -f "${FILE}" ]; then
	echo "Invalid file name!"
	exit 2
elif [ ! -r "${FILE}" ]; then
	echo "File is not readable!"
	exit 3
fi

if [ ! -d "${DIR}" ]; then
	echo "Invalid directory!"
	exit 4
elif [ ! -w "${DIR}" ]; then
	echo "Directory is not writable!"
	exit 5
fi

if [ "$(find "${DIR}" -mindepth 1 | wc -l)" -ne 0 ]; then
	echo "Directory is not empty!"
	exit 6
fi

CNT=1

touch "${DIR}/dict.txt"

while read line; do
	FN_LN="$(cut -d ':' -f 1 < <(echo "${line}") | sed -E "s/\(.*\)//" | awk '$1=$1')"
	NUMBER="$(cat "${DIR}/dict.txt" | egrep "${FN_LN}" | cut -d ';' -f 2)"

	if [ -z "${NUMBER}" ]; then
		echo "${FN_LN};${CNT}" >> "${DIR}/dict.txt"
		NUMBER="${CNT}"
		CNT=$(expr "${CNT}" + 1)
	fi
	echo "${line}" | cut -d ':' -f 2 >> "${DIR}/${NUMBER}.txt"
done < "${FILE}"

exit 0
