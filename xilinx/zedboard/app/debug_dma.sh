#!/bin/sh

touch test.stamp

#dmesg >log3

#dmesg -c >>/dev/null
#if test -z "$(lsmod|grep xvdma)";then
if test -z "$(lsmod|grep xotsu2)";then
	#insmod /lib/modules/3.6.0-digilent-13.01-00002-g06b3889-dirty/xvdma.ko
	insmod lib/modules/3.6.0-digilent-13.01-00002-g06b3889-dirty/xotsu2.ko
fi

while [ -f test.stamp ];do
	date
	#dmesg -c >>/dev/null
	./otsu2
	#./mmio_vdma
	#dmesg >log1
	#hexdump -C data0.bin -n 64
	break
done

#dmesg -c >>/dev/null
#./filter
#hexdump -C data0.bin -n 64
#dmesg >log2
rmmod xotsu2
