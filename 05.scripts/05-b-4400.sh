#!/bin/bash

echo "Enter directory and name: "
read dir name

if [ ! -z "$name" ]; then
	mkdir ./$name
	find $dir -type f -mmin -45 -exec cp {} ./$name/ \;
else 
	mkdir ./$(date +"%d-%m-%Y")
	find $dir -type f -mmin -45 -exec cp {} ./$(date +"%d-%m-%Y") \;
fi

exit 0
