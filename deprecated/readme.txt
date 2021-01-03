# fumPlugin version 0.1 <May 2013>
# ferdowsi university of mashhad <http://its.um.ac.ir>
# written by : Iman Darabi <iman.darabi@gmail.com>

Installation instructions
-------------------------
   o Unpack the plugin tar.gz and copy the files to the appropriate location.  
   o Save the plugin tar.gz locally and note the location the file was saved to.
   o Launch terminal and change directories to the location the file was saved to.
   o Unpack the tar.gz file.  Once unpacked you will see the following :

      + doc : contain sample java script plugin test and plugin mauall documentation.
      + i686 : plugin and library for 32 bit linux architectures.
      + install.sh : installation script.
      + readme.txt : this is me ;) .
      + udev : udev file for HIDAPI devices which changes the permissions 
         to 0666 (world readable/writable) for token devices.
      + x86_64 : plugin and library for 64 bit linux architectures.

   installation : before install plugin, make sure "libssl1.0.0" and "libusb-1.0-0" are already 
   		installed. on debian based systems you can check with this command:
		# dpkg-query -W -f='${Status} ${Version}\n' libssl1.0.0
		# dpkg-query -W -f='${Status} ${Version}\n' libusb-1.0-0

		if any package isn't installed, install them like this (debian based systems): 
		$ apt-get install libssl1.0.0
		$ apt-get install libusb-1.0-0
   		and then run ./install.sh as root.

		WARNING: old systems like debian "squeeze 6", ubuntu "10.4", "10.10", "11.04",
		old version of libssl (libssl0.9.8) is installed by default; so you should
		install new version of libssl before installing fumplugin.

   manuall installation : 
      1. copy "libnovin.so" from x86_64/i686 to /usr/lib and create cymbolic link
         libnovin.so.0 from libnovin.so in /usr/lib :
            # cp libnovin.so /usr/lib
            # ln -s /usr/lib/libnovin.so /usr/lib/libnovin.so.0

      2.  copy "npfumPlugin.so" from x86_64/i686 to /usr/lib/mozilla/plugins/ and 
          /usr/lib64/mozilla/plugins/ :
             # cp  ./x86_64/npfumPlugin.so /usr/lib/mozilla/plugins/
             # cp  ./x86_64/npfumPlugin.so /usr/lib64/mozilla/plugins/

      note: to detect if os is 32 or 64 bit run 'uname -m' and copy the appropriate files
      from x86_64 or i686 directories.

      3. copy 99-hid.rules from udev directory to /etc/udev/rules.d/ :
         cp ./udev/99-hid.rules /etc/udev/rules.d/

Technical Issues and Reporting Bugs
-----------------------------------

To allow us to investigate reported bugs, 
please include the following information:
 
   1) Platform and version
   2) Browser version
   3) Reproducible steps including a URL to the web site where the problem 
      was encountered.

bugs : report bugs to <iman.darabi@gmail.com>
  
for more information about plugin's functionality read pluginManuall.txt

