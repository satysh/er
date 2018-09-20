#!/bin/bash

grep -c -v output/test interact_thetas.txt | root -l -b -q histo_draw.C

