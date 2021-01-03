#!/bin/bash

echo
echo "ferdowsi university, token plugin installation version 0.1 . May, 2013"
echo 
echo "before proceed, unplug token and then press any key"
read

if [ `whoami` != 'root' ]
then
    echo
    echo "error: run install.sh script as root user"
    echo
    exit 1
fi
sleep 0.4

PKG_SSL_OK=$(dpkg-query -W --showformat='${Status}\n' libssl1.0.0 |grep "install ok installed")
echo Checking for libssl: $PKG_SSL_OK ... 
if [ "" == "$PKG_SSL_OK" ]; then
  sleep 0.4
  echo "WARNING: No libssl found, install libssl1.0.0 or higher version "
fi
echo

PKG_USB_OK=$(dpkg-query -W --showformat='${Status}\n' libusb-1.0-0 |grep "install ok installed")
echo Checking for libusb: $PKG_USB_OK ... 
if [ "" == "$PKG_USB_OK" ]; then
  sleep 0.4
  echo "WARNING: No libusb found, install libusb-1.0-0 or higher version "
fi
echo

echo 'copy npfumPlugin.so to /usr/lib/mozilla/plugins/'

if [ ! -d /usr/lib/mozilla/plugins/ ]; then
    mkdir -p /usr/lib/mozilla/plugins/
fi

if [ `uname -m` = 'x86_64' ]; then
    /bin/cp -v ./x86_64/npfumPlugin.so /usr/lib/mozilla/plugins/

elif [ `uname -m` = 'i686' ]; then
    /bin/cp -v ./i686/npfumPlugin.so /usr/lib/mozilla/plugins/

else
    echo 'error: architecture not supported.'
    echo
    exit 1
fi
sleep 0.8

echo
echo 'copy libnovin.so to /usr/lib ... '
/bin/cp -v ./`uname -m`/libnovin.so /usr/lib/
sleep 0.5

echo
echo 'copy 99-hid.rules to /etc/udev/rules.d/ ...'
/bin/cp -v  ./udev/99-hid.rules /etc/udev/rules.d/
sleep 0.5

echo
echo "installation completed successfully :)"
echo "report bugs to <iman.darabi@gmail.com>"
echo
