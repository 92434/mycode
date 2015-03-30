#!/bin/bash

cnt=0

echo $cnt
busybox mount -o rw,remount /misc
busybox mount -o rw,remount /system
#busybox cp /system/etc/wifi/wpa_supplicant.conf /data/misc/wifi/
#insmod /system/lib/wifi/modules/8192cu.ko
#insmod /misc/modules/wlan.ko

#busybox ifconfig wlan0 up

#/system/bin/wpa_supplicant -Dwext -iwlan0 -c/data/misc/wifi/wpa_supplicant.conf &
#/system/bin/wpa_supplicant -Dnl80211 -iwlan0 -c/data/misc/wifi/wpa_supplicant.conf -B

echo $#
sleep 3
#if [ $# -eq 1 ] 
#then 
	echo "into"
	/system/bin/wpa_cli -iwlan0 scan
	/system/bin/wpa_cli -iwlan0 scan_result
	/system/bin/wpa_cli -iwlan0 add_network
	/system/bin/wpa_cli -iwlan0 set_network $cnt ssid "xf22001"
	/system/bin/wpa_cli -iwlan0 set_network $cnt key_mgmt NONE
	/system/bin/wpa_cli -iwlan0 set_network $cnt wep_key0 "22001"	
	/system/bin/wpa_cli -iwlan0 set_network $cnt wep_tx_keyidx 0
	/system/bin/wpa_cli -iwlan0 select_network $cnt
	/system/bin/wpa_cli -iwlan0 enable_network $cnt	
	/system/bin/busybox ifconfig wlan0 192.168.191.160
#fi

echo "end"

sudo wpa_cli add_network
id=0
sudo wpa_cli set_network $id ssid "xf22001"
sudo wpa_cli set_network $id key_mgmt NONE
sudo wpa_cli set_network $id wep_key0 "22001"
sudo wpa_cli set_network $id wep_tx_keyidx 0
sudo wpa_cli enable_network $id
sudo dhclient wlan2


^\s*#\s*CONFIG_
