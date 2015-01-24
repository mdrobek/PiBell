#!/bin/sh
PIBELL_FOLDER=/home/pi/PiBell
#echo "hallo ${PIBELL_FOLDER}"
${PIBELL_FOLDER}/PiBell-arm > ${PIBELL_FOLDER}/logs/log.txt 2> ${PIBELL_FOLDER}/logs/error.log
