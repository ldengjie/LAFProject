#!/bin/bash
if [ "${useFILE1:+1}"="1" ]
then
    unset useFILE1
fi
NuWaConfigScript="/afs/ihep.ac.cn/users/l/lidj/.laf_opt"
inputFILELISTPATH="/dybfs/rec/P13A/GoodRun_v1_minirec/"
outPUTFILEPATH="/afs/ihep.ac.cn/users/l/lidj/largedata/StudyForTest"
outputFile2Name="RUNNUM_IBD.root"
#useFILE1="TOutputSvc.Output" #if not use FILE1 ,just comment out this line
outputFile1Name="RUNNUM_hists.root"
inputSvc="ReconInputSvc" # MiniReconInputSvc or ReconInputSvc
useRPC="FALSE" #FALSE or TRUE
targetdir=$PWD

for site in EH1 EH2 EH3
do
	firstlist=1
	pushd $inputFILELISTPATH/$site/
	for filename in `ls *.list `;
	do
		#  runNum=${filename:3:5}
		temprunNum1=${filename:3:5}
		if test "${temprunNum1//[0-9]/}" #is not int
		then
			temprunNum2=${filename:0:5}
			if test "${temprunNum2//[0-9]/}" #is not int
			then
				echo -e "\e[31;44;1m ! bad run number : ${filename}==>${temprunNum1} or ${temprunNum2} ,skip this list !\e[0m"
				continue
			else
				runNum=${filename:0:5}
			fi
		else
			runNum=${filename:3:5}
		fi

		echo $filename, $runNum
			sed -e "s/RUNNUM/${runNum}/g"  \
				-e "s#INPUTFILELISTPATH#${inputFILELISTPATH}#g"  \
				-e "s#FILENAME#${filename}#g"  \
				-e "s#INPUTSVC#${inputSvc}#g"  \
				-e "s#^//TOutputSvc.Output#${useFILE1:=//TOutputSvc.Output}#g"  \
				-e "s#FILE1NAME#${outputFile1Name/RUNNUM/$runNum}#g"  \
				-e "s#FILE2NAME#${outputFile2Name/RUNNUM/$runNum}#g"  \
				-e "s#USERPC#${useRPC}#g"  \
				-e "s#OUTPUTFILEPATH#${outPUTFILEPATH}#g"  \
				-e "s/SITE/$site/g"  $targetdir/config_temp.txt > $targetdir/$site/config_run${runNum}.txt

			sed -e "s/RUNNUM/${runNum}/g" \
				-e "s#TARGETDIR#$targetdir#g"  \
				-e "s#NuWaConfigScript#${NuWaConfigScript}#g"  \
				-e "s/SITE/$site/g"   $targetdir/job_temp.csh > $targetdir/$site/job_${runNum}.csh
		if [ $firstlist -eq 1 ];then
			if [ -f "$targetdir/$site/submit.sh" ];then
				echo "submit.sh already exists,delete it,create a new one"
				rm -rf "$targetdir/$site/submit.sh"
			else
				echo "submit.sh not exists,create a new one"
			fi
		fi
		echo "qsub -q dyb64q job_${runNum}.csh" >> $targetdir/$site/submit.sh
		let "firstlist += 1"
	done
	popd
done

