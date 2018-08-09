#!/bin/sh
PROGRAM=${PWD##*/}
FILENAME=test.mp4
./$PROGRAM -f $FILENAME
