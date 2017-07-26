#!/bin/bash

#================================================================
#   Copyright (C) 2017年07月26日 肖飞 All rights reserved
#   
#   文件名称：trap.sh
#   创 建 者：肖飞
#   创建日期：2017年07月26日 星期三 15时17分18秒
#   修改日期：2017年07月26日 星期三 15时33分43秒
#   描    述：
#
#================================================================
function do_trap() {
	echo "do_trap!"
	echo $$
	exit 0
}

function main() {
	trap 'echo "goodbye!!!"; do_trap' SIGINT SIGTERM SIGQUIT SIGKILL
	while true;do sleep 1;done
}

main $@
