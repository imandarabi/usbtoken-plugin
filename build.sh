#!/bin/sh
cd build
make


echo ""
echo ""
echo 'copy plugin to /usr/lib/mozilla/plugins'
su -c "cp -v /home/iman/prj/fumplugin/build/bin/fumPlugin/npfumPlugin.so /usr/lib/mozilla/plugins/"

echo ""
echo ""
