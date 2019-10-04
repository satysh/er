#!/bin/bash
### Directories defining ###
OUTDIR=output

### run parametrs defining ###
NEVENTS=1

### Compilation ###
cd ../../../../build/
make -j3
wait
cd -

### Cleaning ###
if [ -d ${OUTDIR} ];then
  rm -vf ${OUTDIR}/*
else
  mkdir ${OUTDIR}
fi


### simulation run ####
root -l -b -q "sim.C(${NEVENTS}, \"${OUTDIR}\")" > ${OUTDIR}/out.txt 2> ${OUTDIR}/err.txt
wait

cat ${OUTDIR}/err.txt

