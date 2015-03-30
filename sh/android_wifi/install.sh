REM this bat used to install 3c and wifi test.

ECHO "test start"

adb devices

adb shell /system/bin/mount -t ext4 -o remount,rw /dev/block/mmcblk0p1 /system/
adb push iwconfig /system/bin/
adb push iwlist /system/bin/
adb push iwpriv /system/bin/
adb push wifi.sh /data/
adb push test.sh /data/
adb push 8192cu.ko /system/lib/wifi/modules/
adb push 8192cu_mp.ko /system/lib/wifi/modules/	

adb shell chmod 777 /system/bin/iw*
adb shell ls -l /system/bin/iw*
adb shell ls -l /system/lib/wifi/modules/

pause
