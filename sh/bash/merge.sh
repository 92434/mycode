#!/bin/sh

#fmpeg -i "concat:微信网页版.mp4|微信网页版-1.mp4|微信网页版-2.mp4|微信网页版-3.mp4|微信网页版-4.mp4|微信网页版-5.mp4|微信网页版-6.mp4" -c copy "微信网页版-1.mp4"

python3 /home/xiaofei/workspace/code/temp/python/downloader/you-get/src/you_get/processor/join_mp4.py --output "刘志强婚礼.mp4" "微信网页版.mp4" "微信网页版-2.mp4" "微信网页版-3.mp4" "微信网页版-4.mp4" "微信网页版-5.mp4" "微信网页版-6.mp4"
