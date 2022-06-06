#!/bin/bash

#1)
echo "Please enter full dir name: "
read dir

echo "Number of all files and directories in ${dir} is: $(find ${dir} -type f,d 2> /dev/null | wc -l)."

#2)
echo "Please enter full dir name: "
read line

find $line -type f,d 2>&1 | echo "Number of files and directories: $(wc -l)"
exit 0
