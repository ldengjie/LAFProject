#!/bin/bash
if test "$1"
then
    if test "${1//[0-9,a-z,A-Z]/}"
    then 
        echo " ! please input right new package name ! "
    else
	    exampleDir=$PWD
	    parentDir=${exampleDir%/*}
	    echo "...now is in $exampleDir" 
	    echo "...will make $1 under $parentDir"
	    examplePackage=${exampleDir##*/}
	    echo "...template package is : $examplePackage"
        echo "...new package is $1"
	    cd $parentDir
        echo "...making $1 dir "
	    mkdir -p $1
        echo "...copy files and modify code"
	    cp -r $exampleDir/* $1
	    cd $1
	    sed -i "s/$examplePackage/$1/g" `grep $examplePackage -rl ./`
	    cd src/
	    mv $examplePackage.cc $1.cc
	    mv $examplePackage.h $1.h
        cd ..
        rm -rf "NewPackage.sh"
        rm -rf "README"
	    cd $exampleDir
        echo "...Done!" 
    fi
else
    echo " ! please input  new package name ! "
fi
