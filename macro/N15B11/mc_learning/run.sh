#!/bin/bash

IONNAME=N15
BEGANGLE=5
ENDANGLE=35
case_n=4
if [ -d output ];then
	rm -rf output/*
else
	mkdir output
fi

if [ -d cases ];then
        cd cases/
        rm -vf ${IONNAME}_case_${case_n}.root
        cd ../
else
        mkdir cases/
fi

rm -fv input/*
cp archive/case_${case_n}/${IONNAME}/* input/

root -l -b -q "rootFileWriter.C(\"${IONNAME}\", ${case_n}, ${BEGANGLE}, ${ENDANGLE})"

cp -f output/${IONNAME}_case_${case_n}.root cases/
