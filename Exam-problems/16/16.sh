#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

#if [ $(id -u) -ne 0 ]; then
#	echo "The script should be executed by the root!"
#	exit 2
#fi

users=$(ps -e -o user | tail -n +2 | sort | uniq)

for user in $users; do
	total_rss=0
	while read line; do
		current_pid=$(echo $line | awk '{print $1}')
		current_rss=$(echo $line | awk '{print $2}')
		total_rss=$(( $total_rss + $current_rss ))
	done < <(ps -u $user -o pid,rss | tail -n +2 | sort -n -k 2)

	echo "User: " $user "Total RSS: " $total_rss

	#if [ $total_rss -gt $1 ]; then
	#	echo "Total RSS is greater than the given argument!"
	#	kill -s SIGTERM $current_pid
	#	sleep 2
	#	kill -s SIGKILL $current_pid
	#fi
done

