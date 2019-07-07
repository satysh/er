#!/bin/bash
NEVENTS=1000
STARTANG=5
NTHREADS=16
ANGNUM=$(grep -cv "cw,c;" input/thetaCMN15.txt)
echo "ANGNUM: ${ANGNUM}"
if [ -d result ]; then
	cd result
        rm -fv resultGr.pdf
	cd -
else
	mkdir result
fi

root -l "cross_section.C(${NEVENTS}, ${STARTANG}, ${NTHREADS}, ${ANGNUM})"

