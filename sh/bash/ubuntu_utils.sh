#!/bin/sh

function mytest() {
	case "$1" in
		start)
			echo "my start"
			;;
		stop)
			echo "my stop"
			;;
		*)
			echo "other"
			;;
	esac
}
#mytest start

##network-manager reset
function network_manager_reset() {
	sudo service network-manager stop
	sudo rm /var/lib/NetworkManager/NetworkManager.state
	sudo service network-manager start
}

#network_manager_reset

function set_iptables {
	case "$1" in
		start)
			sudo iptables -t nat -A POSTROUTING -s 192.168.3.0/24 -j MASQUERADE
			;;
		stop)
			sudo iptables -t nat -D POSTROUTING -s 192.168.3.0/24 -j MASQUERADE
			;;
		*)
			;;
	esac
}
#set_iptables

function enable_vbox_symlink() {
	local VM_NAME SHARE_NAME
	VM_NAME="$1"
	SHARE_NAME="$2"
	echo VBoxManage setextradata $VM_NAME VBoxInternal2/SharedFoldersEnableSymlinksCreate/$SHARE_NAME 1
	echo VBoxManage getextradata $VM_NAME enumerate
}
#enable_vbox_symlink xiaofei_vm_name share_name
