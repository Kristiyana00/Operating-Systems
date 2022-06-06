#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

dir="${1}"
num="${2}"

if [ ! -d "${dir}" ]; then
	echo "First argument is not a directory!"
	exit 2
elif [ ! -r "${dir}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

function validate_num
{
    grep -qE '^[+-]?[0-9]+$' <(echo "${1}")
}

if validate_num "${num}"; then
	while read -r -d% SIZE FILE; do
		if [ "${SIZE}" -gt "${num}" ]; then
			echo "${FILE}"
		fi
	done < <(find "$dir" -type f 2>/dev/null -printf "%s %p%%")
else
	echo "Second argument is not an integer!"
	exit 4
fi

