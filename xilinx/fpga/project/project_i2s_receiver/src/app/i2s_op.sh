#!/bin/sh

i2s_receiver_add() {
	insmod lib/modules/3.6.0-digilent-13.01-00002-g06b3889-dirty/myio.ko
	insmod lib/modules/3.6.0-digilent-13.01-00002-g06b3889-dirty/xdma.ko
}

i2s_receiver_remove() {
	rmmod xdma
	rmmod myio
}

test_receiver() {
	./my_i2s_receiver /dev/xdma
}

for i do
	case $i in
		test)
			test_receiver
			;;
		add)
			i2s_receiver_add
			;;
		remove)
			i2s_receiver_remove
			;;
		*)
			echo "parameter error!!!"
			;;
	esac
done
