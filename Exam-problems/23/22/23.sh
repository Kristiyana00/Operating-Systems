#!/bin/bash

USERS=$(mktemp)
cat /etc/passwd | cut -d : -f 1,6 | tr ':' ' ' > "${USERS}"

MOST_RECENT=$(mktemp)

while read name home; do
	if [ ! -d "${home}" ]; then
		#echo "$name: $home is not a directory!"
		continue
	fi

	if [ ! -r "${home}" ]; then
		continue
	fi

	cur_file="$(find "${home}" -type f -printf "%T@ %f\n" 2>/dev/null | sort -nr | head -n 1)"
	if [ ! -n "${cur_file}" ]; then
		continue
	fi
	echo "${name} ${cur_file}" >> "${MOST_RECENT}"
done < "${USERS}"

cat "${MOST_RECENT}" | sort -nr -k 2 | head -n 1 | cut -d ' ' -f 1,3

rm -- "${MOST_RECENT}"
rm -- "${USERS}"
