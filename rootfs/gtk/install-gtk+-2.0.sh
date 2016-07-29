#!/bin/sh
set -x

#-------------------------------------------------------------------------------
# check arguments
#-------------------------------------------------------------------------------
case $1 in
--help|-help|help|\?)
echo Usage: `basename $0` [prefix]
cat << "EOF"

    prefix - install path

EOF
exit
;;
esac

#-------------------------------------------------------------------------------
# check linux issue
#-------------------------------------------------------------------------------
case "`head -1 /etc/issue | awk '{ print $1 }'`" in
Ubuntu)
INSTALL="sudo apt-get install"
;;
Fedora)
INSTALL="sudo yum install"
;;
*)
echo "Unrecoginzed issue"; exit;
;;
esac

#-------------------------------------------------------------------------------
# install automake
#-------------------------------------------------------------------------------
if [ -z "`command -v automake`" ]; then
$INSTALL automake
fi

#-------------------------------------------------------------------------------
# install autoconf
#-------------------------------------------------------------------------------
if [ -z "`command -v autoconf`" ]; then
$INSTALL autoconf
fi

#-------------------------------------------------------------------------------
# install autoheader
#-------------------------------------------------------------------------------
if [ -z "`command -v autoheader`" ]; then
$INSTALL autoheader
fi

#-------------------------------------------------------------------------------
# install aclocal
#-------------------------------------------------------------------------------
if [ -z "`command -v aclocal`" ]; then
$INSTALL aclocal
fi

#-------------------------------------------------------------------------------
# install libtoolize
#-------------------------------------------------------------------------------
if [ -z "`command -v libtoolize`" ]; then
$INSTALL libtoolize
fi

#-------------------------------------------------------------------------------
# install pkg-config
#-------------------------------------------------------------------------------
if [ -z "`command -v pkg-config`" ]; then
$INSTALL pkg-config
fi

#-------------------------------------------------------------------------------
# install GNU make
#-------------------------------------------------------------------------------
if [ -z "`command -v make`" ]; then
$INSTALL make
fi

#-------------------------------------------------------------------------------
# set compile environment variables
#-------------------------------------------------------------------------------
PREFIX=/usr/local

export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
export LD_LIBRARY_PATH=$PREFIX/lib:/usr/lib

#-------------------------------------------------------------------------------
# install local zlib
#-------------------------------------------------------------------------------
#if [ ! -e $PKG_CONFIG_PATH/zlib.pc ]; then
#echo "installing local zlib......"
#if [ ! -d zlib-1.2.5 ]; then
#if [ ! -e zlib-1.2.5.tar.bz2 ]; then
#wget http://www.zlib.net/zlib-1.2.5.tar.bz2
#fi
#tar -xjf zlib-1.2.5.tar.bz2
#fi
#cd zlib-1.2.5 && ./configure --prefix=$PREFIX && make && make install && cd .. || exit
#rm -rf zlib-1.2.5
#fi

#-------------------------------------------------------------------------------
# install local glib
# 
# dependencies: {
#   sys-devel/gettext,
#   dev-util/pkg-config,
#   sys-apps/dbus
# }
#-------------------------------------------------------------------------------
#if [ ! -e $PKG_CONFIG_PATH/glib-2.0.pc ]; then
#echo "installing local glib......"
#if [ ! -d glib-2.28.3 ]; then
#if [ ! -e glib-2.28.3.tar.bz2 ]; then
#wget ftp://ftp.gtk.org/pub/glib/2.28/glib-2.28.3.tar.bz2
#fi
#tar -xjf glib-2.28.3.tar.bz2
#fi
#cd glib-2.28.3 && ./configure --prefix=$PREFIX && make && make install && cd .. || exit
#rm -rf glib-2.28.3
#fi

#-------------------------------------------------------------------------------
# install local libxml2
# 
# dependencies: {
#   sys-libs/zlib,
#   sys-devel/libtool,
#   sys-devel/autoconf,
#   sys-devel/automake
# }
#-------------------------------------------------------------------------------
#if [ ! -e $PKG_CONFIG_PATH/libxml-2.0.pc ]; then
#echo "installing local libxml2......"
#if [ ! -d libxml2-2.7.8 ]; then
#if [ ! -e libxml2-2.7.8.tar.gz  ]; then
#wget ftp://www.xmlsoft.org/libxml2/libxml2-2.7.8.tar.gz 
#fi
#tar -xzf libxml2-2.7.8.tar.gz 
#fi
#cd libxml2-2.7.8 && ./configure --prefix=$PREFIX && make && make install && cd .. || exit
#rm -rf libxml2-2.7.8
#fi

#-------------------------------------------------------------------------------
# install local libpng
# 
# dependencies: {
#   app-arch/xz
# }
#-------------------------------------------------------------------------------
#if [ ! -e $PKG_CONFIG_PATH/libpng.pc ]; then
#echo "installing local libpng......"
#if [ ! -d libpng-1.2.44 ]; then
#if [ ! -e libpng-1.2.44.tar.bz2 ]; then
#wget ftp://ftp.simplesystems.org/pub/png/src/libpng-1.2.44.tar.bz2
#fi
#tar -xjf libpng-1.2.44.tar.bz2
#fi
#cd libpng-1.2.44 && ./configure --prefix=$PREFIX && make && make install && cd .. || exit
#rm -rf libpng-1.2.44
#fi

#-------------------------------------------------------------------------------
# install local jpeg
#-------------------------------------------------------------------------------
#if [ ! -e $PREFIX/lib/libjpeg.so ]; then
#echo "installing local jpeg......"
#if [ ! -d jpeg-8c ]; then
#if [ ! -e jpegsrc.v8c.tar.gz ]; then
#wget http://www.ijg.org/files/jpegsrc.v8c.tar.gz
#fi
#tar -xzf jpegsrc.v8c.tar.gz
#fi
#cd jpeg-8c && ./configure --prefix=$PREFIX && make && make install && cd .. || exit
#rm -rf jpeg-8c
#fi

#-------------------------------------------------------------------------------
# install local tiff
#-------------------------------------------------------------------------------
#if [ ! -e $PREFIX/lib/libtiff.so ]; then
#echo "installing local tiff......"
#if [ ! -d tiff-3.7.4 ]; then
#if [ ! -e tiff-3.7.4.tar.gz ]; then
#wget ftp://ftp.gtk.org/pub/gtk/v2.10/dependencies/tiff-3.7.4.tar.gz
#fi
#tar -xzf tiff-3.7.4.tar.gz
#fi
#cd tiff-3.7.4 && ./configure --prefix=$PREFIX && make && make install && cd .. || exit
#rm -rf tiff-3.7.4
#fi

#-------------------------------------------------------------------------------
# install local gdk-pixbuf
# 
# dependencies: {
#   dev-libs/glib,
#   media-libs/libpng,
#   media-libs/jpeg,
#   media-libs/tiff
# }
#-------------------------------------------------------------------------------
#if [ ! -e $PKG_CONFIG_PATH/gdk-pixbuf-2.0.pc ]; then
#echo "installing local gdk-pixbuf......"
#if [ ! -d gdk-pixbuf-2.22.1 ]; then
#if [ ! -e gdk-pixbuf-2.22.1.tar.bz2 ]; then
#wget http://ftp.gnome.org/pub/gnome/sources/gdk-pixbuf/2.22/gdk-pixbuf-2.22.1.tar.bz2
#fi
#tar -xjf gdk-pixbuf-2.22.1.tar.bz2
#fi
#cd gdk-pixbuf-2.22.1 && ./configure --prefix=$PREFIX && make && make install && cd .. || exit
#rm -rf gdk-pixbuf-2.22.1 
#fi

#-------------------------------------------------------------------------------
# set xcompile environment variables
#-------------------------------------------------------------------------------
if [ ! -z $1 ]; then
  PREFIX=$1
else
  PREFIX=$PWD/gtk+/arm
fi

export PATH=$PATH:$PREFIX/bin
export HOST=arm-xilinx-linux-gnueabi
export BUILD=i686-linux
export CC=arm-xilinx-linux-gnueabi-gcc
export LD=arm-xilinx-linux-gnueabi-ld
export AR=arm-xilinx-linux-gnueabi-ar
export AS=arm-xilinx-linux-gnueabi-as
export NM=arm-xilinx-linux-gnueabi-nm
export CXX=arm-xilinx-linux-gnueabi-g++
export RANLIB=arm-xilinx-linux-gnueabi-ranlib
export LD_LIBRARY_PATH=$PREFIX/lib
export LD_RUN_PATH=$LD_LIBRARY_PATH
export CFLAGS="-g -I$PREFIX/include"
export CPPFLAGS=-I$PREFIX/include
export LDFLAGS="-L$LD_LIBRARY_PATH -Wl,-rpath -Wl,$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH=$LD_LIBRARY_PATH/pkgconfig

#-------------------------------------------------------------------------------
# Install tslib
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/tslib.pc ]; then
echo "installing tslib......"
if [ ! -d tslib ]; then
if [ ! -e tslib-1.4.tar.gz ]; then
wget http://tslib.googlecode.com/files/tslib-1.4.tar.gz
fi
tar -xzf tslib-1.4.tar.gz
fi
cd tslib
./autogen.sh
cat > $HOST.cache << "EOF"
ac_cv_func_malloc_0_nonnull=yes
EOF
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX --cache-file=$HOST.cache && make && make install || exit
cd ..
rm -rf tslib
fi

#-------------------------------------------------------------------------------
# install zlib
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/zlib.pc ]; then
echo "installing zlib......"
if [ ! -d zlib-1.2.5 ]; then
if [ ! -e zlib-1.2.5.tar.bz2 ]; then
wget http://www.zlib.net/zlib-1.2.5.tar.bz2
fi
tar -xjf zlib-1.2.5.tar.bz2
fi
cd zlib-1.2.5
./configure --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf zlib-1.2.5
fi

#-------------------------------------------------------------------------------
# install libxml2
# 
# dependencies: {
#   sys-libs/zlib,
#   sys-devel/libtool,
#   sys-devel/autoconf,
#   sys-devel/automake
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/libxml-2.0.pc ]; then
echo "installing libxml2......"
if [ ! -d libxml2-2.7.8 ]; then
if [ ! -e libxml2-2.7.8.tar.gz  ]; then
wget ftp://www.xmlsoft.org/libxml2/libxml2-2.7.8.tar.gz 
fi
tar -xzf libxml2-2.7.8.tar.gz 
fi
cd libxml2-2.7.8
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libxml2-2.7.8
fi

#-------------------------------------------------------------------------------
# install expat
#-------------------------------------------------------------------------------
if [ ! -e $PREFIX/lib/libexpat.so ]; then
echo "installing expat......"
if [ ! -d expat-2.0.1 ]; then
if [ ! -e expat-2.0.1.tar.gz ]; then
wget http://nchc.dl.sourceforge.net/project/expat/expat/2.0.1/expat-2.0.1.tar.gz
fi
tar -xzf expat-2.0.1.tar.gz
fi
cd expat-2.0.1
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf expat-2.0.1
fi

#-------------------------------------------------------------------------------
# install dbus
# 
# dependencies: {
#   app-text/docbook-xml-dtd,
#   app-text/xmlto,
#   sys-devel/gettext,
#   dev-util/pkg-config,
#   dev-libs/expat,
#   group/messagebus,
#   user/messagebus,
#   group/plugdev,
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/dbus-1.pc ]; then
echo "installing dbus......"
if [ ! -d dbus-1.4.6 ]; then
if [ ! -e dbus-1.4.6.tar.gz ]; then
wget http://dbus.freedesktop.org/releases/dbus/dbus-1.4.6.tar.gz
fi
tar -xzf dbus-1.4.6.tar.gz
fi
cd dbus-1.4.6
cat > $HOST.cache << "EOF"
ac_cv_have_abstract_sockets=yes
EOF
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX \
--cache-file=$HOST.cache --without-x && make && make install || exit
cd ..
rm -rf dbus-1.4.6
fi

#-------------------------------------------------------------------------------
# install glib
# 
# dependencies: {
#   sys-devel/gettext,
#   dev-util/pkg-config,
#   sys-apps/dbus
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/glib-2.0.pc ]; then
echo "installing GLib......"
if [ ! -d glib-2.28.7 ]; then
if [ ! -e glib-2.28.7.tar.xz ]; then
wget https://git.gnome.org/browse/glib/snapshot/glib-2.28.7.tar.xz
fi
tar -xJf glib-2.28.7.tar.xz
cat >.glib.patch<<EOF
--- glib-2.28.7/gio/Makefile.am	2011-05-21 12:14:42.000000000 +0800
+++ glib-2.28.7/gio/Makefile.am	2014-09-16 16:41:02.363316857 +0800
@@ -598,6 +598,7 @@
 	\$(top_builddir)/glib/libglib-2.0.la		\\
 	\$(top_builddir)/gobject/libgobject-2.0.la	\\
 	\$(top_builddir)/gmodule/libgmodule-2.0.la	\\
+	\$(top_builddir)/gthread/libgthread-2.0.la	\\
 	libgio-2.0.la					\\
 	\$(NULL)
 
@@ -611,6 +612,8 @@
 gsettings_LDADD = \\
 	\$(top_builddir)/glib/libglib-2.0.la		\\
 	\$(top_builddir)/gobject/libgobject-2.0.la	\\
+	\$(top_builddir)/gmodule/libgmodule-2.0.la	\\
+	\$(top_builddir)/gthread/libgthread-2.0.la	\\
 	libgio-2.0.la
 gsettings_SOURCES = gsettings-tool.c
 
@@ -624,6 +627,8 @@
 gdbus_SOURCES = gdbus-tool.c
 gdbus_LDADD = libgio-2.0.la \\
 	\$(top_builddir)/glib/libglib-2.0.la 		\\
+	\$(top_builddir)/gthread/libgthread-2.0.la	\\
+	\$(top_builddir)/gmodule/libgmodule-2.0.la	\\
 	\$(top_builddir)/gobject/libgobject-2.0.la
 
 completiondir = \$(sysconfdir)/bash_completion.d
--- glib-2.28.7/gio/tests/Makefile.am	2011-05-21 12:14:42.000000000 +0800
+++ glib-2.28.7/gio/tests/Makefile.am	2014-09-16 16:43:46.459311720 +0800
@@ -15,6 +15,7 @@
 	\$(top_builddir)/glib/libglib-2.0.la 		\\
 	\$(top_builddir)/gthread/libgthread-2.0.la	\\
 	\$(top_builddir)/gobject/libgobject-2.0.la	\\
+	\$(top_builddir)/gmodule/libgmodule-2.0.la 	\\
 	\$(top_builddir)/gio/libgio-2.0.la
 
 TEST_PROGS +=	 		\\
--- glib-2.28.7/tests/Makefile.am	2011-05-21 12:14:42.000000000 +0800
+++ glib-2.28.7/tests/Makefile.am	2014-09-16 16:49:58.743300064 +0800
@@ -77,7 +77,7 @@
 assert_msg_test_LDADD = \$(libglib)
 if ENABLE_TIMELOOP
 timeloop_LDADD = \$(libglib)
-timeloop_closure_LDADD = \$(libglib) \$(libgobject)
+timeloop_closure_LDADD = \$(libglib) \$(libgobject) \$(libgthread)
 endif
 
 test_programs =					\\
EOF
patch -p0 < .glib.patch
rm .glib.patch
fi
cd glib-2.28.7
./autogen.sh
cat > $HOST.cache << "EOF"
ac_cv_type_long_long=yes
glib_cv_stack_grows=no
glib_cv_has__inline=yes
glib_cv_working_bcopy=no
glib_cv_uscore=no
ac_cv_func_posix_getpwuid_r=yes
ac_cv_func_posix_getgrgid_r=yes
EOF
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX --cache-file=$HOST.cache --disable-silent-rules && make && make install || exit
cd ..
rm -rf glib-2.28.7
fi

#-------------------------------------------------------------------------------
# install atk
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/atk.pc ]; then
echo "installing atk......"
if [ ! -d atk-1.33.6 ]; then
if [ ! -e atk-1.33.6.tar.bz2 ]; then
wget ftp://ftp.acc.umu.se/pub/gnome/sources/atk/1.33/atk-1.33.6.tar.bz2
fi
tar -xjf atk-1.33.6.tar.bz2
fi
cd atk-1.33.6
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf atk-1.33.6
fi

#-------------------------------------------------------------------------------
# install libpng
# 
# dependencies: {
#   app-arch/xz
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/libpng.pc ]; then
echo "installing libpng......"
if [ ! -d libpng-1.2.44 ]; then
if [ ! -e libpng-1.2.44.tar.bz2 ]; then
wget ftp://ftp.simplesystems.org/pub/png/src/libpng-1.2.44.tar.bz2
#wget http://ncu.dl.sourceforge.net/project/libpng/libpng15/1.5.1/libpng-1.5.1.tar.bz2
fi
tar -xjf libpng-1.2.44.tar.bz2
fi
cd libpng-1.2.44
cat > $HOST.cache << "EOF"
ac_cv_lib_z_zlibVersion=yes
EOF
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX \
--cache-file=$HOST.cache && make && make install || exit
cd ..
rm -rf libpng-1.2.44
fi

#-------------------------------------------------------------------------------
# install jpeg
#-------------------------------------------------------------------------------
if [ ! -e $PREFIX/lib/libjpeg.so ]; then
echo "installing jpeg......"
if [ ! -d jpeg-8c ]; then
if [ ! -e jpegsrc.v8c.tar.gz ]; then
wget http://www.ijg.org/files/jpegsrc.v8c.tar.gz
fi
tar -xzf jpegsrc.v8c.tar.gz
fi
cd jpeg-8c
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf jpeg-8c
fi

#-------------------------------------------------------------------------------
# install tiff
#-------------------------------------------------------------------------------
if [ ! -e $PREFIX/lib/libtiff.so ]; then
echo "installing tiff......"
if [ ! -d tiff-3.7.4 ]; then
if [ ! -e tiff-3.7.4.tar.gz ]; then
wget ftp://ftp.gtk.org/pub/gtk/v2.10/dependencies/tiff-3.7.4.tar.gz
fi
tar -xzf tiff-3.7.4.tar.gz
fi
cd tiff-3.7.4
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX --enable-shared && make && make install || exit
cd ..
rm -rf tiff-3.7.4
fi

#-------------------------------------------------------------------------------
# install pixman
# 
# dependencies: {
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/pixman-1.pc ]; then
echo "installing pixman......"
if [ ! -d pixman-0.20.2 ]; then
if [ ! -e pixman-0.20.2.tar.gz ]; then
wget http://www.cairographics.org/releases/pixman-0.20.2.tar.gz
fi
tar -xzf pixman-0.20.2.tar.gz
fi
cd pixman-0.20.2
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX --disable-gtk && make && make install || exit
cd ..
rm -rf pixman-0.20.2
fi

#-------------------------------------------------------------------------------
# install freetype
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/freetype2.pc ]; then
echo "installing freetype......"
if [ ! -d freetype-2.4.4 ]; then
if [ ! -e freetype-2.4.4.tar.bz2 ]; then
wget http://download.savannah.gnu.org/releases/freetype/freetype-2.4.4.tar.bz2
fi
tar -xjf freetype-2.4.4.tar.bz2
fi
cd freetype-2.4.4
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf freetype-2.4.4
fi

#-------------------------------------------------------------------------------
# install fontconfig
# 
# dependencies: {
#   dev-util/pkg-config,
#   dev-libs/expat,
#   media-libs/freetype
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/fontconfig.pc ]; then
echo "installing fontconfig......"
if [ ! -d fontconfig-2.8.0 ]; then
if [ ! -e fontconfig-2.8.0.tar.gz ]; then
wget http://www.freedesktop.org/software/fontconfig/release/fontconfig-2.8.0.tar.gz
fi
tar -xzf fontconfig-2.8.0.tar.gz
fi
cd fontconfig-2.8.0
./configure --host=$HOST --build=$BUILD --with-arch=arm --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf fontconfig-2.8.0
fi

#-------------------------------------------------------------------------------
# install xtrans
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xtrans.pc ]; then
echo "installing xtrans......"
if [ ! -d xtrans-1.2 ]; then
if [ ! -e xtrans-1.2.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/xtrans-1.2.tar.bz2
fi
tar -xjf xtrans-1.2.tar.bz2
fi
cd xtrans-1.2
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf xtrans-1.2
fi

#-------------------------------------------------------------------------------
# install inputproto
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/inputproto.pc ]; then
echo "installing inputproto......"
if [ ! -d inputproto-2.0 ]; then
if [ ! -e inputproto-2.0.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/inputproto-2.0.tar.bz2
fi
tar -xjf inputproto-2.0.tar.bz2
fi
cd inputproto-2.0
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf inputproto-2.0
fi

#-------------------------------------------------------------------------------
# install kbproto
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/kbproto.pc ]; then
echo "installing kbproto......"
if [ ! -d kbproto-1.0.5 ]; then
if [ ! -e kbproto-1.0.5.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/kbproto-1.0.5.tar.bz2
fi
tar -xjf kbproto-1.0.5.tar.bz2
fi
cd kbproto-1.0.5
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf kbproto-1.0.5
fi

#-------------------------------------------------------------------------------
# install xextproto
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xextproto.pc ]; then
echo "installing xextproto......"
if [ ! -d xextproto-7.2.0 ]; then
if [ ! -e xextproto-7.2.0.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/xextproto-7.2.0.tar.bz2
fi
tar -xjf xextproto-7.2.0.tar.bz2
fi
cd xextproto-7.2.0
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf xextproto-7.2.0
fi

#-------------------------------------------------------------------------------
# install xproto
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xproto.pc ]; then
echo "installing xproto......"
if [ ! -d xproto-7.0.20 ]; then
if [ ! -e xproto-7.0.20.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/xproto-7.0.20.tar.bz2
fi
tar -xjf xproto-7.0.20.tar.bz2
fi
cd xproto-7.0.20
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf xproto-7.0.20
fi

#-------------------------------------------------------------------------------
# install xcb-proto
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xcb-proto.pc ]; then
echo "installing xcb-proto......"
if [ ! -d xcb-proto-1.6 ]; then
if [ ! -e xcb-proto-1.6.tar.bz2 ]; then
wget http://xcb.freedesktop.org/dist/xcb-proto-1.6.tar.bz2
fi
tar -xjf xcb-proto-1.6.tar.bz2
fi
cd xcb-proto-1.6
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf xcb-proto-1.6
fi

#-------------------------------------------------------------------------------
# install libpthread-stubs
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/pthread-stubs.pc ]; then
echo "installing libpthread-stubs......"
if [ ! -d libpthread-stubs-0.3 ]; then
if [ ! -e libpthread-stubs-0.3.tar.bz2 ]; then
wget http://xcb.freedesktop.org/dist/libpthread-stubs-0.3.tar.bz2
fi
tar -xjf libpthread-stubs-0.3.tar.bz2
fi
cd libpthread-stubs-0.3
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libpthread-stubs-0.3
fi

#-------------------------------------------------------------------------------
# install libxslt
# 
# dependencies: {
#   dev-libs/libxml2,
#   sys-devel/libtool,
#   sys-devel/autoconf:2.5,
#   sys-devel/automake:1.11 
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/libxslt.pc ]; then
echo "installing libxslt......"
if [ ! -d libxslt-1.1.26 ]; then
if [ ! -e libxslt-1.1.26.tar.gz ]; then
wget ftp://xmlsoft.org/libxslt/libxslt-1.1.26.tar.gz
fi
tar -xzf libxslt-1.1.26.tar.gz
fi
cd libxslt-1.1.26
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && \
make && make install || exit
cd ..
rm -rf libxslt-1.1.26
fi

#-------------------------------------------------------------------------------
# install libXau
# 
# dependencies: {
#   x11-proto/xproto
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xau.pc ]; then
echo "installing libXau......"
if [ ! -d libXau-1.0.6 ]; then
if [ ! -e libXau-1.0.6.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXau-1.0.6.tar.bz2
fi
tar -xjf libXau-1.0.6.tar.bz2
fi
cd libXau-1.0.6
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXau-1.0.6
fi

#-------------------------------------------------------------------------------
# install libXdmcp
# 
# dependencies: {
#   x11-proto/xproto
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xdmcp.pc ]; then
echo "installing libXau......"
if [ ! -d libXdmcp-1.1.0 ]; then
if [ ! -e libXdmcp-1.1.0.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXdmcp-1.1.0.tar.bz2
fi
tar -xjf libXdmcp-1.1.0.tar.bz2
fi
cd libXdmcp-1.1.0
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXdmcp-1.1.0
fi

#-------------------------------------------------------------------------------
# install libxcb
# 
# dependencies: {
#   x11-proto/xcb-proto, 
#   dev-libs/libpthread-stubs,
#   dev-libs/libxslt,
#   x11-libs/libXau,
#   x11-libs/libXdmcp
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xcb.pc ]; then
echo "installing libxcb......"
if [ ! -d libxcb-1.7 ]; then
if [ ! -e libxcb-1.7.tar.bz2 ]; then
wget http://xcb.freedesktop.org/dist/libxcb-1.7.tar.bz2
fi
tar -xjf libxcb-1.7.tar.bz2
fi
cd libxcb-1.7
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libxcb-1.7
fi

#-------------------------------------------------------------------------------
# install libX11
# 
# dependencies: {
#   x11-libs/xtrans,
#   x11-proto/inputproto,
#   x11-proto/kbproto,
#   x11-proto/xextproto,
#   x11-proto/xproto,
#   x11-libs/libxcb
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/x11.pc ]; then
echo "installing libX11......"
if [ ! -d libX11-1.4.2 ]; then
if [ ! -e libX11-1.4.2.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libX11-1.4.2.tar.bz2
fi
tar -xjf libX11-1.4.2.tar.bz2
fi
cd libX11-1.4.2
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libX11-1.4.2
fi

#-------------------------------------------------------------------------------
# install compositeproto
# 
# dependencies: {
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/compositeproto.pc ]; then
echo "installing compositeproto......"
if [ ! -d compositeproto-0.4.2 ]; then
if [ ! -e compositeproto-0.4.2.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/compositeproto-0.4.2.tar.bz2
fi
tar -xjf compositeproto-0.4.2.tar.bz2
fi
cd compositeproto-0.4.2
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf compositeproto-0.4.2
fi

#-------------------------------------------------------------------------------
# install xext
# 
# dependencies: {
#   dev-util/pkg-config,
#   x11-proto/xextproto,
#   x11-proto/xproto,
#   x11-libs/libX11
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xext.pc ]; then
echo "installing xext......"
if [ ! -d libXext-1.2.0 ]; then
if [ ! -e libXext-1.2.0.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXext-1.2.0.tar.bz2
fi
tar -xjf libXext-1.2.0.tar.bz2
fi
cd libXext-1.2.0
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXext-1.2.0
fi

#-------------------------------------------------------------------------------
# install compositeproto
# 
# dependencies: {
#   dev-util/pkg-config,
#   x11-proto/xextproto
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/fixesproto.pc ]; then
echo "installing fixesproto......"
if [ ! -d fixesproto-5.0 ]; then
if [ ! -e fixesproto-5.0.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/fixesproto-5.0.tar.bz2
fi
tar -xjf fixesproto-5.0.tar.bz2
fi
cd fixesproto-5.0
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf fixesproto-5.0
fi

#-------------------------------------------------------------------------------
# install libXfixes
# 
# dependencies: {
#   x11-proto/fixesproto,
#   x11-proto/xextproto,
#   x11-proto/xproto,
#   x11-libs/libX11
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xfixes.pc ]; then
echo "installing libXfixes......"
if [ ! -d libXfixes-5.0 ]; then
if [ ! -e libXfixes-5.0.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXfixes-5.0.tar.bz2
fi
tar -xjf libXfixes-5.0.tar.bz2
fi
cd libXfixes-5.0
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXfixes-5.0
fi

#-------------------------------------------------------------------------------
# install libXcomposite
# 
# dependencies: {
#   x11-proto/compositeproto,
#   x11-libs/libX11,
#   x11-libs/libXfixes,
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xcomposite.pc ]; then
echo "installing libXcomposite......"
if [ ! -d libXcomposite-0.4.3 ]; then
if [ ! -e libXcomposite-0.4.3.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXcomposite-0.4.3.tar.bz2
fi
tar -xjf libXcomposite-0.4.3.tar.bz2
fi
cd libXcomposite-0.4.3
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXcomposite-0.4.3
fi

#-------------------------------------------------------------------------------
# install cairo
# 
# dependencies: {
#   dev-util/pkg-config,
#   media-libs/libpng,
#   x11-dri/mesa,
#   media-libs/freetype,
#   media-libs/fontconfig,
#   x11-libs/pixman,
#   dev-libs/glib,
#}
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/cairo.pc ]; then
echo "installing cairo......"
if [ ! -d cairo-1.10.2 ]; then
if [ ! -e cairo-1.10.2.tar.gz ]; then
wget http://www.cairographics.org/releases/cairo-1.10.2.tar.gz
fi
tar -xzf cairo-1.10.2.tar.gz
fi
cd cairo-1.10.2
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf cairo-1.10.2
fi

#-------------------------------------------------------------------------------
# install pango
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/pango.pc ]; then
echo "installing pango......"
if [ ! -d pango-1.28.3 ]; then
if [ ! -e pango-1.28.3.tar.bz2 ]; then
wget ftp://ftp.gnome.org/pub/GNOME/sources/pango/1.28/pango-1.28.3.tar.bz2
fi
tar -xjf pango-1.28.3.tar.bz2
fi
cd pango-1.28.3
cat > $HOST.cache << "EOF"
have_cairo=true
have_cairo_png=true
have_cairo_freetype=true
EOF
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX \
--cache-file=$HOST.cache --with-x && make && make install || exit
cd ..
rm -rf pango-1.28.3
fi

#-------------------------------------------------------------------------------
# install gdk-pixbuf
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/gdk-pixbuf-2.0.pc ]; then
echo "installing gdk-pixbuf......"
if [ ! -d gdk-pixbuf-2.22.1 ]; then
if [ ! -e gdk-pixbuf-2.22.1.tar.bz2 ]; then
wget http://ftp.gnome.org/pub/gnome/sources/gdk-pixbuf/2.22/gdk-pixbuf-2.22.1.tar.bz2
fi
tar -xjf gdk-pixbuf-2.22.1.tar.bz2
fi
cd gdk-pixbuf-2.22.1
cat > $HOST.cache << "EOF"
gio_can_sniff=yes
EOF
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX \
--cache-file=$HOST.cache && make && make install || exit
cd ..
rm -rf gdk-pixbuf-2.22.1
fi

#-------------------------------------------------------------------------------
# install xineramaproto
# 
# dependencies: {
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xineramaproto.pc ]; then
echo "installing xineramaproto......"
if [ ! -d xineramaproto-1.2.1 ]; then
if [ ! -e xineramaproto-1.2.1.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/xineramaproto-1.2.1.tar.bz2
fi
tar -xjf xineramaproto-1.2.1.tar.bz2
fi
cd xineramaproto-1.2.1
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf xineramaproto-1.2.1
fi

#-------------------------------------------------------------------------------
# install libXinerama
# 
# dependencies: {
#   x11-proto/xextproto,
#   x11-proto/xineramaproto,
#   x11-libs/libX11,
#   x11-libs/libXext,
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xinerama.pc ]; then
echo "installing libXinerama......"
if [ ! -d libXinerama-1.1.1 ]; then
if [ ! -e libXinerama-1.1.1.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXinerama-1.1.1.tar.bz2
fi
tar -xjf libXinerama-1.1.1.tar.bz2
fi
cd libXinerama-1.1.1
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXinerama-1.1.1
fi

#-------------------------------------------------------------------------------
# install libXi
# 
# dependencies: {
#   x11-proto/xproto,
#   x11-proto/xextproto,
#   x11-proto/xineramaproto,
#   x11-libs/libX11,
#   x11-libs/libXext,
#   dev-util/pkg-config,
#   x11-proto/inputproto
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xi.pc ]; then
echo "installing libXi......"
if [ ! -d libXi-1.4.1 ]; then
if [ ! -e libXi-1.4.1.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXi-1.4.1.tar.bz2
fi
tar -xjf libXi-1.4.1.tar.bz2
fi
cd libXi-1.4.1
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXi-1.4.1
fi

#-------------------------------------------------------------------------------
# install renderproto
# 
# dependencies: {
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/renderproto.pc ]; then
echo "installing renderproto......"
if [ ! -d renderproto-0.11.1 ]; then
if [ ! -e renderproto-0.11.1.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/renderproto-0.11.1.tar.bz2
fi
tar -xjf renderproto-0.11.1.tar.bz2
fi
cd renderproto-0.11.1
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf renderproto-0.11.1
fi

#-------------------------------------------------------------------------------
# install libXrender
# 
# dependencies: {
#   dev-util/pkg-config,
#   x11-proto/renderproto,
#   x11-libs/libX11
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xrender.pc ]; then
echo "installing libXrender......"
if [ ! -d libXrender-0.9.6 ]; then
if [ ! -e libXrender-0.9.6.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXrender-0.9.6.tar.bz2
fi
tar -xjf libXrender-0.9.6.tar.bz2
fi
cd libXrender-0.9.6
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXrender-0.9.6
fi

#-------------------------------------------------------------------------------
# install randrproto
# 
# dependencies: {
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/randrproto.pc ]; then
echo "installing randrproto......"
if [ ! -d randrproto-1.3.2 ]; then
if [ ! -e randrproto-1.3.2.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/randrproto-1.3.2.tar.bz2
fi
tar -xjf randrproto-1.3.2.tar.bz2
fi
cd randrproto-1.3.2
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf randrproto-1.3.2
fi

#-------------------------------------------------------------------------------
# install libXrandr
# 
# dependencies: {
#   dev-util/pkg-config,
#   x11-proto/randrproto,
#   x11-proto/renderproto,
#   x11-proto/xextproto,
#   x11-proto/xproto,
#   x11-libs/libX11,
#   x11-libs/libXrender,
#   x11-libs/libXext,
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xrandr.pc ]; then
echo "installing libXrandr......"
if [ ! -d libXrandr-1.3.1 ]; then
if [ ! -e libXrandr-1.3.1.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXrandr-1.3.1.tar.bz2
fi
tar -xjf libXrandr-1.3.1.tar.bz2
fi
cd libXrandr-1.3.1
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXrandr-1.3.1
fi

#-------------------------------------------------------------------------------
# install libXcursor
# 
# dependencies: {
#   dev-util/pkg-config,
#    x11-proto/fixesproto,
#    x11-libs/libX11,
#    x11-libs/libXfixes,
#    x11-libs/libXrender
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xcursor.pc ]; then
echo "installing libXcursor......"
if [ ! -d libXcursor-1.1.11 ]; then
if [ ! -e libXcursor-1.1.11.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXcursor-1.1.11.tar.bz2
fi
tar -xjf libXcursor-1.1.11.tar.bz2
fi
cd libXcursor-1.1.11
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXcursor-1.1.11
fi

#-------------------------------------------------------------------------------
# install damageproto
# 
# dependencies: {
#   dev-util/pkg-config
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/damageproto.pc ]; then
echo "installing damageproto......"
if [ ! -d damageproto-1.2.1 ]; then
if [ ! -e damageproto-1.2.1.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/proto/damageproto-1.2.1.tar.bz2
fi
tar -xjf damageproto-1.2.1.tar.bz2
fi
cd damageproto-1.2.1
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf damageproto-1.2.1
fi

#-------------------------------------------------------------------------------
# install libXdamage
# 
# dependencies: {
#   dev-util/pkg-config,
#   x11-proto/damageproto,
#   x11-proto/fixesproto,
#   x11-proto/xextproto,
#   x11-libs/libX11,
#   x11-libs/libXfixes
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/xdamage.pc ]; then
echo "installing libXdamage......"
if [ ! -d libXdamage-1.1.3 ]; then
if [ ! -e libXdamage-1.1.3.tar.bz2 ]; then
wget http://xorg.freedesktop.org/archive/individual/lib/libXdamage-1.1.3.tar.bz2
fi
tar -xjf libXdamage-1.1.3.tar.bz2
fi
cd libXdamage-1.1.3
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf libXdamage-1.1.3
fi

#-------------------------------------------------------------------------------
# install hicolor-icon-theme
#-------------------------------------------------------------------------------
if [ ! -d $PREFIX/share/icons/hicolor ]; then
echo "installing hicolor-icon-theme......"
if [ ! -d hicolor-icon-theme-0.10 ]; then
if [ ! -e hicolor-icon-theme-0.10.tar.gz ]; then
wget http://icon-theme.freedesktop.org/releases/hicolor-icon-theme-0.10.tar.gz
fi
tar -xzf hicolor-icon-theme-0.10.tar.gz
fi
cd hicolor-icon-theme-0.10
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX && make && make install || exit
cd ..
rm -rf hicolor-icon-theme-0.10
fi

#-------------------------------------------------------------------------------
# install GTK+
# 
# dependencies: {
#   sys-devel/gettext,
#   dev-util/pkg-config,
#   dev-libs/glib,
#   dev-libs/atk,
#   x11-libs/pango,
#   x11-libs/cairo,
#   x11-libs/gdk-pixbuf,
#   x11-libs/libXinerama,
#   x11-libs/libXi,
#   x11-libs/libXrandr,
#   x11-libs/libXcursor,
#   x11-libs/libXfixes,
#   x11-libs/libXcomposite,
#   x11-libs/libXdamage,
#   x11-themes/hicolor-icon-theme
# }
#-------------------------------------------------------------------------------
if [ ! -e $PKG_CONFIG_PATH/gtk+-2.0.pc ]; then
echo "installing GTK+......"
if [ ! -d gtk+-2.24.3 ]; then
if [ ! -e gtk+-2.24.3.tar.bz2 ]; then
wget http://ftp.acc.umu.se/pub/gnome/sources/gtk+/2.24/gtk+-2.24.3.tar.bz2
fi
tar -xjf gtk+-2.24.3.tar.bz2
cat > .gtkdeletedemo.patch <<EOF
--- gtk+-2.24.3/configure	2014-09-17 11:42:15.060022267 +0800
+++ gtk+-2.24.3/configure	2014-09-17 11:43:00.816020834 +0800
@@ -22482,6 +22482,8 @@
 esac
 fi
 GDK_PIXBUF_CSOURCE=\$ac_cv_path_GDK_PIXBUF_CSOURCE
+#xiaofei add
+GDK_PIXBUF_CSOURCE=gdk-pixbuf-csource
 if test -n "\$GDK_PIXBUF_CSOURCE"; then
   { \$as_echo "\$as_me:\${as_lineno-\$LINENO}: result: \$GDK_PIXBUF_CSOURCE" >&5
 \$as_echo "\$GDK_PIXBUF_CSOURCE" >&6; }
EOF
patch -p0 < .gtkdeletedemo.patch
rm .gtkdeletedemo.patch
fi
cd gtk+-2.24.3
./autugen.sh
#cat > $HOST.cache << "EOF"
#ac_cv_path_GTK_UPDATE_ICON_CACHE=/usr/bin/gtk-update-icon-cache
#EOF
#./configure --host=$HOST --build=$BUILD --prefix=$PREFIX --with-x --with-gdktarget=x11 && make && make install || exit
./configure --host=$HOST --build=$BUILD --prefix=$PREFIX --enable-shared=yes --enable-static=yes --disable-glibtest --with-gdktarget=directfb --without-x -enable-display-migration && make && make install || exit
cd ..
rm -rf gtk+-2.24.3.tar.bz2
fi
