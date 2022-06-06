#!/bin/bash

if [ ! $# -eq 1 ] && [ ! $# -eq 2 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "$1 is not a directory!"
	exit 2
fi

if [ $# -eq 2 ]; then
	#hard links
	num_links="$2"
	find "$1" -printf "%p %n\n" | while read line; do
	if [ $(echo ${line} | cut -d ' ' -f 2) -ge "${num_links}" ]; then
			echo "${line}"
	fi
	done
else
	find -L $1 -type l
fi
