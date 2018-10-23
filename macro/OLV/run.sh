#!/bin/bash

nev=1
index=1
rm -fv *.root
rm -fv *txt

cd ~/soft/fork_expertroot_build
make -j3
cd -

#root -l -b -q "sim.C(${nev}, ${index})" 2> err.txt
root -l -b -q "sim_test.C(${nev}, ${index})" > out.txt 2> err.txt
echo err.txt:
cat err.txt
exit
echo "on eventDisplay"
read a
root -l eventDisplay.C
