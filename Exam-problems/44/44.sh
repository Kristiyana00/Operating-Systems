#!/bin/bash

nums_file=$(mktemp)

xxd foo.bin | cut -c 11-49 | tr ' ' '\n' | tr -s '\n' > "${nums_file}"

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
