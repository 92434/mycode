#!/bin/sh

add_u_disk() {
	insmod /lib/modules/g_mass_storage.ko file=/dev/mmcblk0p1,/dev/mmcblk0p2 stall=0 removable=1
}

remove_u_disk() {
	rmmod g_mass_storage
}

start() {
	echo -n "add_u_disk..."
	add_u_disk
	echo "done!"
}

stop() {
	echo -n "remove_u_disk"
	remove_u_disk
	echo "done!"
}


main() {
	case "$1" in
		start)
			start
			;;
		stop)
			stop
			;;
		restart|reload)
			stop
			start
			;;
		*)
			echo "Usage: $0 {start|stop|restart}"
			exit 1
	esac
	exit $?
}

main $@
