#!/bin/bash

if [ $# -eq 0 ]
then 
	echo "No arguments! Please enter 4 arguments: <name of zip file> <non-existent dir name> <non-existent dir name> <non-existent file name>"
	exit 1
fi

if [ $# -ne 4 ]
then
	echo "Please enter 4 arguments: <name of zip file> <non-existent dir name> <non-existent dir name> <non-existent file name>"
	exit 2
fi

FILE_ARCHIVE=$(echo "${1}" | rev | cut -d '/' -f 1 | rev)
FILE_TO_TEST=$(find ~ -name "${FILE_ARCHIVE}")

if [ -e "$FILE_TO_TEST" ]
then
	if ( ! egrep -q 'Zip archive data' < <(file "${FILE_TO_TEST}") );
	then
		echo "First argument error: Enter a zip file!"
		echo "Please enter 4 arguments: <name of zip file> <non-existent dir name> <non-existent dir name> <non-existent file name>"
		exit 3
	fi
else
	echo "First argument error: Enter an existent zip file!"
	echo "Please enter 4 arguments: <name of zip file> <non-existent dir name> <non-existent dir name> <non-existent file name>"
	exit 4
fi

if [ -d $2 ]
then 
	echo "Second argument error: Enter a non-existent dir name!"
	echo "Please enter 4 arguments: <name of zip file> <non-existent dir name> <non-existent dir name> <non-existent file name>"
	exit 5
fi

if [ -d $3 ]
then 
	echo "Third argument error: Enter a non-existent dir name!"
	echo "Please enter 4 arguments: <name of zip file> <non-existent dir name> <non-existent dir name> <non-existent file name>"
	exit 6
fi

if [ -e $4 ]
then 
	echo "Fourth argument error: Enter a non-existent file name!"
	echo "Please enter 4 arguments: <name of zip file> <non-existent dir name> <non-existent dir name> <non-existent file name>"
	exit 7
fi

ARCHIVE="${FILE_TO_TEST}"
DIR="${2}"
FAILED_DIR="${3}"
FILE="${4}"

mkdir -p "${DIR}"
mkdir -p "${FAILED_DIR}"
touch "${FILE}"

unzip "${ARCHIVE}" -d "${DIR}" 2>/dev/null

find "${DIR}" -mindepth 1 -maxdepth 1 2>/dev/null | while read line; do 
	mv "${line}" $(dirname "${line}")/$(echo "${line}" | rev | cut -d '/' -f 1,1 | rev | cut -d '-' -f 1);
done

find "${DIR}" -mindepth 1 -maxdepth 1 -printf "%f\n" 2>/dev/null | sort -n | while read line; do 
	echo "${line}" >> "${FILE}"
done

GRI=$(mktemp)

while read line; do
	if echo $line | egrep -vq ".tar.xz$"; then
		echo "1" >> "${GRI}"
	elif [ $(dirname "${line}" | rev | cut -d '/' -f 1,1 |rev) != $(basename "${line}" | cut -d '.' -f 1,1)  ]; then
		echo "1" >> "${GRI}"
	else
		echo "0" >> "${GRI}"
	fi
done < <(find "${DIR}" -mindepth 2 -maxdepth 2 2>/dev/null | sort -n)

TOCOPY=$(mktemp)
paste -d ' ' "${FILE}" "${GRI}" > "${TOCOPY}"

mv "${TOCOPY}" "${FILE}"

GRF=$(mktemp)

while read line; do
	if egrep -q 'XZ compressed data' < <(file "${line}"); then
		echo "0" >> "${GRF}"
	else
		echo "1" >> "${GRF}"
	fi
done < <(find "${DIR}" -mindepth 2 -maxdepth 2 2>/dev/null | sort -n)

TOCOPY=$(mktemp)
paste -d ' ' "${FILE}" "${GRF}" > "${TOCOPY}"

mv "${TOCOPY}" "${FILE}"

while read line; do
	if egrep -q 'Zip archive data' < <(file "${line}"); then
		unzip -qq "${line}" -d $(dirname "${line}") 2>/dev/null
		if [ $? -ne 0 ]; then
			cp -r $(dirname "${line}") "${FAILED_DIR}"
		fi
	elif egrep -q 'RAR archive data' < <(file "${line}"); then
		unrar x -inul "${line}" $(dirname "${line}") 2>/dev/null
		if [ $? -ne 0 ]; then
			cp -r $(dirname "${line}") "${FAILED_DIR}"
		fi
	else
		tar -xf "${line}" -C $(dirname "${line}") 2>/dev/null
		if [ $? -ne 0 ]; then
			cp -r $(dirname "${line}") "${FAILED_DIR}"
		fi
	fi
done < <(find "${DIR}" -mindepth 2 -maxdepth 2 2>/dev/null | sort -n)

HD=$(mktemp)
GRD=$(mktemp)

while read line; do
	if [ $(find "${line}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | egrep '[0-9]+$' | wc -l) -eq 0 ]; then
		echo "1" >> "${HD}"
		echo "1" >> "${GRD}"
	else
		echo "0" >> "${HD}"
		parent=$(find "${line}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | cut -d '/' -f 2)
		child=$(find "${line}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | cut -d '/' -f 3)

		if [ "${parent}" == "${child}" ]; then
			echo "0" >> "${GRD}"
		else
			echo "1" >> "${GRD}"
		fi
	fi
done < <(find "${DIR}" -mindepth 1 -maxdepth 1 2>/dev/null | sort -n)

TOCOPY=$(mktemp)
paste -d ' ' "${FILE}" "${HD}" > "${TOCOPY}"

mv "${TOCOPY}" "${FILE}"

TOCOPY=$(mktemp)
paste -d ' ' "${FILE}" "${GRD}" > "${TOCOPY}"

mv "${TOCOPY}" "${FILE}"

exit 0
