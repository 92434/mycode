#!/bin/sh
set -x

BUSYBOX_DIR="src/busybox-1.19.4"
TARGET_DIR="initrd"
DIR_NEED="etc proc dev sys usr var tmp root"
UPGRADE_SH="src/upgrade.sh"

if [ -z "$BUSYBOX_DIR" ] || [ -z "$TARGET_DIR" ]; then
	echo "set path first!"
	exit
fi


TARGET_DIR="$(pwd)/$TARGET_DIR"
#TARGET_DIR="/home/xiaofei/nfs_rootfs/$TARGET_DIR"

rm -rf "$TARGET_DIR"

mkdir -p "$TARGET_DIR"

for i in $DIR_NEED
do
	mkdir -p "$TARGET_DIR/$i"
done

rsync -aR "$BUSYBOX_DIR/_install/./" "$TARGET_DIR/"
rsync -aR "$BUSYBOX_DIR/examples/bootfloppy/./etc" "$TARGET_DIR/"
cp -a "$UPGRADE_SH" "$TARGET_DIR/etc/init.d/"

PREINIT_FILE="/etc/preinit"
cat > "$TARGET_DIR$PREINIT_FILE" << EOF
#!/bin/sh
exec /sbin/init
EOF
chmod a+x "$TARGET_DIR$PREINIT_FILE"


INITTAB_FILE="/etc/inittab"
cat > "$TARGET_DIR$INITTAB_FILE" << EOF
::sysinit:/etc/init.d/rcS
::respawn:-/bin/sh
::respawn:-/etc/init.d/upgrade.sh
tty2::askfirst:-/bin/sh
::ctrlaltdel:/bin/umount -a -r
EOF


RCS_FILE="/etc/init.d/rcS"
cat > "$TARGET_DIR$RCS_FILE" << EOF
#!/bin/sh

/bin/mount -a

#挂载/sys为sysfs文件系统
echo "----------mount /sys as sysfs"
/bin/mount -t tmpfs mdev /dev
/bin/mount -t sysfs sysfs /sys

echo "----------Starting mdev......"
/bin/echo /sbin/mdev > /proc/sys/kernel/hotplug
/sbin/mdev -s 
EOF
chmod a+x "$TARGET_DIR$RCS_FILE"

MDEV_CONF="/etc/mdev.conf"
cat > "$TARGET_DIR$MDEV_CONF" << EOF
sd[a-z][0-9]	0:0 666	@/etc/hotplug/usb/udisk_hotplug                              
sd[a-z]	0:0 666	\$/etc/hotplug/usb/udisk_hotplug
EOF


UDISK_HOTPLUG="/etc/hotplug/usb/udisk_hotplug"
mkdir -p $(dirname "$TARGET_DIR$UDISK_HOTPLUG")
cat > "$TARGET_DIR$UDISK_HOTPLUG" << EOF
#!/bin/sh

LOG_FILE="/hotplug.log"
#exec 100>&1
#exec 101>&2
#exec 1>>"\$LOG_FILE"
#exec 2>&1
#set -x
#set
case "\$ACTION" in
	"add")
		if [ -d /sys/block/*/\$MDEV ]  ; then
			mkdir -p "/media/\$MDEV"
			mount "/dev/\$MDEV" "/media/\$MDEV"

			UPGRADE_SWITCH="/x8_upgrade_switch"
			cat > "\$UPGRADE_SWITCH" << CONFIG_CONTENT
X8_ACTION="upgrade"
CONFIG_CONTENT
		fi
	;;
	"remove")
		for i in /media/\$MDEV*;do
			echo umount -l "\$i"
			umount -l "\$i"
			echo rm -rf "\$i"
			rm -rf "\$i"
		done
	;;
	*)
		if [ -n "\$MDEV" ]  ; then
			mkdir -p "/media/\$MDEV"
			mount "/dev/\$MDEV" "/media/\$MDEV"

			UPGRADE_SWITCH="/x8_upgrade_switch"
			cat > "\$UPGRADE_SWITCH" << CONFIG_CONTENT
X8_ACTION="upgrade"
CONFIG_CONTENT
		fi
	;;
esac
#exec 1>&100
#exec 2>&101
#exec 100>&-
#exec 101>&-
EOF
chmod a+x "$TARGET_DIR$UDISK_HOTPLUG"

LIB_SRC_DIR="src"
while read line
do
	if [ -z "$line" -o ${line:0:1} == "#" ];then
		continue
	fi

	rsync -aR "$LIB_SRC_DIR/$line" "$TARGET_DIR/"
done << EOF
#./lib/firmware/kronos_vfw.bin
#./lib/firmware/libphTSATSP_kronos_a0.obj
#./lib/firmware/tmasAFWFramework_kronos.mi
#./lib/firmware/tmscsScf.bin
./lib/ld-2.18.so
./lib/ld-linux-armhf.so.3
#./lib/libanl-2.18.so
#./lib/libanl.so
#./lib/libanl.so.1
#./lib/libasan.so
#./lib/libasan.so.0
#./lib/libasan.so.0.0.0
#./lib/libatomic.so
#./lib/libatomic.so.1
#./lib/libatomic.so.1.0.0
#./lib/libBrokenLocale-2.18.so
#./lib/libBrokenLocale.so
#./lib/libBrokenLocale.so.1
./lib/libc-2.18.so
#./lib/libcidn-2.18.so
#./lib/libcidn.so
#./lib/libcidn.so.1
#./lib/libc_pic/sofini.o
#./lib/libc_pic/soinit.o
./lib/libcrypt-2.18.so
./lib/libcrypt.so
./lib/libcrypt.so.1
./lib/libc.so
./lib/libc.so.6
#./lib/libdl-2.18.so
#./lib/libdl.so
#./lib/libdl.so.2
./lib/libgcc_s.so
./lib/libgcc_s.so.1
#./lib/libitm.so
#./lib/libitm.so.1
#./lib/libitm.so.1.0.0
./lib/libm-2.18.so
#./lib/libmemusage.so
./lib/libm.so
./lib/libm.so.6
./lib/libnsl-2.18.so
./lib/libnsl.a
./lib/libnsl.so
./lib/libnsl.so.1
#./lib/libnss_compat-2.18.so
#./lib/libnss_compat.so
#./lib/libnss_compat.so.2
#./lib/libnss_db-2.18.so
#./lib/libnss_db.so
#./lib/libnss_db.so.2
#./lib/libnss_dns-2.18.so
#./lib/libnss_dns.so
#./lib/libnss_dns.so.2
#./lib/libnss_files-2.18.so
#./lib/libnss_files_pic.a
#./lib/libnss_files_pic.map
#./lib/libnss_files.so
#./lib/libnss_files.so.2
#./lib/libnss_hesiod-2.18.so
#./lib/libnss_hesiod.so
#./lib/libnss_hesiod.so.2
#./lib/libnss_nis-2.18.so
#./lib/libnss_nisplus-2.18.so
#./lib/libnss_nisplus.so
#./lib/libnss_nisplus.so.2
#./lib/libnss_nis.so
#./lib/libnss_nis.so.2
#./lib/libpcprofile.so
./lib/libpthread-2.18.so
./lib/libpthread.so
./lib/libpthread.so.0
#./lib/libresolv-2.18.so
#./lib/libresolv.a
#./lib/libresolv_pic.a
#./lib/libresolv_pic.map
#./lib/libresolv.so
#./lib/libresolv.so.2
#./lib/librt-2.18.so
#./lib/librt.so
#./lib/librt.so.1
#./lib/libSegFault.so
#./lib/libstdc++.so
#./lib/libstdc++.so.6
#./lib/libstdc++.so.6.0.18
#./lib/libstdc++.so.6.0.18-gdb.py
#./lib/libthread_db-1.0.so
#./lib/libthread_db.so
#./lib/libthread_db.so.1
./lib/libtirpc.so
./lib/libtirpc.so.1
./lib/libtirpc.so.1.0.10
#./lib/libutil-2.18.so
#./lib/libutil.so
#./lib/libutil.so.1
#./lib/modules/2.6.34/dcs_net/dcs_net.ko
#./lib/modules/2.6.34/extra/e2proc.ko
#./lib/modules/2.6.34/extra/fpanel.ko
#./lib/modules/2.6.34/extra/i2c-gpio.ko
#./lib/modules/2.6.34/extra/irc.ko
#./lib/modules/2.6.34/extra/linuxdvb.ko
#./lib/modules/2.6.34/extra/lirc_dev.ko
#./lib/modules/2.6.34/extra/lnxcssDrv.ko
#./lib/modules/2.6.34/extra/lnxdebugDrv.ko
#./lib/modules/2.6.34/extra/lnxfssDrv.ko
#./lib/modules/2.6.34/extra/lnxKKALDrv.ko
#./lib/modules/2.6.34/extra/lnxnotifyqDrv.ko
#./lib/modules/2.6.34/extra/lnxplatDrv.ko
#./lib/modules/2.6.34/extra/lnxplatLircDev.ko
#./lib/modules/2.6.34/extra/lnxplatLirc.ko
#./lib/modules/2.6.34/extra/lnxplatnativeDrv.ko
#./lib/modules/2.6.34/extra/lnxpvrDrv.ko
#./lib/modules/2.6.34/extra/lnxscsDrv.ko
#./lib/modules/2.6.34/extra/lnxtmasDrv.ko
#./lib/modules/2.6.34/extra/lnxtmvssDrvGPL.ko
#./lib/modules/2.6.34/extra/lnxtmvssDrv.ko
#./lib/modules/2.6.34/extra/vpmfbDrv.ko
#./lib/modules/2.6.34/extra/xs21e.ko
#./lib/modules/2.6.34/extra/xs3145.ko
#./lib/modules/2.6.34/modules.alias
#./lib/modules/2.6.34/modules.alias.bin
#./lib/modules/2.6.34/modules.ccwmap
#./lib/modules/2.6.34/modules.dep
#./lib/modules/2.6.34/modules.dep.bin
#./lib/modules/2.6.34/modules.devname
#./lib/modules/2.6.34/modules.ieee1394map
#./lib/modules/2.6.34/modules.inputmap
#./lib/modules/2.6.34/modules.isapnpmap
#./lib/modules/2.6.34/modules.ofmap
#./lib/modules/2.6.34/modules.pcimap
#./lib/modules/2.6.34/modules.seriomap
#./lib/modules/2.6.34/modules.softdep
#./lib/modules/2.6.34/modules.symbols
#./lib/modules/2.6.34/modules.symbols.bin
#./lib/modules/2.6.34/modules.usbmap
EOF


#此处开始加入MTD UTILS
while read line
do
	if [ -z "$line" -o ${line:0:1} == "#" ];then
		continue
	fi
	rsync -aR "$line" "$TARGET_DIR/"
done << EOF
#src/mtd-utils/out/./usr/sbin/flash_otp_write
#src/mtd-utils/out/./usr/sbin/flash_unlock
#src/mtd-utils/out/./usr/sbin/docfdisk
#src/mtd-utils/out/./usr/sbin/sumtool
#src/mtd-utils/out/./usr/sbin/flash_otp_dump
#src/mtd-utils/out/./usr/sbin/jffs2dump
#src/mtd-utils/out/./usr/sbin/nandtest
#src/mtd-utils/out/./usr/sbin/flash_erase
#src/mtd-utils/out/./usr/sbin/ftl_check
#src/mtd-utils/out/./usr/sbin/rfddump
#src/mtd-utils/out/./usr/sbin/ubimkvol
#src/mtd-utils/out/./usr/sbin/ubiattach
#src/mtd-utils/out/./usr/sbin/nftldump
#src/mtd-utils/out/./usr/sbin/flash_otp_lock
#src/mtd-utils/out/./usr/sbin/ubirsvol
#src/mtd-utils/out/./usr/sbin/ubiupdatevol
#src/mtd-utils/out/./usr/sbin/nanddump
#src/mtd-utils/out/./usr/sbin/ftl_format
#src/mtd-utils/out/./usr/sbin/rfdformat
#src/mtd-utils/out/./usr/sbin/flashcp
#src/mtd-utils/out/./usr/sbin/ubinize
#src/mtd-utils/out/./usr/sbin/mtd_debug
#src/mtd-utils/out/./usr/sbin/serve_image
#src/mtd-utils/out/./usr/sbin/ubirename
#src/mtd-utils/out/./usr/sbin/ubirmvol
#src/mtd-utils/out/./usr/sbin/ubidetach
#src/mtd-utils/out/./usr/sbin/ubicrc32
#src/mtd-utils/out/./usr/sbin/flash_otp_info
#src/mtd-utils/out/./usr/sbin/flash_lock
#src/mtd-utils/out/./usr/sbin/ubinfo
#src/mtd-utils/out/./usr/sbin/mtdinfo
#src/mtd-utils/out/./usr/sbin/flash_eraseall
#src/mtd-utils/out/./usr/sbin/recv_image
#src/mtd-utils/out/./usr/sbin/doc_loadbios
#src/mtd-utils/out/./usr/sbin/ubiformat
#src/mtd-utils/out/./usr/sbin/nftl_format
#src/mtd-utils/out/./usr/sbin/nandwrite
#src/mtd-utils/out/./usr/share/man/man1/mkfs.jffs2.1.gz
EOF


#此处开始加入mkfs for ext4
while read line
do
	if [ -z "$line" -o ${line:0:1} == "#" ];then
		continue
	fi

	rsync -aR "$line" "$TARGET_DIR/"
done << EOF
#src/e2fsprogs/release/./bin/chattr
#src/e2fsprogs/release/./bin/lsattr
#src/e2fsprogs/release/./bin/uuidgen
#src/e2fsprogs/release/./etc/mke2fs.conf
#src/e2fsprogs/release/./lib/e2initrd_helper
#src/e2fsprogs/release/./sbin/badblocks
#src/e2fsprogs/release/./sbin/blkid
#src/e2fsprogs/release/./sbin/debugfs
#src/e2fsprogs/release/./sbin/dumpe2fs
#src/e2fsprogs/release/./sbin/e2freefrag
#src/e2fsprogs/release/./sbin/e2fsck
#src/e2fsprogs/release/./sbin/e2image
#src/e2fsprogs/release/./sbin/e2label
#src/e2fsprogs/release/./sbin/e2undo
#src/e2fsprogs/release/./sbin/e4defrag
#src/e2fsprogs/release/./sbin/filefrag
#src/e2fsprogs/release/./sbin/findfs
#src/e2fsprogs/release/./sbin/fsck
#src/e2fsprogs/release/./sbin/fsck.ext2
#src/e2fsprogs/release/./sbin/fsck.ext3
#src/e2fsprogs/release/./sbin/fsck.ext4
#src/e2fsprogs/release/./sbin/fsck.ext4dev
#src/e2fsprogs/release/./sbin/logsave
#src/e2fsprogs/release/./sbin/mke2fs
#src/e2fsprogs/release/./sbin/mkfs.ext2
src/e2fsprogs/release/./sbin/mkfs.ext3
src/e2fsprogs/release/./sbin/mkfs.ext4
#src/e2fsprogs/release/./sbin/mkfs.ext4dev
#src/e2fsprogs/release/./sbin/mklost+found
#src/e2fsprogs/release/./sbin/resize2fs
#src/e2fsprogs/release/./sbin/tune2fs
#src/e2fsprogs/release/./sbin/uuidd
#src/e2fsprogs/release/./share/info/libext2fs.info.gz
#src/e2fsprogs/release/./share/locale/ca/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/cs/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/de/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/es/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/fr/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/id/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/it/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/nl/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/pl/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/sv/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/tr/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/vi/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/locale/zh_CN/LC_MESSAGES/e2fsprogs.mo
#src/e2fsprogs/release/./share/man/man1/chattr.1
#src/e2fsprogs/release/./share/man/man1/lsattr.1
#src/e2fsprogs/release/./share/man/man1/uuidgen.1
#src/e2fsprogs/release/./share/man/man5/e2fsck.conf.5
#src/e2fsprogs/release/./share/man/man5/mke2fs.conf.5
#src/e2fsprogs/release/./share/man/man8/badblocks.8
#src/e2fsprogs/release/./share/man/man8/blkid.8
#src/e2fsprogs/release/./share/man/man8/debugfs.8
#src/e2fsprogs/release/./share/man/man8/dumpe2fs.8
#src/e2fsprogs/release/./share/man/man8/e2freefrag.8
#src/e2fsprogs/release/./share/man/man8/e2fsck.8
#src/e2fsprogs/release/./share/man/man8/e2image.8
#src/e2fsprogs/release/./share/man/man8/e2label.8
#src/e2fsprogs/release/./share/man/man8/e2undo.8
#src/e2fsprogs/release/./share/man/man8/e4defrag.8
#src/e2fsprogs/release/./share/man/man8/filefrag.8
#src/e2fsprogs/release/./share/man/man8/findfs.8
#src/e2fsprogs/release/./share/man/man8/fsck.8
#src/e2fsprogs/release/./share/man/man8/fsck.ext2.8
#src/e2fsprogs/release/./share/man/man8/fsck.ext3.8
#src/e2fsprogs/release/./share/man/man8/fsck.ext4.8
#src/e2fsprogs/release/./share/man/man8/fsck.ext4dev.8
#src/e2fsprogs/release/./share/man/man8/logsave.8
#src/e2fsprogs/release/./share/man/man8/mke2fs.8
#src/e2fsprogs/release/./share/man/man8/mkfs.ext2.8
#src/e2fsprogs/release/./share/man/man8/mkfs.ext3.8
#src/e2fsprogs/release/./share/man/man8/mkfs.ext4.8
#src/e2fsprogs/release/./share/man/man8/mkfs.ext4dev.8
#src/e2fsprogs/release/./share/man/man8/mklost+found.8
#src/e2fsprogs/release/./share/man/man8/resize2fs.8
#src/e2fsprogs/release/./share/man/man8/tune2fs.8
#src/e2fsprogs/release/./share/man/man8/uuidd.8
EOF

tar zcf "$(dirname $TARGET_DIR)/initrd.tar.gz" -C "$TARGET_DIR" ./

rm -rf "$TARGET_DIR"
#mv "$TARGET_DIR" "$(dirname $TARGET_DIR)/rootfs"

##
#dd if=/dev/zero of=initrd.img bs=1k count=8192
#/sbin/mke2fs -F -v -m0 initrd.img
#mount -o loop initrd.img initrd
#cp -av rootfs/* initrd
#umount
#gzip -9 initrd.img
##
#cd "$TARGET_DIR"
#find . | cpio –o –H newc > ../initrd
#cd ..
#gzip initrd
#mkimage –A arm –O linux –T ramdisk –C none –a 0x20000 –e 0x20000 –n ramdisk –d initrd.gz ramdisk.img
##
#TARGET_FILE="$(pwd)/initrd.img"
#LOOP_DIR="$(pwd)/loop"
#dd if=/dev/zero of="$TARGET_FILE" bs=1M count=32    #------初始化32M内存空间
#mkfs.ext3 -m0 "$TARGET_FILE"                        #------格式化为ext3
#mkdir -p "$LOOP_DIR"
#sudo mount -o loop "$TARGET_FILE" "$LOOP_DIR"       #------挂载
#rsync -a "$TARGET_DIR/" "$LOOP_DIR/"
#sudo umount "$LOOP_DIR"
#rm "$LOOP_DIR" -rf
#gzip -9 initrd.img
