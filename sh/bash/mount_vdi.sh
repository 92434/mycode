#!/bin/sh

#用qemu-nbd实现mount虚拟硬盘到Host上的功能，需要这么做：
#rmmod nbd
#modprobenbd max_part=8

function load_vdi() {
	local num="$1"
	local vdi="$2"
	echo "qemu-nbd -c /dev/nbd$num $vdi"
	sudo qemu-nbd -c /dev/nbd$num "$vdi"
}

function load_vdis() {
	local i=0
	local vdi
	for vdi in /media/xiaofei/DATA/Users/xiaofei/Documents/VirtualBox\ VMs/Ubuntu12.04/*.vdi;do 
		load_vdi "$i" "$vdi"

		((i++));
	done
	for vdi in /media/xiaofei/DATA/Users/xiaofei/Documents/VirtualBox\ VMs/WinXP/winxp/*.vdi;do 
		load_vdi "$i" "$vdi"

		((i++));
	done
}

function mount_vdi() {
	for i in  /dev/nbd*p*;do mkdir -p $(basename $i);done
	for i in  /dev/nbd*p*;do sudo mount $i $(basename $i);done
}

function main() {
	load_vdis
	sleep 5
	mount_vdi
}

main
