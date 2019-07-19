#!/bin/bash

# Directories
SIMOUTDIR=output_parallel
RESULTSDIR=result
COMPILATIONDIR=
CALCOUTDIR=calc_output
COMPILATIONDIR=../../../build
INDIR=../output_parallel
OUTDIR=output_digi_parallel
GRAPHSOUTDIR=digi_graphs_parallel

# Variables
NEVENTS=1000
MINANGLE=5
MAXANGLE=35

NTHREADS=3

case_beg=2
case_end=4
IONNAME=N15

#Calculate step and Interaction numbers
a=1
b=1
STEP=$(echo "$a/$b" | bc -l) #STEP=a/b
ITNUMBER=$(echo "1+($MAXANGLE-$MINANGLE)/$STEP" | bc -l)

# Compilation
echo ${COMPILATIONDIR:?You must set directory to compilation}
if [ -d ${COMPILATIONDIR} ]; then
	cd ${COMPILATIONDIR}
	make -j${NTHREADS}
	cd -
else
	echo -e "Compilation directory: \e[1m\e[34m${COMPILATIONDIR}\e[0m was not found"
	exit
fi

echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"

for case_n in $(seq ${case_beg} ${case_end}); do

    echo "Case_${case_n}"

    if [ -d mc_learning ]; then
	   cd mc_learning
	   if [ -d input ]; then
	       rm -rf input/*
	   else
	       mkdir input
	   fi
	   cd -
    else
	   mkdir mc_learning
	   mkdir mc_learning/input
    fi

    if [ -d mc_learning/aloteventsarchive/ ];then
        echo "mc_learning/aloteventsarchive/ DONE!"
    else
        mkdir mc_learning/aloteventsarchive/
    fi

    for IT in $(seq 1 ${ITNUMBER}); do
    ####################################### Simulation #######################################
	   ANG=$(echo "$MINANGLE+($IT-1)*$STEP" | bc -l) #curent angle calculate

	   if [ -d ${SIMOUTDIR} ]; then
	       cd ${SIMOUTDIR}
	       rm -fv *
	       cd -
	   else
	       mkdir ${SIMOUTDIR}
	   fi

	   if [ -d ${CALCOUTDIR} ]; then
	       cd ${CALCOUTDIR}
	       rm -fv *
	       cd -
	   else
	       mkdir ${CALCOUTDIR}
	   fi

	   cd ../../geometry/
	   rm -fv N15B11_detector.geo.root
	   cd -

	   if [ -d mc_learning/output/ ];then
	       cd mc_learning/input/
	       rm -fv interact_thetas_*
	       cd -
	   fi
	   echo -e "\e[1m\e[32m========== Cleanup finished  === Angle( ${ANG} ) ============ \e[0m"

	   if [ -d geo/ ]; then
	       cd geo/
	       root -l -b -q "create_N15B11_detector_geo.C(${ANG})"
	       wait
	       cd -
	   else
	       echo -e "\e[1m\e[32m========== Directory to build geometry was not found =========== \e[0m"
           exit
	   fi

	   echo -e "\e[1m\e[32m========== Geometry created  === Angle( ${ANG} ) ============ \e[0m"

	   for THR in $(seq 1 ${NTHREADS}); do
		  echo "Starting simulation in thread ${THR}"
		  root -l -b -q "sim_${case_n}.C(${NEVENTS}, ${THR}, \"${SIMOUTDIR}\", ${ANG})" > ${SIMOUTDIR}/out_${THR}.txt 2> ${SIMOUTDIR}/err_${THR}.txt &
	   done
	   wait
 	   echo -e "\e[1m\e[32m========== Simulations finished  === Angle( ${ANG} ) ============ \e[0m"

       ####################################### MC analysis #######################################
	   if [ -d mc_learning/input/ ];then
           cd mc_learning/input/
	       for THR in $(seq 1 ${NTHREADS});do
		      cat interact_thetas_${THR}.txt >> all_interact_thetas_${ANG}.txt
	       done
           cd -
       fi

	   wait
    done
    echo -e "\e[1m\e[32m========== FINISHED Case_${case_n} ========= \e[0m"
    wait
    cd mc_learning/
    mkdir aloteventsarchive/case_${case_n}/
    mkdir aloteventsarchive/case_${case_n}/${IONNAME}
    cp input/* aloteventsarchive/case_${case_n}/${IONNAME}/
    cd -
done
