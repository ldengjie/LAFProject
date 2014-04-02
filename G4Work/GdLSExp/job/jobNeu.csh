#!/bin/tcsh
source /home/dyb/wenlj/env/g4.csh
cd /publicfs/dyb/user/wenlj/studio/g4test/GdLSExp/job
#GdLSExp runNeu.mac > /publicfs/dyb/data/mc/wenlj/nonLinearity/comptE/log/jobNeu_useNNDCGdCaptureFalse.log
#GdLSExp runNeu.mac > /publicfs/dyb/data/mc/wenlj/nonLinearity/comptE/log/jobNeu_useNNDCGdCaptureTrue_useOldNdtGdSpectraTrue.log
GdLSExp runNeu.mac > /publicfs/dyb/data/mc/wenlj/nonLinearity/comptE/log/jobNeu_useNNDCGdCaptureTrue_useOldNdtGdSpectraFalse.log
