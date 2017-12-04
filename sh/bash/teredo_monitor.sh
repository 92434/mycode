#!/bin/bash

#================================================================
#   Copyright (C) 2017年12月04日 肖飞 All rights reserved
#   
#   文件名称：teredo_monitor.sh
#   创 建 者：肖飞
#   创建日期：2017年12月04日 星期一 09时40分48秒
#   修改日期：2017年12月04日 星期一 17时34分39秒
#   描    述：
#
#================================================================
function main() {
	local i=0;
	while true;do
		if [ "$(sudo ifconfig -a | grep teredo)" = "" ];then
			echo -ne "\r                                                                                    ";
			echo -ne "\r重启teredo...!";
			sudo service miredo restart;
			((i=0))
		else
			if [ "$(sudo ifconfig | grep teredo)" = "" ];then
				((i++));
				echo -ne "\r                                                                                    ";
				echo -ne "\rteredo掉线$((i))秒!";
			else
				echo -ne "\r                                                                                    ";
				echo -ne "\rteredo连接正常!";
				((i=0))
			fi;
		fi;

		read -n 1 -t 1 key;
		if [ "$key" = "r" ];then
			echo -ne "\r                                                                                    ";
			echo -ne "\r重启teredo...";
			sudo service miredo restart;
			((i=0))
			sleep 1;
		fi
	done
}

main $@
