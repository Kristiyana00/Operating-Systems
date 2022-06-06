#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

dir="${1}"
num="${2}"

if [ ! -d "${dir}" ]; then
	echo "Invalid directory!"
	exit 2
elif [ ! -r "${dir}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

function validate_num {
	egrep -q '^[+-]?[0-9]+$' <(echo "${num}")
}

if validate_num "${num}"; then
	all_sizes=$(find "${dir}" -maxdepth 1 -type f 2>/dev/null -size +"${num}"c -printf "%s\n")
	total_size=0
	for i in ${all_sizes}; do
		(( total_size+="${i}" ))
	done
	echo "${total_size}"
else
	echo "Second argument is an invalid integer number!"
	exit 4
fi
