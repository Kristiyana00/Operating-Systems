#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Invalid number of arguments."
        exit 1
fi

if [ ! -d "${1}" ]; then
        echo "$1 is not a directory."
        exit 2
fi

if [ ! -r "${1}" ]; then
        echo "$1 is not readable."
        exit 3
fi

if [ ! -d "${2}" ]; then
        echo "$2 is not a directory."
        exit 4
fi

if [ ! -w "${2}" ]; then
        echo "$2 is not writable."
        exit 5
fi

if [ $(find "${2}" -mindepth 1 2>/dev/null | wc -l) -ne 0 ]; then
        echo "$2 should be empty."
        exit 6
fi

SRC="${1}"
DST="${2}"

for i in $(find "${SRC}" -type f 2>/dev/null); do
        if [ $(echo $(basename $i) | egrep "^\..*\.swp$" | wc -l) -ne 0 ]; then
                tocheck=$(echo $(basename $i) | cut -d '.' -f 2- | rev | cut -d '.' -f 2- | rev)
                if [ $(find $(dirname $i) -maxdepth 1 -name "${tocheck}" | wc -l) -ne 0 ]; then
                        continue
                fi
        fi
        DSTFILE=$(echo ${i} | sed -e "s|${SRC}|${DST}|")
        mkdir -p $(dirname $DSTFILE)
        cp ${i} ${DSTFILE}
done
