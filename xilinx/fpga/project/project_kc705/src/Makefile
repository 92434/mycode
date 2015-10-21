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
#	sudo insmod driver/kc705_i2c_device.ko
	sudo ./app/pcie_dma_device /dev/pciedma_0 &
	dmesg

uninstall:
	-sudo killall -9 pcie_dma_device
	while test -n "$(ps -elf | grep pcie_dma_device 2>/dev/null)"; do sleep 1;done
	-sudo rmmod i2c-dev
#	-sudo rmmod kc705_i2c_device
	-sudo rmmod i2c_gpio
	-sudo rmmod spidev
	-sudo rmmod spi_gpio_all
	-sudo rmmod kc705-module
	sudo dmesg -c
start_dma:
	sudo dmesg -c
	sudo ./app/kc705_device /dev/kc705_pcie start 

stop_dma:
	sudo dmesg -c
	sudo ./app/kc705_device /dev/kc705_pcie stop

test:
	while true;do make start_dma; sleep 3; make stop_dma; sleep 3;done
