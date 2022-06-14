#!/bin/bash

if [ $# -ne 3 ]; then
        echo "Invalid number of arguments."
        exit 1
fi

num="${1}"
pref_symb="${2}"
unit_symb="${3}"

if ! egrep -q ".*,$pref_symb,.*" ./prefix.csv; then
        echo "There is no $pref_symb found."
        exit 2
fi

if ! egrep -q ".*,$unit_symb,.*" ./base.csv; then
        echo "There is no $unit_symb found."
        exit 3
fi

DECIMAL=$(cat ./prefix.csv | egrep ".*,$pref_symb,.*" | cut -d ',' -f 3)

NUM_TO_PRINT=$(echo "$num * $DECIMAL" | bc)

INFO_TO_PRINT=$(cat ./base.csv | egrep ".*,$unit_symb,.*" | awk -F ',' '{print $3", "$1}')

echo "$NUM_TO_PRINT $unit_symb ($INFO_TO_PRINT)"
