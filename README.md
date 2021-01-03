# usbtoken-plugin
Create usb token plugin project for firefox


0. install libssl1.0.0 and libusb-1.0-0 and cmake and build-essential
   for development build ... . 

		$ apt-get install libssl-dev
		$ apt-get install libusb-1.0-0-dev
		$ apt-get install gcc cmake build-essential pkg-config
1. create build directory : 
   ./firebreath/prepmake.sh fumPlugin/ build

2. first run ./build-libs.sh to create libs and it copy them to x86_64 for instance.

3. next copy prj to another os with diffrent architecture... .
for example if you run project in 64 bit so x86_64 directory with appropriate files 
will be created so use 32 bit os to generate i686 directory. 

4. then goto deb-pack and run "genDebpack.sh" script and it with automatically generate .deb package .

