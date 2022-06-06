#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

user="${1}"

if ! id "${user}" 2>/dev/null; then
	echo "Invalid username!"
	exit 2
fi

ps -u "${user}" -o pid,rss,vsz | tail -n +2 | while read PID RSS VSZ; do
	if [ $VSZ -eq 0 ]; then
		proportion="inf"
	else
		proportion=$(echo "scale=4; $RSS/$VSZ" | bc)
	fi

	echo "${VSZ} ${PID} consume ${proportion} of RSS/VSZ memory!"
done | sort -rn -t ' ' -k 1 | cut -d ' ' -f 2-
