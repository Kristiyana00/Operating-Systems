#!/bin/bash

if [ $(id -un) != "oracle" ] && [ $(id -un) != "grid" ]; then
	echo "The script should be executed by oracle or grid."
	exit 1
fi

if [ -z "$ORACLE_HOME" ]; then
	echo "ORACLE_HOME is empty string."
	exit 2
fi

ADRCI="$ORACLE_HOME/bin/adrci"

if [ ! -f "$ADRCI" ]; then
	echo "There is no file adrci."
	exit 3
fi

DIAG_DEST="/u01/app/$(id -un)"

if [ ! -d "$DIAG_DEST" ]; then
	echo "There is no such dir."
	exit 4
fi


K="No ADR homes are set"

#Tuka ne tr da exit-va mai a da izvejda suobshtenie
M=$($ADRCI exec="show homes" | grep "$K" | wc -l)

if [ "$M" -gt 0 ]; then
	exit 5
fi

for a in $($ADRCI exec="show homes" | tail -n +2); do
	#DIAG_DEST/$a
	du -sm $DIAG_DEST/$a
done
