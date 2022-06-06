#!/bin/bash

#1)
echo "Please enter your username: "
read username

echo "Number of ${username}'s active sessions is: $(who | egrep -o ${username} | wc -l)."

#2)
echo "Please enter your username: "
read line

who | grep "^$line" | wc -l

exit 0
