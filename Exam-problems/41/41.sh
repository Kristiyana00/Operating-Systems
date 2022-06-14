#!/bin/bash

if [ $# -ne 3 ]; then
        echo "Invalid number of arguments given."
        exit 1
fi

if [ ! -f "${1}" ]; then
        echo "$1 is not a file."
        exit 2
fi

cat "${1}" | egrep -v "^#" | if ! egrep -q "$2[ ]*="; then
        echo "${2} = ${3} # added at $(date) by $(whoami)" >> $1
elif ! egrep -q "$2[ ]*=[ ]*$3"; then
        old_row=$(cat "${1}" | grep ".*$2[ ]*=.*")
        edited="$(echo "# $old_row # edited at $(date) by $(whoami)")"
        newline="$(echo "${2} = ${3} # added at $(date) by $(whoami)")"
        sed -i "s|$old_row|$edited\n$newline|" "${1}"
fi
