#/bin/tcsh
source /afs/ihep.ac.cn/users/l/lidj/.laf
cd /afs/ihep.ac.cn/users/l/lidj/user/Laf/NtupleAna/NtupleAnaRelease/cmt
source setup.csh
cd /afs/ihep.ac.cn/users/l/lidj/user/Laf/WljIBDSel/run/SITE
time laf.exe config_runRUNNUM.txt > log.runRUNNUM
