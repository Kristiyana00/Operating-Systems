#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

DIR="${1}"

if [ ! -d "${DIR}" ]; then
	echo "Argument is not a directory!"
	exit 2
fi

if [ ! -r "${DIR}" ]; then
	echo "Directory has no read permissions!"
	exit 3
fi

HASHES=$(mktemp)

find "${DIR}" -type f -maxdepth 1 -exec md5sum {} 2>/dev/null \; | sort > "${HASHES}"

cat "${HASHES}" | cut -d ' ' -f 1 | uniq -d | while read DUPLICATED_HASH; do
	grep "${DUPLICATED_HASH}" "${HASHES}" | awk '{print $2}' | tail -n +2 | xargs rm --
done

rm "${HASHES}"
