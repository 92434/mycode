#!/bin/sh

#sudo lsof -p $(ps -elf | grep adt | grep eclipse | awk '{print $4}') -P -n

ips="
8.8.8.8
"

function set_iptables {
	case "$1" in
		start)
			for ip in $ips; do
				sudo iptables -t filter -A INPUT -d $ip/32 -p tcp --dport 80 -j REJECT
			done
			;;
		stop)
			for ip in $ips; do
				sudo iptables -t filter -D INPUT -d $ip/32 -p tcp --dport 80 -j REJECT
			done
			;;
		*)
			;;
	esac
}

set_iptables $@
