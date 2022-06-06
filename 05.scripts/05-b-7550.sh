#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

user="${1}"

if [ $(id -u "${user}") -eq 1 ]; then
	echo "Invalid username!"
	exit 2
fi

CNT=0

while read PID; do
	kill -15 "${PID}"
	sleep 1
	kill -9 "${PID}"
	CNT=$(($CNT+1))
done < <(ps -u "${user}" -o pid=)

echo "Total: ${CNT} killed processes."


