#!/bin/bash

echo "Enter a number and a separator: "
read num sep

if [ -z "$sep" ]; then
	echo $num | sed ':a;s/\B[0-9]\{3\}\>/ &/;ta'
	exit 0
fi

echo $num | sed ':a;s/\B[0-9]\{3\}\>/'$sep'&/;ta'

exit 0
