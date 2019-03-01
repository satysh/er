#!/bin/bash

IONNAME=N15
STARTCASE=1
ENDCASE=8
STARTANG=5
ENDANG=35

if [ -d output ];then
	rm -fv output/*
else
	mkdir output/
fi

root -l -b -q "stdDevsDraw.C(\"${IONNAME}\", ${STARTCASE}, ${ENDCASE}, ${STARTANG}, ${ENDANG})" > output/out.txt 2> output/err.txt

cat output/err.txt

root -l output/stdDev.root
