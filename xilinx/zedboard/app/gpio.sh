#!/bin/sh
#set -x

i=61
while [ $i -le 68 ]
do
	if [ ! -d /sys/class/gpio/gpio$i ]; then
		echo $i > /sys/class/gpio/export
	fi
	let i=$i+1
done
		
i=61
while [ $i -le 68 ]
do
	echo out > /sys/class/gpio/gpio$i/direction
	echo 1 > /sys/class/gpio/gpio$i/value
	let i=$i+1
	sleep 1
done
					
i=61
while [ $i -le 68 ]
do
	echo 0 > /sys/class/gpio/gpio$i/value
	let i=$i+1
	sleep 1
done

i=61
while [ $i -le 68 ]
do
	if [ -d /sys/class/gpio/gpio$i ]; then
		echo $i > /sys/class/gpio/unexport
	fi
	let i=$i+1
done
