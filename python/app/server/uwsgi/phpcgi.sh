#!/bin/bash

#================================================================
#   Copyright (C) 2018年01月21日 肖飞 All rights reserved
#   
#   文件名称：phpcgi.sh
#   创 建 者：肖飞
#   创建日期：2018年01月21日 星期日 16时28分37秒
#   修改日期：2018年01月21日 星期日 16时29分22秒
#   描    述：
#
#================================================================
function main() {
	php-cgi -b localhost:9000
}

main $@
