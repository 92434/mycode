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

#RM_ALL="rm -rf"
RM_ALL="echo rm -rf"

function exec_cmd() {
	echo "$@"
	$@
}

function package_exist() {
	if [ -z "$1" ]; then
		return
	fi
	
	adb shell pm list packages -f -u | grep -i "$1"
}

function clear_app() {
	exec_cmd adb shell pm clear $1
}

function rm_system_app() {
	exec_cmd adb shell "PATH=/data/bin;if [ -f $1 ]; then mv $1 $1.bak;fi"
	exec_cmd adb shell "PATH=/data/bin;if [ -d /data/data/$2 ]; then $RM_ALL /data/data/$2; fi"
}

function rm_dir() {
	exec_cmd adb shell "PATH=/data/bin;if [ -d $1 ]; then $RM_ALL $1; fi"
}

function uninstall_app() {
	package_exist $1
	if [ -n "$(package_exist $1)" ]; then
		exec_cmd adb shell pm uninstall $1
	else
		echo "not exist $1"
	fi
}

function main() {
	local list_clear=(
"com.android.browser"
"com.kugou.android"
"com.sinovatech.unicom.ui"
"com.kingpoint.gmcchh"
"com.example.android.apis"
"com.cleanmaster.sdk"
"com.android.email"
"com.handsgo.jiakao.android"
"com.miui.yellowpage"
"com.miui.mipub"
"com.miui.player"
"com.xiaomi.market"
"com.icbc"
"com.miui.video"
"com.MobileTicket"
"com.jeejen.container.miui"
"com.jeejen.store"
"com.jeejen.knowledge"
"com.jeejen.family.miui"
"com.miui.touchassistant"
)

	local list_mv_rm_system_app=(
"/system/app/SogouInput.apk:com.sohu.inputmethod.sogou.xiaomi"
"/system/priv-app/MiuiVoip.apk:com.miui.voip"
"/system/app/GameCenter.apk:com.xiaomi.gamecenter"
"/system/priv-app/MiGameCenterSDKService.apk:com.xiaomi.gamecenter.sdk.service"
"/system/app/XMPass.apk:com.xiaomi.pass"
"/system/app/PaymentService.apk:com.xiaomi.payment"
)

	local list_rm_dir=(
"/data/miui/app"
)
	local list_uninstall_app=(
"com.mi.vtalk"
"com.funshion.video.player"
"com.pplive.androidsdk.mi"
"com.miui.video.plugin"
"com.xiaomi.payment"
"com.xiaomi.jr"
"com.xiaomi.smarthome"
"com.miui.miuibbs"
"com.xiaomi.o2o"
"com.wali.live"
)

	#echo adb shell busybox mount -o remount,rw,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system
	#adb shell busybox mount -o remount,rw,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system

	local i;

	for i in ${list_clear[@]};do
		clear_app $i
	done

	for i in ${list_mv_rm_system_app[@]};do
		rm_system_app ${i//:/ }
	done

	for i in ${list_rm_dir[@]};do
		rm_dir $i
	done

	for i in ${list_uninstall_app[@]};do
		uninstall_app $i
	done

	#echo adb shell busybox mount -o remount,ro,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system
	#adb shell busybox mount -o remount,ro,seclabel,noatime,noauto_da_alloc,commit=1,data=ordered -t ext4 /emmc@android /system
}

main $@
