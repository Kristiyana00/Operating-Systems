#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

dir="${1}"
str="${2}"

if [ ! -d "${dir}" ]; then
	echo "Invalid directory!"
	exit 2
fi

if [ ! -r "${dir}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

if [ ! -n "${str}" ]; then
	echo "The string is empty!"
	exit 4
fi

temp=$(mktemp)

find "${dir}" -maxdepth 1 -type f -printf "%f\n" 2>/dev/null | egrep "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${str}$" > "${temp}"

newestVersion="$(cat "${temp}" | cut -d '-' -f 2,2 | sort -t '.' -nr -k 1,1 -k 2,2 -k 3,3 | head -n 1)"

egrep "${newestVersion}" "${temp}"

rm -- "${temp}"

exit 0
