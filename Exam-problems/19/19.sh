#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments given!"
	exit 1
fi

first=$(egrep "$1" ~/$1 | wc -l)
second=$(egrep "$2" ~/$2 | wc -l)

if [ $first -gt $second ]; then
	cut -d '-' -f 2 ~/$1 | sed 's/ //' | sort > $1.songs
elif [ $second -gt $first ]; then
	cut -d '-' -f 2 ~/$2 | sed 's/ //' | sort > $2.songs
else
	echo "There is no winner."
fi

exit 0
