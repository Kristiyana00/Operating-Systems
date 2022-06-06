#!/bin/bash

#1)
if [ $# -ne 1 ]; then
	echo "You should enter only one argument!"
	exit 1
fi

if (egrep -q "^[[:alnum:]]+$"< <(echo "$1")); then
	echo "Yes"
else
	echo "No"
fi

#2)

if [ ! $# -eq 1 ]; then
	echo "Incorrect number of arguments!"
	exit 1
fi

matches=1

while read line; do
	if echo "$line" | grep -q -E -v "^[a-zA-Z0-9]$"; then
		matches=0
	fi
done < <(echo "$1" | grep -o .)

if [ "$matches" -eq 1 ]; then
	echo "The argument contains only digits/letters."
else 
	echo "The argument doesn't contain only digits/letters."
fi
exit 0
