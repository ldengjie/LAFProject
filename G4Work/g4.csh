//setenv G4WORKDIR /publicfs/dyb/data/userdata/yangms/g4test
..setenv ROOTSYS  /afs/ihep.ac.cn/soft/dayabay/NuWa-64/opt/external/ROOT/5.26.00e_python2.7/x86_64-slc5-gcc41-opt/root


setenv G4SYSTEM  Linux-g++
#setenv G4INSTALL  /publicfs/dyb/data/userdata/yangms/geant4/4.9.2.p01/x86_64-slc5-gcc41-dbg
#setenv G4INSTALL /afs/ihep.ac.cn/soft/dayabay/NuWa-64/opt/external/geant4/4.9.2.p01/x86_64-slc5-gcc41-opt
setenv G4INSTALL /afs/ihep.ac.cn/soft/dayabay/NuWa-64/opt/external/build/LCG/geant4.9.2.p01
setenv G4INCLUDE  $G4INSTALL/include
setenv G4LIB  $G4INSTALL/lib


setenv G4LEVELGAMMADATA  /publicfs/dyb/data/userdata/yangms/geant4/data/PhotonEvaporation/2.0
setenv G4RADIOACTIVEDATA  /publicfs/dyb/data/userdata/yangms/geant4/data/G4RadioactiveDecay/3.2
setenv G4LEDATA  /publicfs/dyb/data/userdata/yangms/geant4/data/G4EMLOW/6.2
setenv G4NEUTRONHPDATA  /publicfs/dyb/data/userdata/yangms/geant4/data/G4NDL/3.13
setenv G4ABLADATA  /publicfs/dyb/data/userdata/yangms/geant4/data/G4ABLA/3.0


#setenv CLHEP_BASE_DIR  /publicfs/dyb/data/userdata/yangms/geant4/clhep/2.0.4.2/x86_64-slc5-gcc41-opt
!!setenv CLHEP_BASE_DIR /afs/ihep.ac.cn/soft/dayabay/NuWa-64/opt/external/build/LCG/clhep/2.0.4.2/CLHEP
setenv CLHEP_INCLUDE_DIR  $CLHEP_BASE_DIR/include
setenv CLHEP_LIB_DIR  $CLHEP_BASE_DIR/lib
setenv CLHEP_LIB  CLHEP
setenv LD_LIBRARY_PATH  ${ROOTSYS}/lib:$G4INSTALL/lib:$CLHEP_BASE_DIR/lib:$LD_LIBRARY_PATH



#setenv LD_LIBRARY_PATH  /home/dyb/dybsw/NuWa-64/NuWa-trunk/../external/OpenScientist/16.11/x86_64-slc5-gcc41-dbg/lib/:$LD_LIBRARY_PATH
#setenv LD_LIBRARY_PATH  /home/dyb/dybsw/NuWa-64/NuWa-trunk/../external/OpenMotif/2.3.0/x86_64-slc5-gcc41-dbg/lib/:$LD_LIBRARY_PATH
setenv PATH  ${PATH}:$CLHEP_BASE_DIR/bin:${G4WORKDIR}/bin/${G4SYSTEM}:${ROOTSYS}/bin
#source $G4INSTALL/env.csh
setenv DAYA_DATA_DIR /publicfs/dyb/data/userdata/yangms/geant4/data
#setenv MALLOC_CHECK_ 0
