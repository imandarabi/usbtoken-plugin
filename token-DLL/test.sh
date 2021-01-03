#!/bin/sh
#cd lib
#make clean
#make

rm test
gcc -Wall -g -o test dynamic-sample.c -lnovin -ldl
