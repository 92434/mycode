#!/bin/sh

require="
nmap
curl
"

for i in $require;do
	if test -z $(which $i); then
		echo no $i
		exit
	fi
done

if [ $# -eq 0 ]
then
    echo -e "Usage:\n";
    echo -e "    $0 data [www.google.com]"
    exit 0;
fi

#grep -o "[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+"
cat $1 | grep -o "[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+" | sort | uniq | awk '{print $NF}' >$$.ips

if [ ! -f output/result ];then
	./getssl.sh $$.ips
fi
rm $$.ips

sort -k2 -k3 output/result -o output/result

if test -n "$(./filter.sh | head -n 1 | grep "NO")";then
	echo no result
	exit
fi


if [ -z "$2" ];then
	./select.sh
else
	if [ "$(echo $2 | cut -d . -f 1)" = "*" ]
	then
	    #p=${1//\./\\\.}
	    #p=".$p"
	    :
	else
		if [ -z "$(grep "$2" hosts.all)" ];then
			echo "127.0.0.1    $2" >> hosts.all
		fi
	fi
	./use.sh "$2" $(./filter.sh | head -n 1 | awk '{print $1}')
fi
./apply.sh
