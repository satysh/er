#!/bin/bash

rm -fv output/eventDisplay.root

root -l eventDisplay.C
