#!/bin/sh

function clear() {
	rm -rf aclocal.m4 AUTHORS autom4te.cache autoscan.log ChangeLog compile config.h config.h.in config.h.in~ config.log config.guess config.sub config.status configure configure.ac configure.in COPYING depcomp INSTALL install-sh $(find . -type f -name Makefile.in 2>/dev/null) missing NEWS README stamp-h1 .deps
	la
}

function process_configure() {
	local pkgname=$1
	local version=$2

	mv configure.scan configure.in
	sed -i "s/FULL-PACKAGE-NAME/$pkgname/g" configure.in
	sed -i "s/VERSION/$version/g" configure.in
	sed -i "/AC_CONFIG_HEADERS/a \AM_INIT_AUTOMAKE($pkgname, $version)" configure.in
	sed -i "/AC_PROG_CC/a \AC_PROG_RANLIB" configure.in
	sed -i "/AC_PROG_RANLIB/a \AC_PROG_LIBTOOL" configure.in
	sed -i "
	/AC_OUTPUT/d
	" configure.in
	sed -i "
	s/AC_CONFIG_FILES/AC_OUTPUT/g
	" configure.in
	vi configure.in
}

function parse_opt() {
	pkgname="test"
	version="1.0"

	OPTIND=0
	OPTERR=1
	while getopts hp:v: arguments 2>/dev/null; do
		echo "\$arguments is $arguments"
		echo "\$OPTIND is $OPTIND;\$OPTARG is $OPTARG;\$OPTERR is $OPTERR"
		case $arguments in
			h) echo "option h";;
			p) echo "option p = $OPTARG";pkgname=$OPTARG;;
			v) echo "option v = $OPTARG";version=$OPTARG;;
			?) echo "Usage: $0 [-h] [-p pkgname] [-v version]";;
		esac
		echo
		echo
	done
	echo "\$OPTIND is $OPTIND;\$OPTARG is $OPTARG;\$OPTERR is $OPTERR"
	shift $(($OPTIND - 1))
	echo $@

	export pkgname
	export version
}

function gen() {
	parse_opt "$@"
	clear
	autoscan
	process_configure $pkgname $version
	touch NEWS README AUTHORS ChangeLog
	aclocal
	autoheader
	autoconf
	automake -a
	la
}
