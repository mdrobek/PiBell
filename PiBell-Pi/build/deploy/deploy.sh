#!/bin/bash
echo "Copying executables to the Pi (pi@cake)"
rsync -lv /home/lion/Projects/PiBell/PiBell-Pi/bin/* pi@cake:/home/pi/PiBell
