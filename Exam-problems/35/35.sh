#!/bin/bash

if [ $# -ne 1 ]; then
        echo "Invalid number of arguments."
        exit 1
fi

if [ ! -d $1 ]; then
        echo "$1 is not a directory."
        exit 2
fi

if [ ! -r $1 ]; then
        echo "$1 is not readable."
        exit 3
fi

RESULT=$(find $1 -printf "%p %T@\n" 2>/dev/null | egrep ".tgz" | egrep "[a-zA-Z0-9]+_report\-[0-9\.]+\.tgz" | sort -nr -k 2 | head -n 1 | cut -d ' ' -f 1)

if [ -n "$(tar -tvf $RESULT 'meow.txt')" ]; then
        mkdir extracted
        tar -zxvf $RESULT meow.txt
        NAME=$(echo $RESULT | sed -E 's/report\-//' | sed -E 's/\.tgz//')
        mv meow.txt ./extracted/$NAME.txt
fi
