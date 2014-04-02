#/bin/tcsh
source /afs/ihep.ac.cn/users/l/lidj/.laf_opt
cd /afs/ihep.ac.cn/users/l/lidj/file/NtupleAna/NtupleAnaRelease/cmt
source setup.csh
source setup.csh
cd /afs/ihep.ac.cn/users/l/lidj/file/SimplifiedRoot/runWithRoot/P12C/GoodRun-part2-v0/SITE
(time unbuffer root -b -q -l root_RUNNUM.C) > log.RUNNUM
