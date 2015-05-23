#!/bin/sh

ifconfig eth0 down
ifconfig eth0 hw ether 00:AA:BB:CC:DD:EE
/etc/services/udhcpc stop
/etc/services/udhcpc start
