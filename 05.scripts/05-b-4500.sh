#!/bin/bash

echo "Enter UID: "
read userid

while [ false ]; do
	if who -u | grep -q "^$userid"; then
		echo "$userid is logged in."
		exit 0
	else 
		sleep 1
	fi
done
exit 0
