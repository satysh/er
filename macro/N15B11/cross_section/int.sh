#!/bin/bash
beg=11
end=52
file=output/N15_cross_and_theta.txt
#file=input/nrv.txt
root -l -q "Integral.C(\"${file}\", ${beg}, ${end})"