#!/bin/sh

function start_logcat() {
	local package=$1

	if [ -z "$package" ]; then
		echo "package:$package"
		return
	fi

	local pids=($(adb shell busybox ps | grep "$package" | awk '{print $1}'))

	if [ -z "$pids" ]; then
		echo "pids:$pids";
		return;
	fi

	shift

	local pattern
	local i

	for((i=0; i<${#pids[@]}; i++));do
		pattern=${pattern:+$pattern\\|}"${pids[i]}):"
	done

	adb logcat $@ -v brief "*:V" | grep -e "$pattern"
}

start_logcat $@
