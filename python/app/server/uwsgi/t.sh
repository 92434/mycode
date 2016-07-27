#!/bin/sh

prefix=$(pwd)/.install

function install_tengine() {
	if [ -e Makefile ]; then
		:
	else
		./configure --prefix="$prefix"
	fi

	make install
}

function start_tengine() {
	PATH="$prefix/sbin:$PATH"; 
	nginx;
}

function stop_tengine() {
	sudo killall -9 nginx
}

function process_opt() {
	while getopts isk arguments 2>/dev/null; do
		#echo "arguments is $arguments"
		case $arguments in
			i)
				echo "install_tengine"
				install_tengine
				;;
			s)
				echo "start_tengine"
				start_tengine
				;;
			k)
				echo "stop_tengine"
				stop_tengine
				;;
			?)
				echo -e "Usage:$0 [option];"

				echo -e "\t-i:install;"
				echo -e "\t-s:start;"
				return
				;;
		esac
		#echo "\$OPTIND is $OPTIND \$OPTERR is $OPTERR"
	done
	OPTIND=0
}

process_opt $@
