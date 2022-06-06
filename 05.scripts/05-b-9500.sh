#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

STR="${2}"

if [ $1 = "-r" ]; then
	echo -e "\033[0;31m${STR}"
elif [ $1 = "-g" ]; then
	echo -e "\033[0;32m${STR}"
elif [ $1 = "-b" ]; then
	echo -e "\033[0;34m${STR}"
else
	echo "First argument is invalid!"
	exit 2
fi

echo -e '\033[0m'
