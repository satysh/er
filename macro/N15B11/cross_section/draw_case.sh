#!/bin/bash

echo -n "Print case number to draw cross-section: "
read case_n

cp -r archive/ati/case_* archive/

if [ -d input/ ];then
	cd input/
	rm -fv out* target_* theta* dPhi_*
	cd ../
else
	echo "Missing important directory input/."
	exit
fi

if [ -d cases/ ];then
  rm -rf cases/*
else
  mkdir cases/
fi

if [ -d archive/ ];then
	if [ -d archive/case_${case_n} ];then
		if [ -d archive/case_${case_n}/N15/ ];then
			cp archive/case_${case_n}/N15/out.txt input/
			cp archive/case_${case_n}/N15/target_int_num.txt input/
			cp archive/case_${case_n}/N15/thetaCMN15.txt input/
			cp archive/case_${case_n}/N15/dPhi_info.txt input/
			n15=1
		else
			echo "WARNING: Missing directory archive/case_${case_n}/N15/."
			if [ -d  archive/case_${case_n}/B11/ ];then
				cp archive/case_${case_n}/B11/out.txt input/
                        	cp archive/case_${case_n}/B11/target_int_num.txt input/
				cp archive/case_${case_n}/N15/dPhi_info.txt input/
			fi
			n15=0
		fi

		if [ -d archive/case_${case_n}/B11/ ];then
                        cp archive/case_${case_n}/B11/out.txt input/outB11.txt
                        cp archive/case_${case_n}/B11/target_int_num.txt input/target_int_numB11.txt
                        cp archive/case_${case_n}/B11/thetaCMB11.txt input/
			b11=1
                else
                        echo "WARNING: Missing directory archive/case_${case_n}/B11/."
			b11=0
                fi
	else
		echo "Missing important directory archive/case_${case_n}/."
		exit
	fi
else
	echo "Missing important directory archive/."
	exit
fi

if [ $n15 == 1 ];then
	if  [ $b11 == 1 ];then
		echo "Empty archive/case_${case_n}"
	fi
fi
echo "$n15 $b11"

rm -rv archive/case_*

root -l "cross_section.C(0, 5, 0, 31, 1, ${case_n}, \"output\", 0)"
