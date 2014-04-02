#!/bin/bash

#EH1
filedir="/publicfs/dyb/data/rec/P12B/good_files_3sites_Feb17_May11_2012_v1/EH1"
targetdir="/afs/ihep.ac.cn/users/l/lidj/user/Laf/WljIBDSel/run"

pushd $filedir
for filename in `ls *.list `;
do
  runNum=${filename:3:5}
  echo $filename, $runNum
    sed -e "s/RUNNUM/${runNum}/g"  \
        -e "s/SITE/EH1/g"  $targetdir/config_temp.txt > $targetdir/EH1/config_run${runNum}.txt

    sed -e "s/RUNNUM/${runNum}/g" \
        -e "s/SITE/EH1/g"   $targetdir/job_temp.csh > $targetdir/EH1/job_${runNum}.csh
    echo "qsub -q dyb64q job_${runNum}.csh" >> $targetdir/EH1/submit.sh
done
popd

#EH2
filedir="/publicfs/dyb/data/rec/P12B/good_files_3sites_Feb17_May11_2012_v1/EH2"
targetdir="/afs/ihep.ac.cn/users/l/lidj/user/Laf/WljIBDSel/run"

pushd $filedir
for filename in `ls *.list `;
do
  runNum=${filename:3:5}
  echo $filename, $runNum
    sed -e "s/RUNNUM/${runNum}/g"  \
        -e "s/SITE/EH2/g"  $targetdir/config_temp.txt > $targetdir/EH2/config_run${runNum}.txt

    sed -e "s/RUNNUM/${runNum}/g" \
        -e "s/SITE/EH2/g"   $targetdir/job_temp.csh > $targetdir/EH2/job_${runNum}.csh
    echo "qsub -q dyb64q job_${runNum}.csh" >> $targetdir/EH2/submit.sh
done
popd

#EH3
filedir="/publicfs/dyb/data/rec/P12B/good_files_3sites_Feb17_May11_2012_v1/EH3"
targetdir="/afs/ihep.ac.cn/users/l/lidj/user/Laf/WljIBDSel/run"

pushd $filedir
for filename in `ls *.list `;
do
  runNum=${filename:3:5}
  echo $filename, $runNum
    sed -e "s/RUNNUM/${runNum}/g"  \
        -e "s/SITE/EH3/g"  $targetdir/config_temp.txt > $targetdir/EH3/config_run${runNum}.txt

    sed -e "s/RUNNUM/${runNum}/g" \
        -e "s/SITE/EH3/g"   $targetdir/job_temp.csh > $targetdir/EH3/job_${runNum}.csh
    echo "qsub -q dyb64q job_${runNum}.csh" >> $targetdir/EH3/submit.sh
done
popd
