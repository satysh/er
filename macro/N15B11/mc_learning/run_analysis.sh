#!/bin/bash

#work directories names
inputdir=input
outdir=output_analysis

#cases
begin=1
end=9

if [ -d ${inputdir} ];then
	echo ${inputdir} was found
else
	mkdir ${inputdir}
	echo ${inputdir} has been created, pleas fill it.
	exit
fi

if [ -d ${outdir} ];then
	cd ${outdir}
	rm -fv *
	cd -
else
	mkdir ${outdir}
fi

root -l -b -q "analysis.C(${begin}, ${end}, \"${inputdir}\", \"${outdir}\")" 

