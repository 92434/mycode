#!/bin/sh

#sudo lsof -p $(ps -elf | grep adt | grep eclipse | awk '{print $4}') -P -n

ips="
8.8.8.8
14.215.177.37
"

function set_iptables {
	case "$1" in
		start)
			for ip in $ips; do
				sudo iptables -t filter -A OUTPUT -d $ip/32 -p tcp --dport 80 -j REJECT
			done
			;;
		stop)
			for ip in $ips; do
				sudo iptables -t filter -D OUTPUT -d $ip/32 -p tcp --dport 80 -j REJECT
			done
			;;
		*)
			;;
	esac
}

set_iptables $@
