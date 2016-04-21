#!/bin/sh

function get_dvb_out_count() {
	sudo ./app/dvbs2_regs /dev/dvbs2_regs | grep FS_EN2_COUNT_REG | sed 's/FS_EN2_COUNT_REG(030): [^(]*(\(.*\))/\1/g'
}

function get_current_count() {
	local count=$(get_dvb_out_count)
	local count_bak=$count

	while true;do
		sleep 1;
		count=$(get_dvb_out_count);
		echo $((count - count_bak));
		count_bak=$count;
	done
}

get_current_count
