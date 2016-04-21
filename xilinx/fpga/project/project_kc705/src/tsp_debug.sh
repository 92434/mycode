#!/bin/sh

while true;do
	sudo dmesg -c;
	sudo ./app/tsp_regs /dev/tsp_regs >log1;
	dmesg > log2;
	#head -n 32 log1;
	head -n 50 log1;
	#vi log1;
	sleep 1;
done
