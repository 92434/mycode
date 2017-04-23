#!/bin/sh

complete -F _test mycomp

function mycomp() {
	echo "function is $0:$FUNCNAME, args is $*"
}

_test() {
	echo -ne "\x0a==============================="
	echo -ne "\x0aFUNCNAME is $FUNCNAME"
	echo -ne "\x0a@ is $@"
	echo -ne "\x0a1 is $1"
	echo -ne "\x0aCOMP_CWORD is $COMP_CWORD"
	echo -ne "\x0aCOMP_WORDS is ${COMP_WORDS[*]}"
	OPTIONS="-d -e -s -p"
	COMMAND="devices connect disconnect push pull sync shell emu logcat lolcat forward jdwp install uninstall bugreport help version start-server kill-server get-state get-serialno status-window remount reboot reboot-bootloader root usb tcpip"
	COMPREPLY=( $(compgen -W "$OPTIONS $COMMAND" -- "${COMP_WORDS[$COMP_CWORD]}") )
	echo -ne "\x0a[COMPREPLY is ${COMPREPLY[*]}]"
	echo -ne "\x0a===============================" 
}
