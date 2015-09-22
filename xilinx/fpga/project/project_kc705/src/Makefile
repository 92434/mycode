include $(default_rules)

#all_dirs := $(strip $(call all-dirs-under))

SUB_DIRS := driver app

include $(ADD_TARGET)
install:
	sudo insmod driver/i2c-gpio.ko
	sudo insmod driver/i2c-dev.ko
	sudo insmod driver/spi-gpio-all.ko
	sudo insmod driver/spidev.ko
	sudo insmod driver/kc705-module.ko
	dmesg
	sudo ./app/xdma_device /dev/pciedma_0 &

uninstall:
	-sudo killall -9 xdma_device
	while test -n "$(ps -elf | grep xdma_device 2>/dev/null)"; do sleep 1;done
	-sudo rmmod kc705-module
	-sudo rmmod i2c-dev
	-sudo rmmod i2c_gpio
	-sudo rmmod spidev
	-sudo rmmod spi_gpio_all
	sudo dmesg -c
