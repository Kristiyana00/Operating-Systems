#!/bin/bash

#1)
echo "Enter filename and string: "
read file string

grep -q "$string" $file && echo "yes" || echo "no"
#exit 0

#2)
read -p "Enter full path name of file: " FILE_NAME
read -p "Enter string to match: " EXP

grep -qsF "${EXP}" "${FILE_NAME}" && echo "yes" || echo "no"
exit 0
