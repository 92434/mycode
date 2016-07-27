#!/bin/sh

pwd_="$(pwd)/utils"

c=$(echo $PYTHONPATH | sed 's/ /&/g' | sed 's/:/ /g')
#echo "c: $c"

t=($c)
#echo "\${#t[@]}: ${#t[@]}"

item=
path_exist=0
for((i=0;i<${#t[@]};i+=1));do
	item="${t[i]}"
	item=$(echo $item | sed 's/&/ /g')
	if [ "$item" = "$pwd_" ];then
		path_exist=1;
		break;
	fi
done

if [ $path_exist -eq 1 ];then
	echo match
	:
else
	echo mismatch
	export PYTHONPATH=${PYTHONPATH:+$PYTHONPATH:}"$pwd_"
fi
