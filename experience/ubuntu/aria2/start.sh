#!/bin/bash

#================================================================
#   Copyright (C) 2018年01月24日 肖飞 All rights reserved
#   
#   文件名称：start.sh
#   创 建 者：肖飞
#   创建日期：2018年01月24日 星期三 20时55分00秒
#   修改日期：2018年01月24日 星期三 20时56分29秒
#   描    述：
#
#================================================================
function main() {
	screen -S aria2.server sh aria2.sh
	screen -S aria2.ng sh server.sh
}

main $@
