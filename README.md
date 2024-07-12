This is a script for building a webserver on ESP32, to show a toggle switch on the web srver,
and control the GPIO output once user log into the web page.
By deploying another shell script on a PC or Raspberry Pi to handle the accessibility and 
inquery to the webserver, the automation control through whatever web browser to the switch
of your device in your home is achieveable.
No copy right, welcome to copy and use the code if you deem useful.
*Note:
1. Pease compile and upload the main.cpp in /src to ESP32.
2. Please upload the file system files in /data to ESP32 using the Build Filesystem Image /
   Upload Filesystem Image function in platformIO to ESP32 (If you just upload the main.cpp
   to ESP32, then there's no content on the web page once you open it.)
