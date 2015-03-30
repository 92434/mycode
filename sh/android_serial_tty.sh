#!/system/bin/sh

#while true; do busybox getty -n -L ttyUSB0 115200 vt100 -l /system/bin/sh;done &
#while true; do busybox getty -n -L ttyUSB1 115200 vt100 -l /system/bin/sh;done &
#while true; do busybox getty -n -L ttyUSB2 115200 vt100 -l /system/bin/sh;done &
#while true; do busybox getty -n -L ttyUSB3 115200 vt100 -l /system/bin/sh;done &
while true; do busybox getty -n -L ttyS2 115200 vt100 -l /system/bin/sh;done &

busybox mount -o rw,remount /misc

busybox tcpsvd -vE 0.0.0.0 21 busybox ftpd -w /misc/modules &

busybox mount -o rw,remount /system
busybox ifconfig -a

busybox ash
PS1='$USER@$HOSTNAME:${PWD:-?} # '

serial_test /dev/ttyUSB0 &
serial_test /dev/ttyUSB1 &
serial_test /dev/ttyUSB2 &
serial_test /dev/ttyUSB3 &
serial_test /dev/ttyS3 &

echo 0>/proc/acts_hcd
echo o>/proc/acts_hcd
echo a>/proc/acts_hcd
echo c>/proc/acts_hcd
cat /proc/acts_hcd

cat /sys/kernel/debug/usb/devices
for i in `ps | grep "serial_test" | busybox awk '{ if(NF == 9) print $2 }'`;do kill $i;done

ps | grep serial_test
cat 

S0	C19
S2	C9
S3	C5
S5	C6


ttyUSB1	C1
ttyUSB0	C2
ttyUSB3	C3 ?
ttyUSB2	C4

adb root //来获取 root 权限
adb shell busybox mount -o rw,remount /misc //挂载 misc 为可写分区。
adb push ctp_gslX680.ko /misc/modules/ //push 进小机
adb shell sync //确保写到 flash 中
adb shell rmmod gslX680 //卸载旧驱动，名字从 lsmod 中获取
adb shell insmod /misc/modules/ctp_gslX680.ko //加载新驱动


bootup:
T:  Bus=02 Lev=00 Prnt=00 Port=00 Cnt=00 Dev#=  1 Spd=5000 MxCh= 1
B:  Alloc=  0/800 us ( 0%), #Int=  0, #Iso=  0
D:  Ver= 3.00 Cls=09(hub  ) Sub=00 Prot=03 MxPS= 9 #Cfgs=  1
P:  Vendor=1d6b ProdID=0003 Rev= 3.04
S:  Manufacturer=Linux 3.4.0+ xhci-hcd
S:  Product=xHCI Host Controller
S:  SerialNumber=xhci-hcd
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   4 Ivl=256ms

T:  Bus=01 Lev=00 Prnt=00 Port=00 Cnt=00 Dev#=  1 Spd=480  MxCh= 1
B:  Alloc=  0/800 us ( 0%), #Int=  0, #Iso=  0
D:  Ver= 2.00 Cls=09(hub  ) Sub=00 Prot=01 MxPS=64 #Cfgs=  1
P:  Vendor=1d6b ProdID=0002 Rev= 3.04
S:  Manufacturer=Linux 3.4.0+ xhci-hcd
S:  Product=xHCI Host Controller
S:  SerialNumber=xhci-hcd
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   4 Ivl=256ms


T:  Bus=04 Lev=00 Prnt=00 Port=00 Cnt=00 Dev#=  1 Spd=480  MxCh= 1
B:  Alloc=  0/800 us ( 0%), #Int=  0, #Iso=  0
D:  Ver= 2.00 Cls=09(hub  ) Sub=00 Prot=01 MxPS=64 #Cfgs=  1
P:  Vendor=1d6b ProdID=0002 Rev= 3.04
S:  Manufacturer=Linux 3.4.0+ aotg_hcd
S:  Product=AOTG USB Host Controller Driver
S:  SerialNumber=aotg_hcd.1
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   4 Ivl=256ms

T:  Bus=03 Lev=00 Prnt=00 Port=00 Cnt=00 Dev#=  1 Spd=480  MxCh= 1
B:  Alloc=  0/800 us ( 0%), #Int=  0, #Iso=  0
D:  Ver= 2.00 Cls=09(hub  ) Sub=00 Prot=01 MxPS=64 #Cfgs=  1
P:  Vendor=1d6b ProdID=0002 Rev= 3.04
S:  Manufacturer=Linux 3.4.0+ aotg_hcd
S:  Product=AOTG USB Host Controller Driver
S:  SerialNumber=aotg_hcd.0
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   4 Ivl=256ms

T:  Bus=02 Lev=00 Prnt=00 Port=00 Cnt=00 Dev#=  1 Spd=5000 MxCh= 1
B:  Alloc=  0/800 us ( 0%), #Int=  0, #Iso=  0
D:  Ver= 3.00 Cls=09(hub  ) Sub=00 Prot=03 MxPS= 9 #Cfgs=  1
P:  Vendor=1d6b ProdID=0003 Rev= 3.04
S:  Manufacturer=Linux 3.4.0+ xhci-hcd
S:  Product=xHCI Host Controller
S:  SerialNumber=xhci-hcd
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   4 Ivl=256ms

T:  Bus=01 Lev=00 Prnt=00 Port=00 Cnt=00 Dev#=  1 Spd=480  MxCh= 1
B:  Alloc=  0/800 us ( 0%), #Int=  0, #Iso=  0
D:  Ver= 2.00 Cls=09(hub  ) Sub=00 Prot=01 MxPS=64 #Cfgs=  1
P:  Vendor=1d6b ProdID=0002 Rev= 3.04
S:  Manufacturer=Linux 3.4.0+ xhci-hcd
S:  Product=xHCI Host Controller
S:  SerialNumber=xhci-hcd
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   4 Ivl=256ms


tags.sh
vim
serial tools



insmod /misc/modules/rt2x00lib.ko
insmod /misc/modules/rt2x00usb.ko
insmod /misc/modules/rt2800lib.ko
insmod /misc/modules/rt2800usb.ko

busybox ps -w | grep wpa
for i in $(busybox find . -name "*.conf" 2>/dev/null);do if test -n "$(busybox diff -q $i /data/misc/wifi/wpa_supplicant.conf)";then :;else echo $i;fi;done
for i in $(busybox find . -name "*.rc" 2>/dev/null | grep "init.wifi");do if test -n "$(busybox diff -q $i ./android/device/actions/prebuilt/wifi/mt6620/initramfs/init.wifi.rc)";then :;else echo $i;fi;done
for i in $(busybox find . 2>/dev/null | grep "wpa_supplicant.conf");do cp $i $i.bak;done



===================================
ctrl_interface=/data/misc/wifi/sockets
update_config=1
device_name=Android_d488
manufacturer=Actions
model_name=86v Demo
model_number=86v Demo
serial_number=A02FDDF8E0C1F35C
device_type=10-0050F204-5
config_methods=virtual_push_button physical_display keypad
p2p_ssid_postfix=-Android_d488
persistent_reconnect=1

network={
        ssid="xf22001"
        key_mgmt=NONE
        auth_alg=OPEN SHARED
        wep_key0="22001"
        priority=1
}
eth.addr == 00:18:FE:44:C0:1D


root@gs702c:/sys/class/net/eth0 # for i in statistics/*error*;do echo $i; cat $i;done
statistics/rx_crc_errors
65
statistics/rx_errors
65
statistics/rx_fifo_errors
0
statistics/rx_frame_errors
62
statistics/rx_length_errors
0
statistics/rx_missed_errors
0
statistics/rx_over_errors
0
statistics/tx_aborted_errors
0
statistics/tx_carrier_errors
0
statistics/tx_errors
0
statistics/tx_fifo_errors
0
statistics/tx_heartbeat_errors
0
statistics/tx_window_errors
0

root@gs702c:/sys/class/net/eth0 # busybox ifconfig
eth0      Link encap:Ethernet  HWaddr 00:18:FE:F5:8F:3B
          inet addr:192.168.1.152  Bcast:192.168.1.255  Mask:255.255.255.0
          inet6 addr: fe80::218:feff:fef5:8f3b/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:41 errors:65 dropped:0 overruns:0 frame:127
          TX packets:1455 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:5057 (4.9 KiB)  TX bytes:87020 (84.9 KiB)
          Interrupt:209
