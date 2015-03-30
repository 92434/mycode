#!/bin/sh
set -x

test_flashcp(){
	local i=1
	while [ $i -gt 0 ]
	do
		echo -en "\xAA" >>aa.bin
		let i=$i-1
	done

	#cat aa.bin|hexdump -vc

	flashcp aa.bin /dev/mtd3
	dd bs=1 if=/dev/mtd3 skip=0 count=1 of=mtd3.bin seek=0 conv=fsync,notrunc

 
	cmp aa.bin mtd3.bin
	if [ $? = 0 ]; then
		echo "successed!"
	else
		echo "failed!"
	fi
	rm aa.bin
	rm mtd3.bin
}

#echo "test_flashcp $(test_flashcp 2>/dev/null)"

get_mtd_dev(){
	if [ -z "$1" ]; then
		echo "get_mtd_dev:no mtd dev label!"
		exit
	fi
	while read line
	do
		local LABEL=
		LABEL=$(echo "$line" | awk '{print $4}')
		LABEL=$(echo "$LABEL" | sed 's/"//g')
		#echo -en "$LABEL"|hexdump -vc

		if [ "$LABEL" == "$1" ]; then
			LABEL=$(echo "$line" | awk 'BEGIN{FS=":"} {print $1}')
			local TARGET_MTD_DEV="/dev/$LABEL"
			if [ -c "$TARGET_MTD_DEV" ]; then
				echo "$TARGET_MTD_DEV"
			fi
		fi
	done < /proc/mtd
}

#echo $(get_mtd_dev "swap")

flash_firmware(){
	if [ ! -f "$1" ]; then
		echo "flash_firmware:no firmware"
		exit
	fi

	if [ -z "$2" ]; then
		echo "flash_firmware:no mtd dev label"
		exit
	fi
	
	local DEV="$(get_mtd_dev "$2")"
	if [ ! -c "$DEV" ]; then
		echo "flash_firmware:no mtd dev"
		exit
	fi

	flashcp "$1" "$DEV"
	echo "flush $1 into $DEV"
}

#echo $(flash_firmware "aa.bin" "swap")

get_disk_partition_info(){
	if [ ! -b "$1" ]; then
		echo "get_disk_partition_info:no block device"
		exit
	fi
	fdisk "$1" << EOF
p
q
EOF
}


get_disk_all_partition_number(){
	if [ ! -b "$1" ]; then
		echo "get_disk_partition_number:no block device"
		exit
	fi
	get_disk_partition_info "$1" >tmp.bin
	local PARTITION_NUMBERS=""
	while read line
	do
		local PARTITION_INFO=
		PARTITION_INFO=$(echo "$line" | grep -o -E "^[^\s]+\s+[0-9]+\s+[0-9]+\s+[0-9]+\s+[0-9]+\s+[^\s]+$")
		PARTITION_INFO=$(echo "$PARTITION_INFO" | awk '{print $1}')
		if [ -z "$PARTITION_INFO" ]; then
			continue
		fi
		PARTITION_INFO=$(echo "$PARTITION_INFO" | grep -o -E "[0-9]+$")
		PARTITION_NUMBERS=${PARTITION_NUMBERS:+"$PARTITION_NUMBERS "}"$PARTITION_INFO"
	done < tmp.bin
	rm tmp.bin
	echo $PARTITION_NUMBERS
}

#echo $(get_disk_all_partition_number "/dev/mmcblk0")

get_disk_all_cylinders(){
	if [ ! -b "$1" ]; then
		echo "get_disk_all_cylinders:no block device"
		exit
	fi
	get_disk_partition_info "$1" >tmp.bin
	while read line
	do
		local PARTITION_INFO=
		PARTITION_INFO=$(echo "$line" | grep -o -E "[0-9]+\s+cylinders$")
		if [ -z "$PARTITION_INFO" ]; then
			continue
		fi
		echo "$(echo "$PARTITION_INFO" | grep -o -E "[0-9]+")"
	done < tmp.bin
	rm tmp.bin
}

#echo $(get_disk_all_cylinders "/dev/mmcblk0")

del_disk_partition(){
	if [ ! -b "$1" ]; then
		echo "del_disk_partition:no block device"
		exit
	fi
	if [ -z "$2" ]; then
		echo "del_disk_partition:no partition number"
		exit
	fi
	
	fdisk "$1" << EOF
d
$2
w
EOF
}

#del_disk_partition "/dev/mmcblk0" "1"
#del_disk_partition "/dev/mmcblk0" "2"

add_disk_partition(){
	if [ ! -b "$1" ]; then
		echo "add_disk_partition:no block device"
		exit
	fi
	if [ -z "$2" ]; then
		echo "del_disk_partition:no partition number"
		exit
	fi
	
	local START_CYLINDER=
	local END_CYLINDER=
	case "$2" in
	1)
		let END_CYLINDER=$(get_disk_all_cylinders "$1")/8
		;;
	2)
		;;
	esac
	
	fdisk "$1" << EOF
n
p
$2
$START_CYLINDER
$END_CYLINDER
w
EOF
}

#add_disk_partition "/dev/mmcblk0" "1"
#add_disk_partition "/dev/mmcblk0" "2"


make_disk_fs(){
	if [ ! -b "$1" ]; then
		echo "make_disk_fs:no block device"
		exit
	fi
	if [ -z "$2" ]; then
		echo "make_disk_fs:no fs type"
		exit
	fi
	
	mkfs."$2" "$1"
}

#make_disk_fs "/dev/mmcblk0p1" "ext3"
#make_disk_fs "/dev/mmcblk0p2" "ext3"

un_compress_firmware(){
	if [ ! -f "$1" ]; then
		echo "un_compress_firmware:no firmware"
		exit
	fi
	if [ -z "$2" ]; then
		echo "un_compress_firmware:no firmware dir"
		exit
	fi
	
	tar -zxf $1 -C "$2"

	chown -R 0:0 "$2"
}

flash_uldr(){
	local ULDR_BIN="uldr.bin"
	local ULDR_LABEL="uldr"
	if [ ! -f "$1/$ULDR_BIN" ]; then
		echo "flash_uldr:no uldr bin"
		exit
	fi
	
	echo $(flash_firmware "$1/$ULDR_BIN" "$ULDR_LABEL")
}

flash_uboot(){
	local UBOOT_BIN="u-boot.bin"
	local UBOOT_LABEL="u-boot"
	if [ ! -f "$1/$UBOOT_BIN" ]; then
		echo "flash_uboot:no uboot bin"
		exit
	fi
	
	echo $(flash_firmware "$1/$UBOOT_BIN" "$UBOOT_LABEL")
}

flash_kernel(){
	local KERNEL_BIN="zImage"
	local KERNEL_LABEL="kernel"
	if [ ! -f "$1/$KERNEL_BIN" ]; then
		echo "flash_kernel:no zImage bin"
		exit
	fi
	
	echo $(flash_firmware "$1/$KERNEL_BIN" "$KERNEL_LABEL")
}

flash_mark(){
	
	local MARK_BIN="mark.bin"
	local MARK_LABEL="sfc_extra"
	
	echo -ne "\x55" > "$MARK_BIN"
	echo $(flash_firmware "$MARK_BIN" "$MARK_LABEL")
	rm $MARK_BIN
}


flash_unmark(){
	
	local MARK_BIN="mark.bin"
	local MARK_LABEL="sfc_extra"
	
	echo -ne "\x00" > "$MARK_BIN"
	echo $(flash_firmware "$MARK_BIN" "$MARK_LABEL")
	rm $MARK_BIN
}

format_disk(){
	
	local DISK="$1"
	if [ ! -b "$DISK" ]; then
		echo "format_disk:no $DISK"
		exit
	fi

	del_disk_partition "$DISK" "1"
	del_disk_partition "$DISK" "2"
	add_disk_partition "$DISK" "1"
	add_disk_partition "$DISK" "2"
	echo "format_disk: partition done!"
	sleep 3

	make_disk_fs "$DISK""p1" "ext3"
	make_disk_fs "$DISK""p2" "ext3"
}
#format_disk "/dev/mmcblk0"

do_rootfs(){
	if [ ! -d "$1" ]; then
		echo "do_rootfs: no firmware dir"
	fi

	local FIRMWARE_DIR="$1"
	local ROM_DIR=$(dirname "$1")
	local ROOTFS_AR="$FIRMWARE_DIR/rootfs.tar.gz"
	local ROOTFS_DIR="$FIRMWARE_DIR/rootfs"

	rm -rf "$ROOTFS_DIR"
	mkdir -p "$ROOTFS_DIR"

	[ ! -f "$ROOTFS_AR" ] && echo "upgrade:no rootfs data!!!" && exit

	un_compress_firmware "$ROOTFS_AR" "$ROOTFS_DIR"

	################backup
	################backup

	################clear
	################clear

	################copy
	for i in $(ls -A "$ROM_DIR")
	do
		if [ "$ROM_DIR/$i" == "$FIRMWARE_DIR" ]; then
			continue
		fi
		rm -rf "$ROM_DIR/$i"
	done

	for i in $(ls -A "$ROOTFS_DIR")
	do
		mv "$ROOTFS_DIR/$i" "$ROM_DIR/"
	done
	################copy
}

finaly(){
	#if [ -f "$1" ]; then
	#	rm "$1"
	#fi

	if [ -d "$2" ]; then
		rm -rf "$2"
	fi
}



first_time(){
	format_disk "/dev/mmcblk0"
	local FIRMWARE_AR=$(echo $(find /media/ -name "$1") | awk '{print $1}')
	local X8_PARTITION="/dev/mmcblk0p1"
	local MOUNT_DIR="/initrd"

	umount -l "$MOUNT_DIR" && rm -rf "$MOUNT_DIR"
	mkdir -p "$MOUNT_DIR"

	mount "$X8_PARTITION" "$MOUNT_DIR" || ( echo "first_time: mount initrd partition error" && exit )

	[ ! -f "$FIRMWARE_AR" ] && echo "first_time:no firmware!!!" && exit
	un_compress_firmware "$FIRMWARE_AR" "$MOUNT_DIR"

	umount -l "$MOUNT_DIR" && rm -rf "$MOUNT_DIR"
}

upgrade(){
	flash_mark


	local FIRMWARE_AR=$(echo $(find /media/ -name "$1") | awk '{print $1}')
	local X8_ROM_PARTITION="/dev/mmcblk0p2"
	local ROM_DIR="/x8"
	local FIRMWARE_DIR="$ROM_DIR/firmware"

	umount -l "$ROM_DIR" && rm -rf "$ROM_DIR"
	mkdir -p "$ROM_DIR"

	mount "$X8_ROM_PARTITION" "$ROM_DIR" || ( echo "upgrade: mount x8 rom partition error" && exit )
	rm -rf "$FIRMWARE_DIR"
	mkdir -p "$FIRMWARE_DIR"

	[ ! -f "$FIRMWARE_AR" ] && echo "upgrade:no firmware!!!" && exit

	un_compress_firmware "$FIRMWARE_AR" "$FIRMWARE_DIR"

	flash_uldr "$FIRMWARE_DIR"
	flash_uboot "$FIRMWARE_DIR"
	flash_kernel "$FIRMWARE_DIR"
	do_rootfs "$FIRMWARE_DIR"
	finaly "$FIRMWARE_AR" "$FIRMWARE_DIR"

	umount -l "$ROM_DIR" && rm -rf "$ROM_DIR"
	
	flash_unmark
}



UPGRADE_SWITCH="/x8_upgrade_switch"
[ ! -f "$UPGRADE_SWITCH" ] && sleep 3 && exit
. "$UPGRADE_SWITCH"
rm -f "$UPGRADE_SWITCH"

case "$X8_ACTION" in
	"upgrade")
		#first_time "initrd.tar.gz"
		upgrade "firmware.x8.tar.gz"
		reboot
		;;
	*)
		;;

esac
