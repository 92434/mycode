#!/bin/bash

#================================================================
#   Copyright (C) 2018年01月21日 肖飞 All rights reserved
#   
#   文件名称：server.sh
#   创 建 者：肖飞
#   创建日期：2018年01月21日 星期日 08时59分29秒
#   修改日期：2018年01月21日 星期日 08时59分41秒
#   描    述：
#
#================================================================
function main() {
	python -m SimpleHTTPServer 4080
}

main $@
