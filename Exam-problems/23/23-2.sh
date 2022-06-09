#!/bin/bash

while read u h; do
        find $h -type f -printf"$u %p %T@\n"

done < <(cut -d ':' -f 1,6 /etc/passwd | tr ':' ' ') | sort -nr -k 3 | head -n 1 | awk '{print $1, $2}'
