#!/bin/bash

FQDN="([a-z0-9]+\.)+"
TTL="[0-9]*"
#purviqt red  SOA

SOA="$FQDN\s+$TTL\s+IN\s+SOA\s+$FQDN\s+$FQDN"
#SOAS - single
SOAS="$SOA\s+([0-9]+\s+){5}"
#SOAM - mult
SOAM="$SOA\s+\("

for f ; do
	h=$(head -n 1 $f)
	
	r=$(echo "$h" | egrep "$SOAS|$SOAM" | wc -l)

	if [ "$r" -eq 0 ]; then
		echo "$f : problem."
		continue
	fi

	r=$(echo "$h" | egrep "$SOAS" | wc -l)

	if [ $r - eq 1 ]; then
		# SN - serial number
		SN=$(echo "$h" | rev | awk '{print $5}' | rev )
	else 
		SN=$(head -n 2 "$f" | tail -n 1 | cut -d ';' -f 1)
	fi

	Y=$(echo "$SN" | cut -c 1-8)
	N=$(echo "$SN" | cut -c 9-10)
	D=$(date +%Y%m%d)
	if [ $N -eq 99 ]; then
		echo "$f : 99 mods."
		continue;
	fi

	if [ $Y -lt $D ]; then
		#new serial number
		NSN="{$D}00"
	else
		M=$(( $N + 1 ))
		NSN="${Y}$(printf '%02d' $M)"
	fi

	sed -E -i "s/$SN/$NSN/" $f

done

exit 0
