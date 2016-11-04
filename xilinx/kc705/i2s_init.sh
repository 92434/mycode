#!/bin/bash

i2s_group1_gpio_list="
232
233
234
235
"

i2s_group2_gpio_list="
242
243
244
245
"

i2s_group3_gpio_list="
252
253
254
255
"

function export_gpio_list() {
	for i in $i2s_group1_gpio_list; do
		echo $i > /proc/gpio_test/export
	done
	for i in $i2s_group2_gpio_list; do
		echo $i > /proc/gpio_test/export
	done
	for i in $i2s_group3_gpio_list; do
		echo $i > /proc/gpio_test/export
	done
}

function unexport_gpio_list() {
	for i in $i2s_group1_gpio_list; do
		echo $i > /proc/gpio_test/unexport
	done
	for i in $i2s_group2_gpio_list; do
		echo $i > /proc/gpio_test/unexport
	done
	for i in $i2s_group3_gpio_list; do
		echo $i > /proc/gpio_test/unexport
	done
}

function select_i2s_intf() {
	local intf_no=$1

	if (($intf_no >> 0 & 1 == 1)); then
		echo 1 > /proc/gpio_test/$2;	
	else
		echo 0 > /proc/gpio_test/$2;	
	fi

	if (($intf_no >> 1 & 1 == 1)); then
		echo 1 > /proc/gpio_test/$3;	
	else
		echo 0 > /proc/gpio_test/$3;	
	fi

	if (($intf_no >> 2 & 1 == 1)); then
		echo 1 > /proc/gpio_test/$4;	
	else
		echo 0 > /proc/gpio_test/$4;	
	fi

	if (($intf_no >> 3 & 1 == 1)); then
		echo 1 > /proc/gpio_test/$5;	
	else
		echo 0 > /proc/gpio_test/$5;	
	fi
}

function get_defined() {
	local l=$1;
	code="echo \$$l";
	eval $code;
}

function init_i2s_group() {
	local ports="$(get_defined "i2s_group""$1""_gpio_list")"

	for((i=0;i<16;i++));do
		select_i2s_intf $i $ports;

		sudo /home/n3150m/kc705_driver/app/i2s_spi_dev -D /dev/spidev1.0 <<EOF
		2
		8
		0
EOF
	done
}

function main() {
	export_gpio_list
	
	init_i2s_group 1
	#init_i2s_group 2
	#init_i2s_group 3

	unexport_gpio_list
}

main
