#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

#if [ $(id -u) -ne 0 ]; then
#	echo "The script must be executed as root!"
#	exit 2
#fi

if [ ! -d "$1" ]; then
	echo "$1 is not a directory!"
	exit 3
fi

if [ ! -d "$2" ]; then
	echo "$2 is not a directory!"
	exit 3
fi

if [ $(find "${2}" -type f | wc -l) -ne 0 ]; then
	echo "$2 contains files!"
	exit 4
fi

SRC=$(dirname "${1}")"/"$(basename "${1}")
DST=$(dirname "${2}")"/"$(basename "${2}")
ABC="${3}"

for i in $(find "${SRC}" -type f -name "*${ABC}*"); do
	DSTFILE=$(echo ${i} | sed -e "s|${SRC}|${DST}|")
	mkdir -p $(dirname $DSTFILE)
	cp ${i} ${DSTFILE}
done

