#!/bin/bash

#if [ $(id -u) -ne 0 ]; then
#	echo "The script must be executed by the root!"
#	exit 1
#fi

for user in $(ps -e -o user= | sort | uniq); do
	TOTAL_RSS=0
	PS_CNT=0

	while read CPID CRSS; do
		PS_CNT=$(expr $PS_CNT + 1)
		TOTAL_RSS=$(expr $TOTAL_RSS + $CRSS)
		MAX_RSS=${CRSS}
		MAX_RSS_PID=${CPID}
	done < <(ps -u "${user}" -o pid=,rss= | sort -n -k 2)

	if [ ${PS_CNT} -eq 0 ]; then
		continue
	fi

	AVG_RSS=$(expr ${TOTAL_RSS} '/' ${PS_CNT})
	echo "User:" ${user} "Number of processes: "${PS_CNT} "Total RSS: "$TOTAL_RSS

	if [ ${MAX_RSS} -gt $(expr ${AVG_RSS} '*' 2) ]; then
		echo "Killing process ${MAX_RSS_PID}"
		#kill -s SIGTERM ${MAX_RSS_PID}
		#sleep 2
		#kill -s SIGKILL ${MAX_RSS_PID}
	fi	
done
