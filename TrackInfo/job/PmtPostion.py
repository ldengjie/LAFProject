#!/usr/bin/env python
#
# Reconstruction Results Analysis Example Script
#

# Load DybPythonAlg
from DybPython.DybPythonAlg import DybPythonAlg
from GaudiPython import SUCCESS, FAILURE
from GaudiPython import gbl, loaddict
from DybPython.Util import irange
from GaudiKernel import SystemOfUnits as units

import PyCintex
import ROOT
import re

# Make shortcuts to ROOT classes
TH1F = gbl.TH1F
TH2F = gbl.TH2F
TTree = gbl.TTree
TProfile = gbl.TProfile
CLHEP = gbl.CLHEP

Detector = gbl.DayaBay.Detector
Site = gbl.Site
DetectorId = gbl.DetectorId
ServiceMode = gbl.ServiceMode
AdPmtSensor = gbl.DayaBay.AdPmtSensor
PmtCalibData = gbl.DayaBay.PmtCalibData

from GaudiKernel import SystemOfUnits as units
from array import array
import math
import ROOT

loaddict("libCLHEPRflx")
HepLorentzVector = gbl.CLHEP.HepLorentzVector
loaddict("libHepMCRflx")
HepMCGenEvent = gbl.HepMC.GenEvent

class ReconAnaAlg(DybPythonAlg):
    "ReconAnaAlg: reconstruction results analysis, a DybPythonAlg"
    def __init__(self,name):
        DybPythonAlg.__init__(self,name)
        self.hist = {}
        return

    def initialize(self):
        status = DybPythonAlg.initialize(self)
        print "Init ReconAnaAlg",self.name()
        if status.isFailure(): return status

        self.target_de_name = '/dd/Structure/AD/db-ade1/db-sst1/db-oil1'

        # Initialize services
        self.cableSvc = self.svc('ICableSvc','StaticCableSvc')
        self.calibSvc = self.svc('ICalibDataSvc','StaticCalibDataSvc')
        #self.statsSvc = self.svc('IStatisticsSvc','StatisticsSvc')
        self.pmtSvc = self.svc('IPmtGeomInfoSvc','PmtGeomInfoSvc')

        # CalibReadout Data
        self.obsQ = array('d', 192*[0])

        # RecTrigger Data
        self.expQ = array('d', 192*[0])
        self.firstEntry = True

        # Making histograms

        return SUCCESS

    def execute(self):
        print "Executing ReconAnaAlg",self.name()

        # reset
        self.obsQ = array('d', 192*[0])
        self.expQ = array('d', 192*[0])
        self.totalObsQ = 0.0

        evt = self.evtSvc()
        recHdr = evt["/Event/Rec/AdScaled"] 
        if recHdr == None:
            self.error("Failed to get current RecHeader")
            return FAILURE
        
        #recResults = recHdr.recTrigger()
        recTrigger = recHdr.recTrigger()
        if recTrigger == None:
            self.info("No recTrigger this cycle")
            return SUCCESS

        self.siteName = "DayaBay"
        self.detName = "AD1"
        self.siteIds = {
          'DayaBay' : gbl.Site.kDayaBay,
          'LingAo'  : gbl.Site.kLingAo,
          'Far'     : gbl.Site.kFar,
        }
        self.detIds = {
          'AD1'  :  gbl.DetectorId.kAD1,
          'AD2'  :  gbl.DetectorId.kAD2,
          'AD3'  :  gbl.DetectorId.kAD3,
          'AD4'  :  gbl.DetectorId.kAD4,
        }
        site = 4
        detector = 3 
       
        svcmode = ServiceMode(recHdr.context(), 0)
        filename = "test"+str(site)+str(detector) +".txt"
        infofile = open(filename,'w')
        for localId in range(0, 192):
            ring = localId/24 + 1
            column = localId%24 + 1
#pmtId = AdPmtSensor(ring, column, recTrigger.detector().site(),recTrigger.detector().detectorId())
            pmtId = AdPmtSensor(ring, column, site,detector)
            chanId = self.cableSvc.feeChannelId(pmtId, svcmode)
            pmtCalib = self.calibSvc.pmtCalibData(pmtId, svcmode)
            if pmtCalib == None:
                self.error("No calib data for pmt local ID:%I" %localId)
                return FAILURE
                
            if pmtCalib.m_status != PmtCalibData.kGood:
                continue

            pmtPos = self.pmtSvc.get(pmtId.fullPackedData()).localPosition()
            pmtNorm = self.pmtSvc.get(pmtId.fullPackedData()).localDirection()
#infofile.write("%s %s %s %s %s %s %s\n" %(site,detector,ring,column,pmtPos.x(),pmtPos.y(),pmtPos.z()))
            infofile.write("%s %s %s %s %s\n" %(ring,column,pmtPos.x(),pmtPos.y(),pmtPos.z()))
        infofile.close()
        self.info(" ! DONE ! ")
        return FAILURE

        
    def finalize(self):        
        print "Finalizing ReconAnaAlg",self.name()
        status = DybPythonAlg.finalize(self)
        return status

def configure(argv = []):
    # Process module options
    import sys, getopt
    opts,args = getopt.getopt(argv,"o:")
    for opt,arg in opts:
        if opt == "-o":
            statsOutput = arg
    from StatisticsSvc.StatisticsSvcConf import StatisticsSvc
    statsSvc = StatisticsSvc()
    import DataSvc
    DataSvc.Configure()
    from DetHelpers.DetHelpersConf import PmtGeomInfoSvc
    pgisvc = PmtGeomInfoSvc("PmtGeomInfoSvc")
    pgisvc.StreamItems = [ "/dd/Structure/DayaBay" ]   
    return

def run(app):
    '''
    Configure and add an algorithm to job
    '''
    app.ExtSvc += ["StaticCableSvc", "StatisticsSvc", "StaticCalibDataSvc",
                   "PmtGeomInfoSvc"]
    reconAnaAlg = ReconAnaAlg("ReconAnaAlg")
    app.addAlgorithm(reconAnaAlg)
    pass
