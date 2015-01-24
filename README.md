# PiBell
PiBell is a free Client-Server Application to transform the Raspberry Pi into a (loud) ringing bell. Its main purpose is to make people aware of something, for instance, an incoming Skype call, lunch time, a finished download, a door bell or anything else that people need to be informed about. Since PiBell is connected to the internet, virtually everything "online" can trigger the bell.

## Architecture
The PiBell project is divided into 3 separate sub modules, each with its on purpose:
* PiBell-Pi - A plain C program monitoring the backend server for 'Ring' requests and if triggered, rings the bell. (Runs on the Pi)
* PiBell-GUI - Java frontend GUI used to send a 'Ring' request to a PiBell. (Runs on everything that has a JVM on it, but probably Desktops though)
* PiBell-Server - J2EE backend that receives and caches 'Ring' requests. (Runs on your server machine)
All 3 modules have to run in order to have a working PiBell.

## Hardware Requirements
This project has the following requirements:
+ 1 Rapsberry Pi B+ (with USB power cord or battery pack)
+ 1 Speaker (with a 3.5mm jack audio input)
+ 1 USB-Wifi dongle (if you want a mobile PiBell)
+ 1 Server (with Tomcat/Jetty and Java installed)
+ at least 1 device that sends a 'Ring' request (e.g., your Desktop machine)
