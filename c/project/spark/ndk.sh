#!/bin/bash

#================================================================
#   Copyright (C) 2017年07月14日 肖飞 All rights reserved
#   
#   文件名称：ndk.sh
#   创 建 者：肖飞
#   创建日期：2017年07月14日 星期五 16时26分44秒
#   修改日期：2017年08月15日 星期二 10时44分43秒
#   描    述：
#
#================================================================
export PATH=$PATH:~/data/android-ndk-r14b

function main() {
	ndk-build NDK_LOG=1 NDK_DEBUG=1 V=1 NDK_HOST_32_BIT=1 $@
}

#main $@
main T=1 LIB_NAME=9348
main T=2 LIB_NAME=9348
