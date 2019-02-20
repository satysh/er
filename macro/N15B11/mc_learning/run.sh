#!/bin/bash

IONNAME=N15
BEGANGLE=5
ENDANGLE=10
case_n=1
if [ -d output ];then
	rm -rf output/*
else
	mkdir output
fi

if [ -d cases ];then
        cd cases/
        rm -vfi ${IONNAME}_case_${case_n}.root
        cd ../
else
        mkdir cases/
fi

root -l -b -q "analizator.C(\"${IONNAME}\", ${case_n}, ${BEGANGLE}, ${ENDANGLE})"

cp -f output/${IONNAME}_case_${case_n}.root cases/
