#!/bin/bash

#================================================================
#   Copyright (C) 2018年01月22日 肖飞 All rights reserved
#   
#   文件名称：save.sh
#   创 建 者：肖飞
#   创建日期：2018年01月22日 星期一 09时37分21秒
#   修改日期：2018年01月22日 星期一 09时39分42秒
#   描    述：
#
#================================================================
function main() {
	if [ -f config.json ];then
		echo mv config.json confs/config.json.$(date +%m%d.%H%M%S)
	fi
}

main $@
