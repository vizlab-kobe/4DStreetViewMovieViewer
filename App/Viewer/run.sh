#!/bin/sh
PROGRAM=${PWD##*/}

DIRNAME=~/Documents/GitHub/4DStreetViewMovieViewer/movies/
# DIRNAME=/Volumes/Samsung_T5/4dsv/data_181108/movies/
EXTENSION=mp4
DIMENSIONS=(20 20 20)
POSITION=(10 10 10)
FRAME_RATE=5

./$PROGRAM -dir $DIRNAME -ext $EXTENSION -dims ${DIMENSIONS[0]} ${DIMENSIONS[1]} ${DIMENSIONS[2]} -pos ${POSITION[0]} ${POSITION[1]} ${POSITION[2]} -fps $FRAME_RATE
