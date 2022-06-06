#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "The argument is not a file!"
	exit 2
fi

cat $1 | cut -d '-' -f 2- | sed 's/ //' | cat -n | tr -s ' ' | sed 's/ //' | sed 's/\t/. /' | sort -t '.' -k 2
