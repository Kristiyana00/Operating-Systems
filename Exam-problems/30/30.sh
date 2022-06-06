#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

LOGDIR="${1}"

if [ ! -d "${LOGDIR}" ]; then
   	echo "Invalid directory!"
	exit 2
fi

if [ ! -r "${LOGDIR}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

RECORDS=$(mktemp)

while read FRIEND; do
	LINES="$(find "${LOGDIR}" -mindepth 4 -maxdepth 4 -type f | egrep "${FRIEND}" | xargs - I {} wc -l {} | awk '{print $1}' | awk '{sum += $1} END {print sum}')"

	echo "$LINES $FRIEND" >> "${RECORDS}"
done < <(find "${LOGDIR}" -mindepth 3 -maxdepth 3 -type d | cut -d '/' -f 4 | sort | uniq)

cat "${RECORDS}" | sort -nr -k 1 | head -n 10
