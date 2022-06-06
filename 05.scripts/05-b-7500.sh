#!/bin/bash

num=$(( (RANDOM%200)-100 ))

cntr=0

read -p "Guess: " guess

function validate_guess 
{
	egrep -q '^[+-]?[0-9]+$' <(echo "${1}")
}

while true; do
	cntr=$((cntr+1))
	if validate_guess "${guess}"; then
		if [ "${guess}" -gt "${num}" ]; then
			echo "...smaller!"
		elif [ "${guess}" -lt "${num}" ]; then
			echo "...bigger!"
		else
			echo "RIGHT! Guessed "${num}" in "${cntr}" tries!"
			exit 0
		fi
	else
		echo "Not a valid guess!"
	fi

	read -p "Guess: " guess
done

