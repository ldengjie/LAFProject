#!/bin/bash

#EH1
filedir="/dybfs/rec/P12C/GoodRun-part2-v0/EH1"
targetdir="/afs/ihep.ac.cn/users/l/lidj/file/SimplifiedRoot/runWithRoot"

pushd $filedir
for filename in `ls *.list `;
do
  runNum=${filename:3:5}
#  runNum=${filename:0:5}
  echo $filename, $runNum
    sed -e "s/RUNNUM/${runNum}/g"  \
        -e "s/SITE/EH1/g"  $targetdir/root_template.C  > $targetdir/P12C/GoodRun-part2-v0/EH1/root_${runNum}.C

    sed -e "s/RUNNUM/${runNum}/g" \
        -e "s/SITE/EH1/g"   $targetdir/job_template.csh > $targetdir/P12C/GoodRun-part2-v0/EH1/job_${runNum}.csh
    echo "qsub -q dybdpq job_${runNum}.csh" >> $targetdir/P12C/GoodRun-part2-v0/EH1/submit.sh
done
popd

#EH2
filedir="/dybfs/rec/P12C/GoodRun-part2-v0/EH2"
targetdir="/afs/ihep.ac.cn/users/l/lidj/file/SimplifiedRoot/runWithRoot"

pushd $filedir
for filename in `ls *.list `;
do
  runNum=${filename:3:5}
#  runNum=${filename:0:5}
  echo $filename, $runNum
    sed -e "s/RUNNUM/${runNum}/g"  \
        -e "s/SITE/EH2/g"  $targetdir/root_template.C  > $targetdir/P12C/GoodRun-part2-v0/EH2/root_${runNum}.C

    sed -e "s/RUNNUM/${runNum}/g" \
        -e "s/SITE/EH2/g"   $targetdir/job_template.csh > $targetdir/P12C/GoodRun-part2-v0/EH2/job_${runNum}.csh
    echo "qsub -q dybdpq job_${runNum}.csh" >> $targetdir/P12C/GoodRun-part2-v0/EH2/submit.sh
done
popd

#EH3
filedir="/dybfs/rec/P12C/GoodRun-part2-v0/EH3"
targetdir="/afs/ihep.ac.cn/users/l/lidj/file/SimplifiedRoot/runWithRoot"

pushd $filedir
for filename in `ls *.list `;
do
  runNum=${filename:3:5}
#  runNum=${filename:0:5}
  echo $filename, $runNum
    sed -e "s/RUNNUM/${runNum}/g"  \
        -e "s/SITE/EH3/g"  $targetdir/root_template.C  > $targetdir/P12C/GoodRun-part2-v0/EH3/root_${runNum}.C

    sed -e "s/RUNNUM/${runNum}/g" \
        -e "s/SITE/EH3/g"   $targetdir/job_template.csh > $targetdir/P12C/GoodRun-part2-v0/EH3/job_${runNum}.csh
    echo "qsub -q dybdpq job_${runNum}.csh" >> $targetdir/P12C/GoodRun-part2-v0/EH3/submit.sh
done
popd
