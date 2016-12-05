#!/bin/sh

insmod /lib/modules/g_mass_storage.ko file=/dev/mmcblk0p1,/dev/mmcblk0p2 stall=0 removable=1
