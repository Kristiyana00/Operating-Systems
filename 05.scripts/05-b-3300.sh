#!/bin/bash

echo "Enter 3 file names: "
read file1 file2 file3

paste -d '\n' $file1 $file2 | sort > $file3
exit 0
