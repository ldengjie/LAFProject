#/bin/tcsh
source /panfs/panfs.ihep.ac.cn/home/dyb/wenlj/.tcshrc.laf
cd /panfs/panfs.ihep.ac.cn/home/dyb/wenlj/LAF/NtupleAna/NtupleAnaRelease/cmt
source setup.csh
cd /publicfs/dyb/data/userdata/wenlj/EH1/LAF/IBDSel/job/P12A/notUseRPC_notTagADMuon
time laf.exe config_run14791_test.txt > log.run14791_test
