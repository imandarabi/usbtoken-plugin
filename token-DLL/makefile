all: hid-libusb.o novintools.o libnovin.so

libnovin.so: lib/novinlib.h lib/novinlib.c lib/hid-libusb.o lib/novintools.o
	#gcc -Wall -c -fPIC lib/novinlib.c lib/hid-libusb.o lib/novintools.o -I./lib/ -shared -o gooz.so
	gcc -Wall -c -fPIC lib/novinlib.c -I./lib/ -shared -o gooz.so

#libnovin.so: lib/novinlib.h lib/novinlib.c lib/hid-libusb.o lib/novintools.o
#	gcc -Wall -c -fPIC lib/novinlib.c -I./lib/ -o lib/libnovin.o
#	gcc -shared -o libnovin.so lib/libnovin.o
#	gcc -Wall -o lib/novinlib.so -I./lib/ -shared -fPIC lib/novinlib.c

#libnovin.o: lib/novinlib.h lib/novinlib.c lib/hid-libusb.o lib/novintools.o

novintools.o: lib/novintools.h lib/novintools.c
	gcc -Wall -c lib/novintools.c -o lib/novintools.o -I./lib/

hid-libusb.o: lib/hidapi.h  lib/hid-libusb.c
	gcc -Wall -L. -I/usr/include/libusb-1.0/ -c lib/hid-libusb.c -o lib/hid-libusb.o

clean:
	rm -rf lib/hid-libusb.o lib/novinlib.so lib/novintools.o *~ lib/*~



