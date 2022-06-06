#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

N=10
CUR_ARG=1

if [ "${1}" = "-n" ] && [ $# -gt 2 ]; then
	N="${2}"
	CUR_ARG=3
fi

ALL_ARG="$#"

buf=$(mktemp)

while [ $CUR_ARG -le $ALL_ARG ]; do
	CUR_FILE="${!CUR_ARG}"
	echo "${CUR_FILE}"
	
	if [ -f "${CUR_FILE}" ] && [ -r "${CUR_FILE}" ]; then
		cat "${CUR_FILE}" | tail -n $N | sed -E "s/([^ ]+ [^ ]+ )(.*)/\1$CUR_FILE \2/" >> "$buf"
	fi

	CUR_ARG=$(expr $CUR_ARG + 1)
done

cat "$buf" | sort

exit 0
