#!/bin/bash

IONNAME=N15
BEGANGLE=5
ENDANGLE=35

echo -n "Type case number: "
read case_n

#cp -r archive/phi/${IONNAME}/case_${case_n}/ archive/

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

if [ -d input/ ];then
	rm -fv input/*
else
	mkdir input/
fi

cp archive/case_${case_n}/${IONNAME}/* input/

root -l -b -q "rootFileWriter.C(\"${IONNAME}\", ${case_n}, ${BEGANGLE}, ${ENDANGLE})"

cp -f output/${IONNAME}_case_${case_n}.root cases/

#rm -r archive/phi/${IONNAME}/case_${case_n}/ 

