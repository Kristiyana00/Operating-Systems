#!/bin/bash

#if [ $(id -u) -ne 0 ]; then
#	echo "The script must be executed by the root!"
#	exit 1
#fi

cat /etc/passwd | while read line; do
	user=$(echo "${line}" | cut -d : -f 1)
	encrypted_pass=$(echo "${line}" | cut -d : -f 2)
	uid=$(echo "${line}" | cut -d : -f 3)
	gid=$(echo "${line}" | cut -d : -f 4)
	home=$(echo "${line}" | cut -d : -f 6)

	if [ -z "${home}" ]; then
		echo "$user has no homedir set"
		continue
	fi

	if [ ! -d $home ]; then
		echo "$user has homedir $home which is not a directory"
		continue
	fi

	dirperm=$(ls -ld "${home}" | awk '{print $1}')
	dirowner=$(ls -ld "${home}" | awk '{print $3}')

	if [ $dirowner != $user ]; then
		echo "$user is not owner of $home"
		continue
	fi

	if [ $(echo $dirperm | cut -c 3) != "w" ]; then
		echo "$user can't write in $home"
		continue
	fi
done

