#!/bin/sh

function main() {
	sudo killall -9 dnsmasq
	sudo ap-hotspot stop
	sudo ap-hotspot start
}

main >/home/xiaofei/wifi.log 2>&1
