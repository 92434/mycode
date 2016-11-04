#!/bin/sh

function red() {
	echo -ne "\033[31m$@\033[0m"
}

function process_opt() {
	local error="Use [sh $0 -h] for help!"

	while getopts "pr" arguments 2>/dev/null; do
		echo "arguments is $arguments"
		error=
		case $arguments in
			p)
				echo "patch for product..."
				patch -p0 < product.patch
				mv product.patch recovery.patch
				;;
			r)
				echo "recovery for product..."
				patch -Rp0 < recovery.patch
				mv recovery.patch product.patch
				;;
			?)
				echo -e "Usage:$0 [option];"

				echo -e "\t-p:patch for product"
				echo -e "\t-r:recovery for product"
				return
				;;
		esac
		#echo "\$OPTIND is $OPTIND \$OPTERR is $OPTERR"
	done
	OPTIND=0

	if [ -n "$error" ]; then
		echo $(red "$error")
	fi
}

function main() {
	process_opt $@
}

main $@
