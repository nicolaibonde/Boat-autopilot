#!/bin/bash
echo ---------------------------- >> server.log
date -u >> server.log
echo $$ >> server.log
echo "Checking for connection to wifi" >> server.log
running=0
nodepid=0
while :
do
	if [ "$running" == 0 ]
	then
		if ! iwconfig 2>&1 | grep -qi "off/any" 
		then
			echo "starting server" >> server.log
			nodejs /home/pi/Boat-autopilot/nodejs_server/init.js >> server.log &
			nodepid=$!
			echo "nodejspid: $nodepid" >> server.log
			running=1
		fi
	fi

	if iwconfig 2>&1 | grep -q off/any
	then
		if ! [[ $nodepid == 0 ]];
		then
			echo "killing pid: $nodepid" >> server.log
			sudo kill "$nodepid" >> server.log
			nodepid=0
		fi

		echo "server is not up" >> server.log
		running=0
		sudo ifdown --force wlan0 >> server.log
		sudo ifup wlan0 >> server.log
	fi
sleep 1
done

