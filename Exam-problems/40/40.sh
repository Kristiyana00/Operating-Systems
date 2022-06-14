#!/bin/bash

if [ $# -ne 3 ]; then
        echo "Invalid number of arguments."
        exit 1
fi

if [ ! -f "${1}" ]; then
        echo "$1 is not a file."
        exit 2
fi

if [ ! -r "${1}" ]; then
        echo "$1 is not readable."
        exit 3
fi

if [ ! -e "${2}" ]; then
        echo "$2 is not a file."
        exit 4
fi

if [ ! -d "${3}" ]; then
        echo "$3 is not a directory."
        exit 6
fi

touch $2

while read file; do
        if [ $(cat "${file}" | awk '{print "Line " NR ":" $s }' | egrep -v "^Line [[:digit:]]+:#" | egrep -v "^Line [[:digit:]]+:{ .* };$" | egrep -v "^Line [[:digit:]]+:$" | wc -l) -ne 0 ]; then
                echo "Error in $(basename $file):"

                while read line; do
                        echo "${line}"
                done < <(cat $file | awk '{ print "Line " NR ":" $s}' | egrep -v "^Line [[:digit:]]+:#" | egrep -v "^Line [[:digit:]]+:{ .* };$" | egrep -v "^Line [[:digit:]]+:$")
        else
                cat $file >> $2
        fi
done < <(find "${3}" | egrep ".cfg$")

cat $1 | if ! egrep -q "^$(basename $2 | sed 's/.cfg//'):"; then
        pass="$(pwgen 16 1)"

        md5="$(echo -n $pass | md5sum | awk '{print $1}')"

        echo $(basename $2 | sed 's/.cfg//'):$md5 >> $1
        echo $(basename $2 | sed 's/.cfg//') $pass
fi
