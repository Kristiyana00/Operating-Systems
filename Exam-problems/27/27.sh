#!/bin/bash

if [ $# -ne 1 ] && [ $# -ne 2 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

dir="${1}"
file=""

if [ ! -d "${dir}" ]; then
	echo "Invalid directory!"
	exit 4
fi

if [ ! -r "${dir}" ]; then
	echo "Directory is not readable!"
	exit 5
fi

if [ $# -eq 2 ]; then
	file="${2}"
	
	if [ ! -f "${file}" ]; then
		echo "Invalid file!"
		exit 2
	fi

	if [ ! -w "${file}" ]; then
		echo "File is not writable!"
		exit 3
	fi
fi

cnt=0

while read symlink; do
	if egrep -q "broken symbolic link" <(echo "${symlink}"); then
		cnt=$(expr $cnt + '1')
	else
		if [ -n "${file}" ]; then
			stat -c "%N" "$(echo "${symlink}" | cut -d ':' -f 1)" | tr -d "\'" >> "${file}"
		else
			stat -c "%N" "$(echo "${symlink}" | cut -d ':' -f 1)" | tr -d "\'"
		fi
	fi
done < <(find "${dir}" -type l 2>/dev/null -exec file {} \;)

if [ -n "${file}" ]; then
	cat "${file}"
fi

echo "Broken symlinks: ${cnt}"
