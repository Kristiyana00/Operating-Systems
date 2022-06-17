#!/bin/bash

FQDN="([a-z0-9]+\.)+"
TTL="[0-9]*"

SOA="$FQDN\s+$TTL\s+IN\s+SOA\s+$FQDN\s+$FQDN"

SOAS="$SOA\s+([0-9]+\s+){4}[0-9]+"
SOAM="$SOA\s+\("

for line ; do
        header=$(cat $line | head -n 1)

        check=$(echo $header | egrep "${SOAS}|${SOAM}" | wc -l)

        if [ $check -eq 0 ]; then
                echo "$line: problem."
                continue
        fi

        if [ $(echo $header | egrep "$SOAS" | wc -l) -eq 1 ]; then
                SN=$(echo $header | rev | awk '{print $5}' | rev)
        elif [ $(echo $header | egrep "$SOAM" | wc -l) -eq 1 ]; then
                SN=$(cat $line | head -n 2 | tail -n 1 | cut -d ';' -f 1 | tr -d ' ')
        fi

        Y=$(echo $SN | cut -c 1-8)
        N=$(echo $SN | cut -c 9-10)
        D=$(date +%Y%m%d)

        if [ $N -eq 99 ]; then
                echo "$line: 99 mods."
                continue
        fi

        if [ $Y -lt $D ]; then
                NSN="${D}00"
        else
                tochange=$(expr $N + 1)
                NSN="${Y}$(printf '%02d' $tochange)"
        fi

        sed -E -i "s|$SN|$NSN|" $line
done
