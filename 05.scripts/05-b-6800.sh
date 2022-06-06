#!/bin/bash

echo "Enter directory name: "
read dir

find $dir -maxdepth 1 -not -path '*/\.*' -type f -exec stat -c "%n %s" {} \; | awk -F "$dir/" '{print $2}' | awk '{print $1" " "("$2" bytes)"}'

find $dir -maxdepth 1 -not -path '*/\.*' -type d | while read -r dir2; do
printf "%s " "$dir2"; find "$dir2" -type f,d | wc -l; done | tail -n +2 | awk -F "$dir/" '{print $2}' | awk '{print $1" " "("$2" entries)"}'

exit 0
