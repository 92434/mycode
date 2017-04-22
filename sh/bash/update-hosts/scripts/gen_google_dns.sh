#!/bin/sh

google_dns=$(nslookup -q=TXT _netblocks.google.com 8.8.4.4 | grep -o "ip4:[^ ]\+" | awk 'FS=":" {print $2}')

for i in $google_dns; do
	./enum_server_ip.sh "$i" >>google_dns
done
