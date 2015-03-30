#!/bin/sh

i=1
while true;do
	./out/bin/tcp_client 192.168.1.210 <<EOF      
$i
EOF
	let i=$i+1
	sleep 0.01
done
