#!/bin/bash

#ion name sets 
ionname=B11

#work directories names
inputdir=input
outdir=output_analysis

#cases 1-9
begin=1
end=9

#lab angles 5-35
thbeg=5
thend=35
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

root -l -b -q "analysis.C(${begin}, ${end}, ${thbeg}, ${thend}, \"${inputdir}\", \"${outdir}\", \"${ionname}\")" 

