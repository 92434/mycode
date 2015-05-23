#!/bin/sh

dmesg -c >>/dev/null
if test -z "$(lsmod|grep glyphy)";then
	echo "install glyphy.ko"
	insmod /lib/modules/3.15.0-xilinx-dirty/glyphy.ko
fi
#sleep 0.1

#dmesg

#dmesg -c >>/dev/null
#./glyphy_test

#dmesg


echo "==================test info====================="
touch .glyphy
count=0
while `test -f .glyphy`; do
	info=`date`
	count=$((count+1))
	printf "%s[%d]\r" "$info" $count
	./glyphy_test
done

echo "==================test end!====================="
rmmod glyphy
