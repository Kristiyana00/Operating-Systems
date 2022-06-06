#!/bin/bash

echo "Enter 3 numbers: "
read num left right

if ! [ "$num" -eq "$num" ] 2>/dev/null; then
	exit 3
elif ! [ "$left" -eq "$left" ] 2>/dev/null; then
	exit 3
elif ! [ "$right" -eq "$right" ] 2>/dev/null; then
	exit 3
fi

if [ "$left" -gt "$right" ]; then
	exit 2
fi

if [ "$num" -gt "$right" ] || [ "$num" -lt "$left" ]; then
	exit 1
fi

if [ "$num" -ge "$left" ] && [ "$num" -le "$right" ]; then
	exit 0
fi

exit 0

