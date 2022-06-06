#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

FIRST_ARG="${1}"
FLAG_RECUR=0

if [ "${FIRST_ARG}" = -r ]; then
	FLAG_RECUR=1
	shift 1
fi

export LOGFILE=$(mktemp)

for i; do
	if [ -d "${i}" ]; then
		DIR_CONTENT=$(find "${i}" -mindepth 1 | wc -l | awk '{$1=$1}1')
		if [ "${DIR_CONTENT}" -eq 0 ]; then
			echo "[$(date +"Y-%m-%d %H:%M:%S")] Removed directory ${i}/" >> "${LOGFILE}"
			rm -di "${i}"
		elif [ "${FLAG_RECUR}" -eq 1 ]; then
			echo "[$(date +"Y-%m-%d %H:%M:%S")] Removed directory recursively ${i}/" >> "${LOGFILE}"
			rm -Rdi "${i}"
		fi
	elif [ -f "${i} ]; then
		echo "[$(date +"Y-%m-%d %H:%M:%S")] Removed file ${i}" >> "${LOGFILE}"
		rm -di ${i}
	fi
done

echo "Log file info: "
cat ${LOGFILE}
