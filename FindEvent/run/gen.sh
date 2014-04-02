#!/bin/bash

#EH1
filedir="/dybfs/rec/P12E/GoodRun_v4/minirec_peakCharge/EH1"
targetdir="/workfs/dyw/lidj/FindEvent/run"

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
filedir="/dybfs/rec/P12E/GoodRun_v4/minirec_peakCharge/EH2"
targetdir="/workfs/dyw/lidj/FindEvent/run"

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
filedir="/dybfs/rec/P12E/GoodRun_v4/minirec_peakCharge/EH3"
targetdir="/afs/ihep.ac.cn/users/l/lidj/user/Laf/FindEvent/run"
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
