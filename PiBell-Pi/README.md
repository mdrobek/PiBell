# PiBell-Pi
PiBell-Pi is a small C program that runs on the Raspberry Pi (or every other ARM microcontroller that runs a linux) and plays a ringtone on the speaker attached to the Pi whenever it receives a 'Ring'-request. 

# Dependencies, Prerequisites and Installation
PiBell-Pi expects a linux environment on the Pi, for instance, [Noobs][1] or [Raspbian][1]. Additionally, the following dependencies are required to run/compile PiBell sucessfully:
+ [libcurl][4] - easy access of REST web services
+ libc6 - standard c99 libraries (stdio, stdlib, stdbool, string, math, unistd)

## Installation

### Setup
1) Make sure you have a linux system on your Pi and your Pi has internet access.
2) If you're not locally working on your Pi, login to via ssh.
3) Open a terminal

### Prepare the environment and run it
1) Install libc6 and libcurl:
`sudo apt-get install libc6 libcurl4-openssl-dev`
2) Create and change to a new folder:
`mkdir ~/PiBell`
`cd ~/PiBell`
3) Copy the executable PiBell-Pi/bin/PiBell-arm to the folder ~/PiBell, e.g., via ssh from your Desktop or from a USB stick.
4) Run it
`./PiBell-arm`


# Compiling PiBell-Pi
This section describes how to build the PiBell-Pi project to create the executable. The build system being used in this case is [cmake][2], since it helps us setting up the build environment as needed. PiBell-Pi itself is supposed to be run on the Pi (ARM arch), but for debugging reasons it comes handy to run it on your local desktop system (x86 arch) as well.

TODO: Go on here


# Acknowledgements
Credits to whom credits belong:
+ [cJSON][3] built by Dave Gamble
+ [libcurl][4]

[1]: http://www.raspberrypi.org/downloads/
[2]: http://www.cmake.org/
[3]: http://sourceforge.net/projects/cjson/
[4]: http://curl.haxx.se/libcurl/
