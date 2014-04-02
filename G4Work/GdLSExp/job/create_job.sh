#!/bin/bash

#Program:
#       submit jobs to simulate Co60 events
#History:
#       2011/08/25     Version2.0
#Author:
#       Xiaohui Chen      xhchen@live.cn

#set path
seed=10000
for ((i=1; i<=19; i=i+1))
do
   echo "/gdls/generator/setParticle gamma" > run$i.mac
   if [ $i -lt 10 ]; then
     echo "/gdls/generator/setEnergy  0.$i" >> run$i.mac 
   fi
   if [ $i -ge 10 ]; then
     j=$(($i-9))
     echo "/gdls/generator/setEnergy  $j.0" >> run$i.mac 
   fi
   echo "/histo/fileName /publicfs/dyb/data/mc/wenlj/nonLinearity/comptE/GdLSExp$i.root" >> run$i.mac
   echo "/histo/setSeed $seed" >> run$i.mac
   echo "/run/beamOn 200000" >> run$i.mac

   echo "#!/bin/tcsh" >job$i.csh
   echo "source /home/dyb/wenlj/env/g4.csh" >>job$i.csh
   echo "cd /publicfs/dyb/user/wenlj/studio/g4test/GdLSExp/job" >>job$i.csh
   echo "GdLSExp run$i.mac > /publicfs/dyb/data/mc/wenlj/nonLinearity/comptE/log/job$i.log" >>job$i.csh

   seed=`expr $seed + 1`
   chmod 755 job$i.csh
   qsub -q dybtuningq job$i.csh
done
