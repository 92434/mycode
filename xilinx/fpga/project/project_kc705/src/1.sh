#!/bin/sh

function get_count() {
	sudo ./app/dvbs2_regs /dev/dvbs2_regs | grep FS_EN2_COUNT_REG | sed 's/FS_EN2_COUNT_REG(030): [^(]*(\(.*\))/\1/g'
	#sudo ./app/dvbs2_regs /dev/dvbs2_regs | grep FS_EN_COUNT_REG | sed 's/FS_EN_COUNT_REG(034): [^(]*(\(.*\))/\1/g'
	#sudo ./app/dvbs2_regs /dev/dvbs2_regs | grep MPEG_BYTES_COUNT_REG | sed 's/MPEG_BYTES_COUNT_REG(038): [^(]*(\(.*\))/\1/g'
	#sudo ./app/dvbs2_regs /dev/dvbs2_regs | grep SYMBOL_1X_OE_COUNT_REG | sed 's/SYMBOL_1X_OE_COUNT_REG(03c): [^(]*(\(.*\))/\1/g'
	#sudo ./app/dvbs2_regs /dev/dvbs2_regs | grep SYMBOL_2X_OE_COUNT_REG | sed 's/SYMBOL_2X_OE_COUNT_REG(040): [^(]*(\(.*\))/\1/g'
}

function get_current_count() {
	local count=$(get_count)
	local count_bak=$count

	while true;do
		sleep 1;
		count=$(get_count);
		echo $((count - count_bak));
		count_bak=$count;
	done
}

get_current_count
