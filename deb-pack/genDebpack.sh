#!/bin/sh

NAME=fumplugin
VERSION=1.0.0
ARCH=`uname -m`
OS=$(lsb_release -si)
VER=$(lsb_release -sr)

PACK_NAME=$NAME-$VERSION-$ARCH

echo "generating new debian package ..."
sleep 0.1

if [ -d $PACK_NAME ]; then
    rm -rf ./$PACK_NAME
    echo "old direcotry package \""$PACK_NAME"\" removed. "
    sleep 0.1
fi

echo
mkdir -vp ./$PACK_NAME/DEBIAN
sleep 0.1
mkdir -vp ./$PACK_NAME/etc/udev/rules.d
sleep 0.2
mkdir -vp ./$PACK_NAME/usr/lib/mozilla/plugins
sleep 0.1
mkdir -vp ./$PACK_NAME/usr/share/man/man3
sleep 0.2
echo

if [ -d ../$ARCH ]; then
    echo -n 'cp: ' && cp -v ../$ARCH/libnovin.so ./$PACK_NAME/usr/lib
    sleep 0.1
    echo -n 'cp: ' && cp -v ../$ARCH/npfumPlugin.so ./$PACK_NAME/usr/lib/mozilla/plugins
    sleep 0.1
else
    echo "error:" $ARCH "architecture is not generated."
    exit 1
fi

echo -n 'cp: ' && cp -v ../token-DLL/udev/99-hid.rules ./$PACK_NAME/etc/udev/rules.d
sleep 0.1
echo -n 'cp: ' && cp -v ../doc/novinlib.3.gz ./$PACK_NAME/usr/share/man/man3
sleep 0.1

if [ "`dpkg-query -W -f='${Status}\n' libssl1.0.0`" = "install ok installed" ] ; then
    echo -n 'cp: ' && cp -v ../doc/control-$ARCH-libssl1.0.0 ./$PACK_NAME/DEBIAN/control
else
    echo -n 'cp: ' && cp -v ../doc/control-$ARCH-libssl0.9.8 ./$PACK_NAME/DEBIAN/control
fi

sleep 0.1

echo
echo '===== final step, create deb package ====='
dpkg --build $PACK_NAME
echo
mkdir -p ./release/$OS-$VER/
cp -vf $PACK_NAME.deb ./release/$OS-$VER/
rm -rf $PACK_NAME $PACK_NAME.deb
