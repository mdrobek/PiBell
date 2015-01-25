#!/bin/bash

# The local git folder for the PiBell project
PIBELL_LOCAL_FOLDER=~/Projects
# The Raspberry Pi installation folder for the PiBell-Pi program
PIBELL_RASPBERRYPI_FOLDER=~/PiBell
# ssh username for the Pi
PI_USERNAME=pi
# Raspberry Pi hostname (IP should also work)
PI_NAME=cake


echo "Copying PiBell program to the Pi (${PI_USERNAME}@${PI_NAME})"
# Sync the executables
rsync -lv ${PIBELL_LOCAL_FOLDER}/PiBell/PiBell-Pi/bin/* ${PI_USERNAME}@${PI_NAME}:${PIBELL_RASPBERRYPI_FOLDER}
# Sync the program resources 
rsync -lv ${PIBELL_LOCAL_FOLDER}/PiBell/PiBell-Pi/res ${PI_USERNAME}@${PI_NAME}:${PIBELL_RASPBERRYPI_FOLDER}
