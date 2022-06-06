#!/bin/bash

if [ $# -ne 0 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

#if [ $(id -u) -ne 0 ]; then
#	echo "The script must be executed by the root!"
#	exit 2
#fi

TOTAL_ROOT_RSS="$(ps -u "root" -o rss= | awk '{s+=$1}END {print s}')"

#echo "${TOTAL_ROOT_RSS}"

while read USER_UID HOME_DIR; do
	if [ "${USER_UID}" -eq 0 ]; then
		continue
	fi

	[ ! -d "${HOME_DIR}" ] || [ "$(stat -c "%u" "${HOME_DIR}")" != "${USER_UID}" ] || [ "$(stat -c "%A" "${HOME_DIR}" | cut -c 3,3)" != "w" ] || continue

	TOTAL_USER_RSS="$(ps -u "${USER_UID}" -o rss= | awk '{s+=$1}END{print s}')"

	if [ ! -n "${TOTAL_USER_RSS}" ]; then
		TOTAL_USER_RSS=0
	fi
	
	if [ "${TOTAL_USER_RSS}" -gt "${TOTAL_ROOT_RSS}" ]; then
		echo "${USER_UID} has more RSS than ROOT: $TOTAL_USER_RSS vs $TOTAL_ROOT_RSS"
		#killall -u "${USER_UID}" -m .
		#sleep 2
		#killall -u "${USER_UID}" -KILL -m .
	fi
done < <(cat /etc/passwd | cut -d ':' -f 3,6 | tr ':' ' ')
