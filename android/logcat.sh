#!/bin/sh

function start_logcat() {
	local package=$1

	if [ -z "$package" ]; then
		echo "package:$package"
		return
	fi

	local pid=$(adb shell busybox ps | grep "$package" | awk '{print $1}')

	if [ -z "$pid" ]; then
		echo "pid:$pid";
		return;
	fi

	shift

	adb logcat $@ -v brief "*:V" | grep $pid
}

start_logcat $@
