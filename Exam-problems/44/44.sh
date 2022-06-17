#!/bin/bash

if [ $# -ne 2 ]; then
        echo "error"
        exit 1
fi

if [ ! -f "${1}" ]; then
        echo "error."
        exit 2
fi

if [ ! -r "${2}" ]; then
        echo "error"
        exit 3
fi

touch $2

nums_file=$(mktemp)

xxd "${1}" | cut -c 11-49 | tr ' ' '\n' | tr -s '\n' > "${nums_file}"

num_nums="$(cat "${nums_file}" | wc -l)"

echo "const uint32_t arrN = ${num_nums};" > "${2}"
echo "const uint16_t arr[] = {" >> "${2}"

for i in $(cat "${nums_file}"); do
        left="$(echo "${i}" | cut -c1-2)"
        right="$(echo "${i}" | cut -c3-4)"
        echo "    0x${right}${left}," >> "${2}"
done

echo "};" >> "${2}"

rm "${nums_file}"
