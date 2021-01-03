# NOVINLIB - linux based library for 
# communication with novin afzar HID devices.
# Windows HID simplification
#
# iman darabi <iman.darabi@gmail.com>
# May 12, 2013
#
# 
# 
# This program is free software; you can redistribute it and/or         
# modify it under the terms of the GNU General Public License           
# as published by the Free Software Foundation; either version          
# 2 of the License, or (at your option) any later version.              


# add -g to compile with debuge options

all: libnovin.so

libnovin.so:libnovin.o novintools.o hid-libusb.o
	gcc -Wall -g -shared -fPIC novintools.o libnovin.o hid-libusb.o  -o libnovin.so -lcrypto `pkg-config libusb-1.0 --libs` `pkg-config libssl --libs` -lrt -lpthread

libnovin.o:novinlib.h novinlib.c 
	gcc -Wall -g -fPIC -c `pkg-config libusb-1.0 --cflags` novinlib.c -o libnovin.o 

novintools.o:novintools.h novintools.c
	gcc -Wall -g  -fPIC -c `pkg-config libusb-1.0 --cflags` novintools.c -o novintools.o 

hid-libusb.o:hidapi.h hid-libusb.c
	gcc -Wall -g  -fPIC -c `pkg-config libusb-1.0 --cflags` hid-libusb.c -o hid-libusb.o

clean:
	rm -rf *.o *.so  *~
