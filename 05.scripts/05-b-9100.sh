#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

SRC="${1}"
DEST="${2}"

if [ ! -d "${SRC}" ]; then
	echo "Invalid source directory!"
	exit 2
elif [ ! -r "${SRC}" ]; then
	echo "Source directory is not readable!"
	exit 3
fi

if [ ! -d "${DEST}" ]; then
	echo "Invalid destination directory!"
	exit 4
elif [ ! -r "${DEST}" ]; then
	echo "Destination directory is not readable!"
	exit 5
fi

while read ext; do
	mkdir -p "${DEST}/${ext}"
	find "${SRC}" -type f -name "*.${ext}" 2>/dev/null -print0 | xargs -0 -I {} cp {} "${DEST}/${ext}"
done < <(find "${SRC}" -type f -name "*.*" -printf "%f\n" | rev | cut -d '.' -f 1 | rev | sort | uniq)
