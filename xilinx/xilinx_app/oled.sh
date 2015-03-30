#!/bin/sh
#set -x

write_to_oled() {
	echo -ne "" >xiaofei.bin
	i=0;
	while [ $i -lt $1 ];
	do 
		echo -ne $2 >> xiaofei.bin;
		let i=$i+1;
	done;
	cat xiaofei.bin >/dev/zed_oled;
}

write_to_oled 512 "\\x00"

c=0;
while [ $c -lt 512 ];
do
	write_to_oled $c "\\xff";
	let c=$c+1;
	#sleep 0.01;
done

cat logo.bin >/dev/zed_oled
sleep 1
