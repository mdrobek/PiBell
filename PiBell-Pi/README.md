# PiBell-Pi
PiBell-Pi is a small C program that runs on the Raspberry Pi (or every other ARM microcontroller that runs a linux) and plays a ringtone on the speaker attached to the Pi whenever it receives a 'Ring'-request. PiBell-Pi is cross-compiled on a linux x86_64 machine. As of now there is no *make* file for a native compilation on the Raspberry Pi itself.

### Running PiBell
PiBell-Pi expects a linux environment on the Pi, for instance, [Noobs][1] or [Raspbian][1]. In this section, I'm going to explain how to setup up your Pi environment to execute the pre-built PiBell-Pi binary file on your Pi.

##### Dependencies, Prerequisites and Installation
The following dependencies are required to run/compile PiBell sucessfully on your Pi linux system:  
+ [libcurl][4] - easy access of REST web services
+ libc6 - standard c99 libraries (stdio, stdlib, stdbool, string, math, unistd)  

##### Checkout and setup
1) Checkout PiBell to your local system (your desktop):  
`cd ~`  
`git clone https://github.com/mdrobek/PiBell.git`  
2) Make sure you have a linux system on your Pi and your Pi has internet access.  
3) If you're not locally working on your Pi, login via ssh to your Pi.  

##### Prepare the Pi and run PiBell
1) PiBell requires the following dependencies on your Pi:  
  1a) [libcurl][4] - easy access of REST web services  
  1b) libc6 - standard c99 libraries (stdio, stdlib, stdbool, string, math, unistd)  
You can install those via:  
`sudo apt-get install libc6 libcurl4-openssl-dev`  
2) Create and change to a new folder:  
`mkdir ~/PiBell`  
`cd ~/PiBell`  
3) Copy the executable PiBell-Pi/bin/PiBell-arm from your local git copy on your local machine to the folder ~/PiBell on your Pi, e.g., by using the deploy script  
`~/PiBell/PiBell-Pi/build/deploy/deploy.sh`  
**NOTE**: You will need to adjust some parameters in the deploy script, e.g., your Pi hostname and username  
4) That's it! Now run it  
`./PiBell-arm`


### Compiling PiBell-Pi
This section describes how to build the PiBell-Pi project to create the executable. The build system being used in this case is [cmake][2], since it helps us setting up the build environment as needed. PiBell-Pi itself is supposed to be run on the Pi (ARM arch), but for debugging reasons it comes handy to run it on your local desktop system (x86 arch) as well. As mentioned earlier, we are cross-compiling PiBell-Pi on a linux x86_64 machine. There is currently no *make* file to compile it natively on the Raspberry Pi.

##### Setting up the Cross-Compiler
TODO: Go on here


### Acknowledgements
Credits to whom credits belong:
* [cJSON][3] built by Dave Gamble
* [libcurl][4]

[1]: http://www.raspberrypi.org/downloads/
[2]: http://www.cmake.org/
[3]: http://sourceforge.net/projects/cjson/
[4]: http://curl.haxx.se/libcurl/
