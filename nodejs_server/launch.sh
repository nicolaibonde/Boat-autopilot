#!/bin/bash
mkdir /var/log/CAPTAIN
echo ---------------------------- >> /var/log/CAPTAIN/server.log
date -u >> /var/log/CAPTAIN/server.log
pid=$$
echo "Launch script pid: $pid" >> /var/log/CAPTAIN/server.log
echo "Checking for changes on the git repo" >> /var/log/CAPTAIN/server.log
git -C /home/pi/Boat-autopilot/ pull >> /var/log/CAPTAIN/server.log
echo "Checking for connection to WiFi" >> /var/log/CAPTAIN/server.log
running=0
nodepid=0
while :
do
	if [ "$running" == 0 ]
	then
		if ! iwconfig 2>&1 | grep -qi "off/any"
		then
			echo "Starting server" >> /var/log/CAPTAIN/server.log
			nodejs /home/pi/Boat-autopilot/nodejs_server/init.js >> /var/log/CAPTAIN/server.log &
			nodepid=$!
			echo "nodejs pid: $nodepid" >> /var/log/CAPTAIN/server.log
			running=1
		fi
	fi

	if iwconfig 2>&1 | grep -q off/any
	then
		if ! [[ $nodepid == 0 ]];
		then
			echo "killing pid: $nodepid" >> /var/log/CAPTAIN/server.log
			sudo kill "$nodepid" >> /var/log/CAPTAIN/server.log
			nodepid=0
		fi

		echo "Server is not up" >> /var/log/CAPTAIN/server.log
		running=0
		sudo ifdown --force wlan0 >> /var/log/CAPTAIN/server.log
		sudo ifup wlan0 >> /var/log/CAPTAIN/server.log
	fi
sleep 1
done
