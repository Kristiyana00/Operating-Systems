#!/bin/bash

TEMP=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n > "${TEMP}"

if [ $(wc -l "${TEMP}") -eq 0 ]; then
	echo "No valid numbers!"
	exit 1
else
	echo "There are $(wc -l ${TEMP} | cut -d ' ' -f 1) valid numbers!"
fi
