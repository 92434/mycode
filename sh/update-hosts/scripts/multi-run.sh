#!/bin/bash

count=100

pronum=10               #进程个数

tmpfile="$$.fifo"        #临时生成管道文件
mkfifo $tmpfile
exec 6<>$tmpfile
rm $tmpfile

for(( k=0; k<$pronum; k++ ))
do
	echo "init.$k"
done >&6

##for(( i = 0; i < $count; i++ ))
##do
##	read line
##	#echo $line
##	{
##		echo "$line.${i} finished."
##	} &
##done <&6
##
##wait

for((i = 0; i < $count;i))
do
	for((j = 0; j < $pronum; j++))
	do
		read line
		{
			echo "$line.$((i+j)) finished."
			sleep 1
		} &

	done <&6


	if ((i % $pronum == 0));then
		wait
		for(( k=0; k<$pronum; k++ ))
		do
			if ((i + k != $count));then
				echo "init.$k"
			fi
		done >&6
	fi

	((i=i+$pronum))
done
