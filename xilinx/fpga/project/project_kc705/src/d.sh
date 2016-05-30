#!/bin/sh

sudo dmesg -c
sudo ./app/dvbs2_regs /dev/dvbs2_regs > log
dmesg >>log
#vi log
cat log
