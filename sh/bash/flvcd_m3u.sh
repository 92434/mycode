#!/bin/bash
#filename wgetflv.sh
#author 晴耕雨讀
#date 08/28/2010
#source http://sinolog.it/?p=1589
#
#update  2011-03-14 14:10:08 
 
#打印帮助
#print_help_msg () {
#    echo "You see, I'm nothing ."
#    exit 0
#}
  
#检测参数：视频网址是否输入  by er
check_param () {
    if [ $# -ne 1 ]; then
        read -p '输入网址：' url
    else
        url=$1
    fi
}
  
#检测m3u文件是否存在
check_m3u () {
    if ! [ -a p.m3u ] || [ `wc -l p.m3u|awk '{ print $1 }'` -eq 0 ]; then
    echo '#EXTM3U'> p.m3u
    fi
}
  
check_param $*
check_m3u
  
export LC_ALL=en_US.UTF-8
  
#使用php解析地址
ue=$(php -r "echo urlencode('$url');")
parser="http://www.flvcd.com/parse.php?flag=&format=&kw=$ue&sbt=%BF%AA%CA%BCGO%21"
#使用wget下载flvcd的页面保存为meta.html
if ! wget $parser -U mozilla -O meta.html; then
    echo "Unable to touch the parser, check network status for the cause ."
    exit 0
fi
  
#处理
grep "<N>" meta.html> title.lst
grep "<U>" meta.html> url.lst
iconv -f gbk -t utf-8 title.lst -o title.lst
sed -i 's/<N>//g' title.lst
sed -i 's/ //g' title.lst
sed -i 's/<U>//g' url.lst
  
l1=`wc -l title.lst|awk '{ print $1 }'`
l2=`wc -l url.lst|awk '{ print $1 }'`
if [ "$l1" != "$l2" ]; then
    echo "Title.lst has $l1 lines, but url.lst got $l2."
    exit 0
fi
if [ $l1 -eq 0 ]; then
    echo "Nothing got from the parser, check meta.html for detail info."
    exit 0
fi
  
arrTitle=(`cat title.lst`)
arrURL=(`cat url.lst`)
  
idx=$((`wc -l p.m3u|awk '{ print $1 }'`-1))
while [ $idx -lt $l1 ]; do
    title=${arrTitle[$idx]}
    url=${arrURL[$idx]}
    idx=$((idx+1))
    echo '已经限速到111024B...'
    if ! wget -c --limit-rate=111024 $url -U mozilla -O "${title}.flv"; then
        echo "Failed fetching ${title}.flv, maybe its URL has been changed !"
    exit 0
    fi
    cmd="sed -i '\$a\\${title}.flv' p.m3u"
    eval $cmd
done
  
#清除tmp文件 p.m3u meta.html by er
rm -vf p.m3u
rm -vf meta.html
echo 'done !'
exit 0
