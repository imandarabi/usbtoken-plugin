#!/bin/bash

if [ -f "fumPlugin-linux-0.1.tar" ];
then
    rm -rf ./fumPlugin-0.1-linux.tgz 
fi

if [ -d "fumPlugin-0.1-linux" ]
then
    rm -rf ./fumPlugin-0.1-linux
fi

mkdir ./fumPlugin-0.1-linux

if [ -d "x86_64" ]
then
    cp -R x86_64 ./fumPlugin-0.1-linux
else
    echo "x86_64 arch is not generated. "
    rm -rf ./fumPlugin-0.1-linux
    exit 1
fi

if [ -d "i686" ]
then
    cp -R i686 ./fumPlugin-0.1-linux
else
    echo "i686 arch is not generated. "
    rm -rf ./fumPlugin-0.1-linux
    exit 1
fi


cp -v -R token-DLL/udev ./fumPlugin-0.1-linux

echo
echo
echo "copy docs to package ..."
cp -R -v ./doc ./fumPlugin-0.1-linux

mv ./fumPlugin-0.1-linux/doc/readme.txt ./fumPlugin-0.1-linux
mv ./fumPlugin-0.1-linux/doc/install.sh ./fumPlugin-0.1-linux

echo
echo "create tar package ..."
tar czvf fumPlugin-0.1-linux.tgz fumPlugin-0.1-linux
#tar cvf fumPlugin-linux-0.1.tar fumPlugin-linux-0.1/
#gzip fumPlugin-linux-0.1.tar
echo

rm -rf ./fumPlugin-0.1-linux