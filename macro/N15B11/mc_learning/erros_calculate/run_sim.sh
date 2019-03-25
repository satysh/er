#!/bin/bash

### Variables ###
RUNNUM=1
TESTANGLE=17
NEVENTS=10000
NTRD=7
case_n=2
IONNAME=N15

### Directories ###
SIMOUTDIR=output_parallel
OUTDIR=output

### Compilation ###
cd ../../../../../build/
make -j${NTRD}
wait 
cd -
echo -e "\e[1m\e[32m========== Compilation finished ========= \e[0m"

### Clean previos run results ###
if [ -d ../../${SIMOUTDIR} ];then
	rm -fv ../../${SIMOUTDIR}/*
else
	mkdir ../../${SIMOUTDIR}
fi

if [ -d ${OUTDIR} ];then
	rm -fv ${OUTDIR}/*
else
	mkdir ${OUTDIR}
fi
rm -fv ../../../../geometry/N15B11_detector.geo.root
echo -e "\e[1m\e[32m========== Cleanup finished   =========== \e[0m"

### Build geometry ###
cd ../../geo/
root -l -b -q "create_N15B11_detector_geo.C(${TESTANGLE})"
cd -
echo -e "\e[1m\e[32m========== Build geometry finished   =========== \e[0m"

let RUNNUM="${RUNNUM} - 1"
### Loop over the runs
for runIt in $(seq 0 ${RUNNUM});do
	echo -e "\e[1m\e[31mRun: ${runIt} started\e[0m"
	### Run sim ### 
	cd ../../
	for trdIt in $(seq 1 ${NTRD});do
		let index="${runIt}*${NTRD} + trdIt"
		root -l -b -q "sim.C(${NEVENTS}, ${index}, \"${SIMOUTDIR}\", ${TESTANGLE})" 2> ${SIMOUTDIR}/err_${index}.txt > ${SIMOUTDIR}/out_${index}.txt &
		echo "Trd: ${trdIt} simulation is runing"
	done
	wait
	echo -e "\e[1m\e[31mRun: ${runIt} finished\e[0m"
	cd -
	### Merge registered thetas ###
	for It in $(seq 1 ${NTRD});do
		let index="${runIt}*${NTRD} + It"
		cat ../input/interact_thetas_${index}.txt >> ${OUTDIR}/run_${runIt}_thetas.txt
	done
done

let RUNNUM=${RUNNUM}+1
### root file form with tree ###
rm -fv ${OUTDIR}/${IONNAME}_case_${case_n}.root # remove previos root file
root -l -b -q "rootFileForm.C(\"${IONNAME}\", \"${OUTDIR}\", ${case_n}, ${RUNNUM}, ${TESTANGLE})" # create new
echo -e "\e[1m\e[32m========== root file form finished   =========== \e[0m"

###  root file read and create stdDevs.txt ###
rm -fv ${OUTDIR}/stdDevs.txt # remove previos txt
root -l -b -q "rootFileRead.C(\"${IONNAME}\", \"${OUTDIR}\", ${case_n}, ${RUNNUM}, ${TESTANGLE})" 
echo -e "\e[1m\e[32m========== root read finished  =========== \e[0m"
echo " "
echo -e "\e[1m\e[32m========== StdDevs  =========== \e[0m"
cat ${OUTDIR}/stdDevs.txt # remove previos tx
echo -e "\e[1m\e[32m========== ALL FINISHED  ====== \e[0m"
