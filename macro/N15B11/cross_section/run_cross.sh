#!/bin/bash
NEVENTS=1000
STARTANG=20
NTHREADS=3
ANGNUM=1
STEP=1
if [ -d result ]; then
	cd result
        rm -fv resultGr.pdf 
	cd -
else
	mkdir result
fi

root -l "cross_section.C(${NEVENTS}, ${STARTANG}, ${NTHREADS}, ${ANGNUM}, ${STEP})"
