#!/bin/sh

#usage: pm list packages [-f] [-d] [-e] [-s] [-3] [-i] [-u] [--user USER_ID] [FILTER]
#       pm list permission-groups
#       pm list permissions [-g] [-f] [-d] [-u] [GROUP]
#       pm list instrumentation [-f] [TARGET-PACKAGE]
#       pm list features
#       pm list libraries
#       pm list users
#       pm path PACKAGE
#       pm dump PACKAGE
#       pm install [-l] [-r] [-t] [-i INSTALLER_PACKAGE_NAME] [-s] [-f]
#                  [--algo <algorithm name> --key <key-in-hex> --iv <IV-in-hex>]
#                  [--originating-uri <URI>] [--referrer <URI>] PATH
#       pm uninstall [-k] PACKAGE
#       pm clear [--user USER_ID] PACKAGE
#       pm enable [--user USER_ID] PACKAGE_OR_COMPONENT
#       pm disable [--user USER_ID] PACKAGE_OR_COMPONENT
#       pm disable-user [--user USER_ID] PACKAGE_OR_COMPONENT
#       pm disable-until-used [--user USER_ID] PACKAGE_OR_COMPONENT
#       pm block [--user USER_ID] PACKAGE_OR_COMPONENT
#       pm unblock [--user USER_ID] PACKAGE_OR_COMPONENT
#       pm grant PACKAGE PERMISSION
#       pm revoke PACKAGE PERMISSION
#       pm set-install-location [0/auto] [1/internal] [2/external]
#       pm get-install-location
#       pm set-permission-enforced PERMISSION [true|false]
#       pm trim-caches DESIRED_FREE_SPACE
#       pm create-user USER_NAME
#       pm remove-user USER_ID
#       pm get-max-users
#
#pm list packages: prints all packages, optionally only
#  those whose package name contains the text in FILTER.  Options:
#    -f: see their associated file.
#    -d: filter to only show disbled packages.
#    -e: filter to only show enabled packages.
#    -s: filter to only show system packages.
#    -3: filter to only show third party packages.
#    -i: see the installer for the packages.
#    -u: also include uninstalled packages.
#
#pm list permission-groups: prints all known permission groups.
#
#pm list permissions: prints all known permissions, optionally only
#  those in GROUP.  Options:
#    -g: organize by group.
#    -f: print all information.
#    -s: short summary.
#    -d: only list dangerous permissions.
#    -u: list only the permissions users will see.
#
#pm list instrumentation: use to list all test packages; optionally
#  supply <TARGET-PACKAGE> to list the test packages for a particular
#  application.  Options:
#    -f: list the .apk file for the test package.
#
#pm list features: prints all features of the system.
#
#pm list users: prints all users on the system.
#
#pm path: print the path to the .apk of the given PACKAGE.
#
#pm dump: print system state associated w ith the given PACKAGE.
#
#pm install: installs a package to the system.  Options:
#    -l: install the package with FORWARD_LOCK.
#    -r: reinstall an exisiting app, keeping its data.
#    -t: allow test .apks to be installed.
#    -i: specify the installer package name.
#    -s: install package on sdcard.
#    -f: install package on internal flash.
#    -d: allow version code downgrade.
#
#pm uninstall: removes a package from the system. Options:
#    -k: keep the data and cache directories around after package removal.
#
#pm clear: deletes all data associated with a package.
#
#pm enable, disable, disable-user, disable-until-used: these commands
#  change the enabled state of a given package or component (written
#  as "package/class").
#
#pm grant, revoke: these commands either grant or revoke permissions
#  to applications.  Only optional permissions the application has
#  declared can be granted or revoked.
#
#pm get-install-location: returns the current install location.
#    0 [auto]: Let system decide the best location
#    1 [internal]: Install on internal device storage
#    2 [external]: Install on external media
#
#pm set-install-location: changes the default install location.
#  NOTE: this is only intended for debugging; using this can cause
#  applications to break and other undersireable behavior.
#    0 [auto]: Let system decide the best location
#    1 [internal]: Install on internal device storage
#    2 [external]: Install on external media
#
#pm trim-caches: trim cache files to reach the given free space.
#
#pm create-user: create a new user with the given USER_NAME,
#  printing the new user identifier of the user.
#
#pm remove-user: remove the user with the given USER_IDENTIFIER,
#  deleting all data associated with that user

function package_exist() {
	if [ -z "$1" ]; then
		return
	fi
	
	adb shell pm list packages -f -u | grep -i "$1"
}

function main() {
	#local ip=

	#ip="$1"
	#if [ -z "$ip" ]; then
	#	echo "ip:$ip";
	#	return;
	#fi

	#adb connect $*
	#adb root

	echo adb shell busybox mount -o remount,rw,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system
	adb shell busybox mount -o remount,rw,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system

	echo adb shell pm clear com.android.browser
	adb shell pm clear com.android.browser

	echo adb shell pm clear com.kugou.android
	adb shell pm clear com.kugou.android

	echo adb shell pm clear com.sinovatech.unicom.ui
	adb shell pm clear com.sinovatech.unicom.ui

	echo adb shell pm clear com.kingpoint.gmcchh
	adb shell pm clear com.kingpoint.gmcchh

	echo adb shell pm clear com.example.android.apis
	adb shell pm clear com.example.android.apis

	echo adb shell pm clear com.cleanmaster.sdk
	adb shell pm clear com.cleanmaster.sdk

	echo adb shell pm clear com.android.email
	adb shell pm clear com.android.email

	echo adb shell pm clear com.handsgo.jiakao.android
	adb shell pm clear com.handsgo.jiakao.android

	echo adb shell pm clear com.miui.yellowpage
	adb shell pm clear com.miui.yellowpage

	echo adb shell pm clear com.miui.mipub
	adb shell pm clear com.miui.mipub

	echo adb shell pm clear com.miui.player
	adb shell pm clear com.miui.player

	echo adb shell pm clear com.xiaomi.market
	adb shell pm clear com.xiaomi.market

	echo adb shell pm clear com.icbc
	adb shell pm clear com.icbc

	echo adb shell pm clear com.miui.video
	adb shell pm clear com.miui.video

	echo adb shell pm clear com.MobileTicket
	adb shell pm clear com.MobileTicket

	echo adb shell pm clear com.jeejen.container.miui
	adb shell pm clear com.jeejen.container.miui

	echo adb shell pm clear com.jeejen.store
	adb shell pm clear com.jeejen.store

	echo adb shell pm clear com.jeejen.knowledge
	adb shell pm clear com.jeejen.knowledge

	echo adb shell pm clear com.jeejen.family.miui
	adb shell pm clear com.jeejen.family.miui

	echo adb shell pm clear com.miui.touchassistant
	adb shell pm clear com.miui.touchassistant

	echo adb shell "PATH=/data/bin;if [ -f /system/app/SogouInput.apk ]; then mv /system/app/SogouInput.apk /system/app/SogouInput.apk.bak;fi"
	adb shell "PATH=/data/bin;if [ -f /system/app/SogouInput.apk ]; then mv /system/app/SogouInput.apk /system/app/SogouInput.apk.bak;fi"

	echo adb shell "PATH=/data/bin;if [ -d /data/data/com.sohu.inputmethod.sogou.xiaomi ]; then rm -rf /data/data/com.sohu.inputmethod.sogou.xiaomi; fi"
	adb shell "PATH=/data/bin;if [ -d /data/data/com.sohu.inputmethod.sogou.xiaomi ]; then rm -rf /data/data/com.sohu.inputmethod.sogou.xiaomi; fi"
	
	echo adb shell "PATH=/data/bin;if [ -f /system/priv-app/MiuiVoip.apk ]; then mv /system/priv-app/MiuiVoip.apk /system/priv-app/MiuiVoip.apk.bak;fi"
	adb shell "PATH=/data/bin;if [ -f /system/priv-app/MiuiVoip.apk ]; then mv /system/priv-app/MiuiVoip.apk /system/priv-app/MiuiVoip.apk.bak;fi"

	echo adb shell "PATH=/data/bin;if [ -d /data/data/com.miui.voip ]; then rm -rf /data/data/com.miui.voip; fi"
	adb shell "PATH=/data/bin;if [ -d /data/data/com.miui.voip ]; then rm -rf /data/data/com.miui.voip; fi"

	echo adb shell "PATH=/data/bin;if [ -f /system/app/GameCenter.apk ]; then mv /system/app/GameCenter.apk /system/app/GameCenter.apk.bak;fi"
	adb shell "PATH=/data/bin;if [ -f /system/app/GameCenter.apk ]; then mv /system/app/GameCenter.apk /system/app/GameCenter.apk.bak;fi"

	echo adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.gamecenter ]; then rm -rf /data/data/com.xiaomi.gamecenter; fi"
	adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.gamecenter ]; then rm -rf /data/data/com.xiaomi.gamecenter; fi"

	echo adb shell "PATH=/data/bin;if [ -f /system/priv-app/MiGameCenterSDKService.apk ]; then mv /system/priv-app/MiGameCenterSDKService.apk /system/priv-app/MiGameCenterSDKService.apk.bak;fi"
	adb shell "PATH=/data/bin;if [ -f /system/priv-app/MiGameCenterSDKService.apk ]; then mv /system/priv-app/MiGameCenterSDKService.apk /system/priv-app/MiGameCenterSDKService.apk.bak;fi"

	echo adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.gamecenter.sdk.service ]; then rm -rf /data/data/com.xiaomi.gamecenter.sdk.service; fi"
	adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.gamecenter.sdk.service ]; then rm -rf /data/data/com.xiaomi.gamecenter.sdk.service; fi"

	echo adb shell "PATH=/data/bin;if [ -f /system/app/XMPass.apk ]; then mv /system/app/XMPass.apk /system/app/XMPass.apk.bak;fi"
	adb shell "PATH=/data/bin;if [ -f /system/app/XMPass.apk ]; then mv /system/app/XMPass.apk /system/app/XMPass.apk.bak;fi"

	echo adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.pass ]; then rm -rf /data/data/com.xiaomi.pass; fi"
	adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.pass ]; then rm -rf /data/data/com.xiaomi.pass; fi"

	echo adb shell "PATH=/data/bin;if [ -f /system/app/PaymentService.apk ]; then mv /system/app/PaymentService.apk /system/app/PaymentService.apk.bak;fi"
	adb shell "PATH=/data/bin;if [ -f /system/app/PaymentService.apk ]; then mv /system/app/PaymentService.apk /system/app/PaymentService.apk.bak;fi"

	echo adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.payment ]; then rm -rf /data/data/com.xiaomi.payment; fi"
	adb shell "PATH=/data/bin;if [ -d /data/data/com.xiaomi.payment ]; then rm -rf /data/data/com.xiaomi.payment; fi"

	echo adb shell "PATH=/data/bin;if [ -d /data/miui/app ]; then rm -rf /data/miui/app; fi"
	adb shell "PATH=/data/bin;if [ -d /data/miui/app ]; then rm -rf /data/miui/app; fi"

	if [ -n "$(package_exist com.mi.vtalk)" ]; then
		echo adb shell pm uninstall com.mi.vtalk
		adb shell pm uninstall com.mi.vtalk
	fi

	if [ -n "$(package_exist com.funshion.video.player)" ]; then
		echo adb shell pm uninstall com.funshion.video.player
		adb shell pm uninstall com.funshion.video.player
	fi

	if [ -n "$(package_exist com.pplive.androidsdk.mi)" ]; then
		echo adb shell pm uninstall com.pplive.androidsdk.mi
		adb shell pm uninstall com.pplive.androidsdk.mi
	fi

	if [ -n "$(package_exist com.miui.video.plugin)" ]; then
		echo adb shell pm uninstall com.miui.video.plugin
		adb shell pm uninstall com.miui.video.plugin
	fi

	if [ -n "$(package_exist com.xiaomi.pass)" ]; then
		echo adb shell pm uninstall com.xiaomi.pass
		adb shell pm uninstall com.xiaomi.pass
	fi

	if [ -n "$(package_exist com.xiaomi.payment)" ]; then
		echo adb shell pm uninstall com.xiaomi.payment
		adb shell pm uninstall com.xiaomi.payment
	fi

	if [ -n "$(package_exist com.xiaomi.jr)" ]; then
		echo adb shell pm uninstall com.xiaomi.jr
		adb shell pm uninstall com.xiaomi.jr
	fi

	if [ -n "$(package_exist com.xiaomi.smarthome)" ]; then
		echo adb shell pm uninstall com.xiaomi.smarthome
		adb shell pm uninstall com.xiaomi.smarthome
	fi

	if [ -n "$(package_exist com.miui.miuibbs)" ]; then
		echo adb shell pm uninstall com.miui.miuibbs
		adb shell pm uninstall com.miui.miuibbs
	fi

	if [ -n "$(package_exist com.xiaomi.o2o)" ]; then
		echo adb shell pm uninstall com.xiaomi.o2o
		adb shell pm uninstall com.xiaomi.o2o
	fi

	if [ -n "$(package_exist com.wali.live)" ]; then
		echo adb shell pm uninstall com.wali.live
		adb shell pm uninstall com.wali.live
	fi

	echo adb shell busybox mount -o remount,ro,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system
	adb shell busybox mount -o remount,ro,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system
}

main $@
