#!/bin/tcsh
source /afs/ihep.ac.cn/users/l/liuyb/.tcshrc
cd /workfs/dyw/liuyb/LAF/SinglesAlg/ana/yield/fiveSlices/spectra

date
time ./getSpectra.exe\
-n -1\
-tbkgl 201\
-tbkgh 260\
-tsigl 1\
-tsigh 60\
-el 2.5\
-eh 20\
-o Hist_EH1_B12.root\
-s EH1\
>&! log.eh1
date
