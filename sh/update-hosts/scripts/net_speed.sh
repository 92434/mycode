#!/bin/sh
#netstat -nap

if [ -z $1 ];then
	interface=eth0
fi

interface=$1

function update_info() {
	info=($(ifconfig $interface | grep -o "RX bytes\:\([^ ]\+\).*TX bytes\:\([^ ]\+\).*" | sed 's/RX bytes\:\([^ ]\+\).*TX bytes\:\([^ ]\+\).*/\1 \2/g'))
}

function get_received() {
	echo ${info[0]}
}

function get_send() {
	echo ${info[1]}
}

update_info
now_received=$(get_received)
last_received=$(get_received)
now_send=$(get_send)
last_send=$(get_send)

while ((1));do
	update_info
	now_received=$(get_received)
	now_send=$(get_send)

	received=$(($now_received - $last_received))
	send=$(($now_send - $last_send))

	received_M=$(($received/(1024*1024)))
	received_M_left=$(($received%(1024*1024)))
	received_K=$(($received_M_left/1024))
	received_K_left=$(($received%1024))
	received_B=$(($received_M_left%1024))

	send_M=$(($send/(1024*1024)))
	send_M_left=$(($send%(1024*1024)))
	send_K=$(($send_M_left/1024))
	send_K_left=$(($send%1024))
	send_B=$(($send_M_left%1024))

	rx_str=$(printf "RX:%dM,%dK,%dB" $received_M $received_K $received_B)
	tx_str=$(printf "TX:%dM,%dK,%dB" $send_M $send_K $send_B)
	printf "%-16s%-16s\r" $tx_str $rx_str

	last_received=$now_received
	last_send=$now_send

	sleep 1
done
