#!/bin/sh

function get_dvb_out_count() {
	sudo ./app/dvbs2_regs /dev/dvbs2_regs | grep SYMBOL_2X_OE_POSEDGE_COUNT | sed 's/SYMBOL_2X_OE_POSEDGE_COUNT: [^(]*(\(.*\))/\1/g'
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
