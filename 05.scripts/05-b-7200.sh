#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

for i; do
	if [ -f "${i}" ]; then
		if [ -r "${i}" ]; then
			echo "$(basename "${i}") is readable file!"
		else
			echo "$(basename "${i}") is not readable file!"
		fi
	elif [ -d "${i}" ]; then
		cnt_files=$(find "${i}" -type f 2>/dev/null | wc -l | awk '{$1=$1}1')
		echo "$(find "${i}" -type f -size -"${cnt_files}"c 2>/dev/null | xargs basename -a)"
	else
		echo "$(basename "${i}") is not a file/directory!"
	fi
done

