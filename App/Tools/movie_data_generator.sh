#!/bin/sh

PNG_DIRNAME=~/Work/DataGenerator/Data/sgks_july08/png_sgks/
BMP_DIRNAME=~/Work/DataGenerator/Data/sgks_july08/bmp_data/
#BMP_DIRNAME=~/../../Volumes/Samsung_T5/NewData/bmp_data/
SPHERICAL_DIRNAME=~/Work/DataGenerator/Data/sgks_july08/spherical_data/
#SPHERICAL_DIRNAME=~/../../Volumes/Samsung_T5/NewData/spherical_map/
MOVIES_DIRNAME=~/Work/DataGenerator/Data/sgks_july08/movies/
#MOVIES_DIRNAME=~/../../Volumes/Samsung_T5/NewData/movies/

PROGRAM=${PWD##*/}
FRONT=.00
LEFT=.01
RIGHT=.02
BACK=.03
TOP=.04
BOTTOM=.05

BMP_BASENAME="image"
MP4_BASENAME="movies"

command_name="ffmpeg "
option1="-vcodec libx264 "
option2="-pix_fmt yuv420p "
input_fps="-r 7 "
output_fps="-r 7 "

if [ ! -e ${BMP_DIRNAME} ]; then
    mkdir  ${BMP_DIRNAME}
fi

cd ${PNG_DIRNAME}
mogrify -path ${BMP_DIRNAME} -format bmp "*.png"
cd ../

# 全方位画像(bmp)を作成する
if [ ! -e ${SPHERICAL_DIRNAME} ];then
    mkdir ${SPHERICAL_DIRNAME}
fi

#for PATTERN in v0001 v0002 v0003
for PATTERN in v0001
do     
    for FRAME in t000000000 t000000020 t000000040 t000000060 t000000080 t000000100 t000000120
    do
        for CAM in `seq -w 0 999`
        do
            CAMERA=.00${CAM}
            FILENAME=${BMP_BASENAME}${PATTERN}${FRAME}${CAMERA}
       #Ex: FILENAME : imagev0001t000000020.00153
            # フレーム番号を連番にする
            if [ ${FRAME} = "t000000000" ];then
                RE_FRAME=t000000001
            elif [ ${FRAME} = "t000000020" ];then
                RE_FRAME=t000000002
            elif [ ${FRAME} = "t000000040" ];then
                RE_FRAME=t000000003
            elif [ ${FRAME} = "t000000060" ];then
                RE_FRAME=t000000004
            elif [ ${FRAME} = "t000000080" ];then
                RE_FRAME=t000000005
            elif [ ${FRAME} = "t000000100" ];then
                RE_FRAME=t000000006
            elif [ ${FRAME} = "t000000120" ];then
                RE_FRAME=t000000007
            fi
            SPHERICALMAP=${BMP_BASENAME}${PATTERN}${CAMERA}${RE_FRAME}
       #Ex: SPHERICALMAP : imagev0001.000153t000000020
            ./${PROGRAM} -top ${BMP_DIRNAME}/${FILENAME}${TOP}.bmp -left ${BMP_DIRNAME}/${FILENAME}${LEFT}.bmp -right ${BMP_DIRNAME}/${FILENAME}${RIGHT}.bmp -bottom ${BMP_DIRNAME}/${FILENAME}${BOTTOM}.bmp -back ${BMP_DIRNAME}/${FILENAME}${BACK}.bmp -front ${BMP_DIRNAME}/${FILENAME}${FRONT}.bmp -output ${SPHERICAL_DIRNAME}/${SPHERICALMAP}.bmp
        done
    done
done 


# 動画(mp4)を作成する
if [ ! -e ${MOVIES_DIRNAME} ];then
    mkdir ${MOVIES_DIRNAME}
fi

#for PATTERN in v0001 v0002 v0003
for PATTERN in v0001
do
    if [ ! -e ${MOVIES_DIRNAME}${PATTERN} ];then
        mkdir ${MOVIES_DIRNAME}${PATTERN}
        echo "done."
    fi

    for CAM in `seq -w 0 999`
    do
        CAMERA=.00${CAM}
        FRAME_BASENAME=t00000000
        FRAME=${FRAME_BASENAME}"%01d"

        #input file name : imagev0001.00153t000000020.bmp
        BMP_FILE=${BMP_BASENAME}${PATTERN}${CAMERA}${FRAME}.bmp
        input="-i "${SPHERICAL_DIRNAME}${BMP_FILE}" "

        #output file name: moviesv000100153.mp4
        MP4_FILENAME=${MP4_BASENAME}${PATTERN}"00"${CAM}.mp4
        output=${MOVIES_DIRNAME}${PATTERN}"/"${MP4_FILENAME}

        command=${command_name}${input_fps}${input}${option1}${option2}${output_fps}${output}
        ${command}
    done
done
