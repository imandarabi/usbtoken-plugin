#!/bin/sh

ARCH_DIR=`uname -m`

cd ./token-DLL/lib
make clean
make

cd ../..

rm -rf ./$ARCH_DIR
mkdir ./$ARCH_DIR

cp -v ./token-DLL/lib/libnovin.so ./$ARCH_DIR/


cd ./build
make
cd ..

echo
echo
echo "copy plugin to package ... "
cp -v  build/bin/fumPlugin/npfumPlugin.so ./$ARCH_DIR/

