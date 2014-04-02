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
        self.stats["file0/hists/scintX"] = TH1F("scintX", "Scint X (cm)", 
                                             500, -250, 250)
        
        self.stats["file0/hists/scintY"] = TH1F("scintY", "Scint Y (cm)", 
                                             500, -250, 250)
        
        self.stats["file0/hists/scintZ"] = TH1F("scintZ", "Scint Z (cm)", 
                                             500, -250, 250)

        self.stats["file0/hists/scintE"] = TH1F("scintE", "Scint E (MeV)", 
                                             1000, 0, 10)
        
        self.stats["file0/hists/croPmtPeakAdcPrf"] = TProfile("croPmtPeakAdcPrf", 
                       "charge profile for each PMT", 192, 0, 192, 0.0, 50, "e")

        return SUCCESS

    def execute(self):
        print "Executing ReconAnaAlg",self.name()

        # reset
        self.obsQ = array('d', 192*[0])
        self.expQ = array('d', 192*[0])
        self.totalObsQ = 0.0

        evt = self.evtSvc()
        
        # SimEvent Data
        simhdr = evt['/Event/Sim/SimHeader']
        if simhdr == None:
            roh = evt['/Event/Readout/ReadoutHeader']
            ilist = roh.findHeaders(51301)
            if ilist.size() == 1:
                simhdr = ilist[0]
            if simhdr == None:
                print "No SimHeader in this ReadOut. Skip."
                return SUCCESS

        statshdr = simhdr.unobservableStatistics()
        stats = statshdr.stats()

        self.qEdep = stats["QEDepInGdLS"].sum() + stats["QEDepInLS"].sum()
        scintX = stats["xQESumGdLS"].sum() + stats["xQESumLS"].sum()
        scintY = stats["yQESumGdLS"].sum() + stats["yQESumLS"].sum()
        scintZ = stats["zQESumGdLS"].sum() + stats["zQESumLS"].sum()
        if self.qEdep < 0.1:
           print "Low energy deposit in LS or GdLS. Skip."
           return SUCCESS

        scintX = scintX/self.qEdep
        scintY = scintY/self.qEdep
        scintZ = scintZ/self.qEdep

        genX = stats["x_Trk1"].sum()
        genY = stats["y_Trk1"].sum()
        genZ = stats["z_Trk1"].sum()

        # Get underlying DE object
        de = self.getDet(self.target_de_name)
        if not de:
            print 'Failed to get DE', self.target_de_name
            return FAILURE

        # Get the AD coordinates of the vertexes
        Gaudi = PyCintex.makeNamespace('Gaudi')
        scintGlbPoint = Gaudi.XYZPoint(scintX, scintY, scintZ)
        scintLclPoint = de.geometry().toLocal(scintGlbPoint)
        genGlbPoint = Gaudi.XYZPoint(genX, genY, genZ)
        genLclPoint = de.geometry().toLocal(genGlbPoint)

        self.scintX = scintLclPoint.x()
        self.scintY = scintLclPoint.y()
        self.scintZ = scintLclPoint.z()
        self.genX = genLclPoint.x()
        self.genY = genLclPoint.y()
        self.genZ = genLclPoint.z()

        self.stats["file0/hists/scintX"].Fill(self.scintX / units.centimeter)
        self.stats["file0/hists/scintY"].Fill(self.scintY / units.centimeter)
        self.stats["file0/hists/scintZ"].Fill(self.scintZ / units.centimeter)
        self.stats["file0/hists/scintE"].Fill(self.qEdep)

        radialRGen = math.sqrt(self.genX*self.genX+self.genY*self.genY)
        if radialRGen > 2000. or ROOT.TMath.Abs(self.genZ) > 2000: 
            self.info("Generated vertex is beyond the LS. Skip.")
            return SUCCESS
        
        # CalibReadoutEvent Data
        croHdr = evt["/Event/CalibReadout/CalibReadoutHeader"]
        if croHdr == None:
            self.error("Failed to get current calib readout header")
            return FAILURE
        
        readout = croHdr.calibReadout()
        if readout == None:
            self.info("No calibrated readout this cycle")
            print "scintE: ", self.qEdep, " MeV"
            return SUCCESS

        if readout.channelReadout().size() == 0:
            self.info("no channel Readout")
            return SUCCESS

        svcMode = ServiceMode(croHdr.context(), 0)
        for channel in readout.channelReadout():
            #channel = channelPair.second
            pmtId = channel.pmtSensorId().fullPackedData()
#pmtId = self.cableSvc.adPmtSensor(chanId, svcMode)
#            localId = (pmtId.ring()-1)*24 + (pmtId.column()-1)
            ring = (pmtId & 0x0000ff00)>>8
            column = (pmtId & 0x000000ff)
            localId = (ring-1)*24 + (column-1)
            self.obsQ[localId] = channel.maxCharge()
            self.totalObsQ = self.totalObsQ + channel.maxCharge()
            self.stats["file0/hists/croPmtPeakAdcPrf"].Fill(localId, channel.maxCharge())

        if self.totalObsQ < 50:
            self.info("Low total p.e number. Skip.")
            return SUCCESS

        # RecEvent Data
        recHdr = evt["/Event/Rec/AdMLRec"] 
        if recHdr == None:
            self.error("Failed to get current RecHeader")
            return FAILURE
        
        #recResults = recHdr.recTrigger()
        recTrigger = recHdr.recTrigger()
        if recTrigger == None:
            self.info("No recTrigger this cycle")
            return SUCCESS

        #for recPair in irange(recResults.begin(), recResults.end()):
        #recTrigger = recPair.second
        recName = "ML"
        histRecXName = "recX_" + recName
        histRecYName = "recY_" + recName
        histRecZName = "recZ_" + recName
        histDeltaXName = "deltaX_" + recName
        histDeltaYName = "deltaY_" + recName
        histDeltaZName = "deltaZ_" + recName
        histResoRName = "resoR_" + recName
        histDriftRName = "driftR_" + recName
        histRZName = "RZ_" + recName
        prfqmuRName = "qmuR_" + recName
        prfqmuThetaName = "qmuTheta_" + recName
        prfqmuQobsName = "qmuQobs_" + recName
        prfqmuPMTName = "qmuPMT_" + recName
        histErecRrecName = "hist_ErecR_" + recName
        histErecZrecName = "hist_ErecZ_" + recName
        histChi2TestName = "Chi2Test_" + recName
        vtxQualityName = "vtxQuality_" + recName
        
        prfErecRrecName = "prf_ErecRrec_" + recName
        prfErecZrecName = "prf_ErecZrec_" + recName

        prfQsumRtrueName = "prf_QsumR_" + recName
        prfQsumZtrueName = "prf_QsumZ_" + recName
        
        prfEratioRtrueName = "prf_EratioRtrue_" + recName
        prfEratioZtrueName = "prf_EratioZtrue_" + recName
        prfRbiasRtrueName = "prf_Rbias_Rtrue_" + recName
        prfZbiasZtrueName = "prf_Zbias_Ztrue_" + recName
        histRbiasRtrueName = "hist_Rbias_Rtrue_" + recName
        histZbiasZtrueName = "hist_Zbias_Ztrue_" + recName

        if self.firstEntry:
            # Make the histograms
            self.stats["file0/hists/%s"%histRecXName] = TH1F(histRecXName, 
                                "recX (cm)", 500, -250, 250)
        
            self.stats["file0/hists/%s"%histRecYName] = TH1F(histRecYName, 
                                "recY (cm)", 500, -250, 250)
        
            self.stats["file0/hists/%s"%histRecZName] = TH1F(histRecZName, 
                                "recZ (cm)", 500, -250, 250)
        
            self.stats["file0/hists/%s"%histDeltaXName] = TH1F(histDeltaXName, 
                                "recX - scintX (cm)", 500, -250, 250)
        
            self.stats["file0/hists/%s"%histDeltaYName] = TH1F(histDeltaYName, 
                                "recY - scintY (cm)", 500, -250, 250)
        
            self.stats["file0/hists/%s"%histDeltaZName] = TH1F(histDeltaZName, 
                                "recZ - scintZ (cm)", 500, -250, 250)
       
            self.stats["file0/hists/%s"%histResoRName] = TH1F(histResoRName, 
                                "|#vec{R}_{rec} - #vec{R}_{scint}| (cm)",
                                50, 0, 100)
        
            self.stats["file0/hists/%s"%histDriftRName] = TH1F(histDriftRName, 
                                "|#vec{R}_{rec}| - |#vec{R}_{scint}| (cm)",
                                200, -100, 100)
        
            self.stats["file0/hists/%s"%histRZName] = TH2F(histRZName, 
                                "R_{rec}^{radial} v.s Z_{rec} [cm]", 
                                250, 0, 250, 500, -250.0, 250.)

            self.stats["file0/hists/%s"%histErecRrecName] = TH2F(histErecRrecName, 
                                "E_{rec} (a.u) v.s (R_{rec}^{radial})^{2} [m]", 
                                60, 0.0, 6.0, 1000, 0.0, 10.)
        
            self.stats["file0/hists/%s"%histErecZrecName] = TH2F(histErecZrecName, 
                                "E_{rec} (a.u) v.s Z_{rec} [m]", 
                                50, -2.5, 2.5, 1000, 0.0, 10.)
        
            self.stats["file0/hists/%s"%prfErecRrecName] = TProfile(prfErecRrecName, 
                                "E_{rec} (a.u) v.s (R_{rec}^{radial})^{2} [m^{2}]", 
                                60, 0.0, 6.0, 0.0, 10., "e")
        
            self.stats["file0/hists/%s"%prfErecZrecName] = TProfile(prfErecZrecName, 
                                "E_{rec} (a.u) v.s Z_{rec} [m]", 
                                50, -2.5, 2.5, 0.0, 10., "e")
        
            self.stats["file0/hists/%s"%prfQsumZtrueName] = TProfile(prfQsumZtrueName, 
                                "totalCharge v.s Z_{true} [m]", 
                                50, -2.5, 2.5, 0.0, 500., "e")
        
            self.stats["file0/hists/%s"%prfQsumRtrueName] = TProfile(prfQsumRtrueName, 
                                "totalCharge v.s (R_{true}^{radial})^{2} [m^{2}]", 
                                60, 0, 6.0, 0.0, 500., "e")
        
            self.stats["file0/hists/%s"%prfEratioRtrueName] = TProfile(prfEratioRtrueName, 
                                "E_{rec}/E_{true} [a.u] v.s (R_{true}^{radial})^{2} [m]", 
                                60, 0., 6.0, 0.0, 5., "e")
        
            self.stats["file0/hists/%s"%prfEratioZtrueName] = TProfile(prfEratioZtrueName, 
                                "E_{rec}/E_{true} [a.u] v.s Z_{true} [m]", 
                                50, -2.5, 2.5, 0.0, 5., "e")
        
            self.stats["file0/hists/%s"%prfRbiasRtrueName] = TProfile(prfRbiasRtrueName, 
                                "R_{bias} [cm] v.s (R_{true}^{radial})^{2} [m]", 
                                60, 0., 6.0, -100., 100., "e")
        
            self.stats["file0/hists/%s"%prfZbiasZtrueName]  = TProfile(prfZbiasZtrueName, 
                                "Z_{bias} [cm] v.s Z_{true}^{2} [m]", 
                                50, -2.5, 2.5, -100., 100., "e")
        
            self.stats["file0/hists/%s"%histRbiasRtrueName] = TH2F(histRbiasRtrueName, 
                                "R_{bias} [cm] v.s (R_{true}^{radial})^{2} [m]", 
                                60, 0., 6.0, 400, -100., 100.)

            self.stats["file0/hists/%s"%histZbiasZtrueName]  = TH2F(histZbiasZtrueName, 
                                "Z_{bias} [cm] v.s Z_{true}^{2} [m]", 
                                50, -2.5, 2.5, 400, -100., 100.)
        
            self.stats["file0/hists/%s"%vtxQualityName] = TH1F(vtxQualityName, 
                                "vertex quality", 2000, 0, 4000.)

        recX = recTrigger.position().x()
        recY = recTrigger.position().y()
        recZ = recTrigger.position().z()
        recE = recTrigger.energy()
        vtxQuality = recTrigger.positionQuality()
        print "vtxQuality: ", vtxQuality

        vtxRec = ROOT.TVector3( recX, recY, recZ)
        vtxScint = ROOT.TVector3(self.scintX, self.scintY, self.scintZ)
                                    
        deltaX = recX - self.scintX 
        deltaY = recY - self.scintY
        deltaZ = recZ - self.scintZ
        vtxReso = ROOT.TVector3( deltaX, deltaY, deltaZ)
        driftR = vtxRec.Mag() - vtxScint.Mag()
        resoR  = vtxReso.Mag() 
        radialR2 = recX*recX + recY*recY
        radialR = math.sqrt(recX*recX + recY*recY)
        radialTrueR2 = (self.scintX*self.scintX + self.scintY*self.scintY)
        radialTrueR = math.sqrt(self.scintX*self.scintX + self.scintY*self.scintY)

        self.stats["file0/hists/%s"%histRecXName].Fill(recX/units.centimeter)
        self.stats["file0/hists/%s"%histRecYName].Fill(recY/units.centimeter)
        self.stats["file0/hists/%s"%histRecZName].Fill(recZ/units.centimeter)
        self.stats["file0/hists/%s"%histDeltaXName].Fill(deltaX/units.centimeter)
        self.stats["file0/hists/%s"%histDeltaYName].Fill(deltaY/units.centimeter)
        self.stats["file0/hists/%s"%histDeltaZName].Fill(deltaZ/units.centimeter)
        self.stats["file0/hists/%s"%histResoRName].Fill(resoR/units.centimeter)
        self.stats["file0/hists/%s"%histDriftRName].Fill(driftR/units.centimeter)
        self.stats["file0/hists/%s"%histRZName].Fill(radialR/units.centimeter, recZ/units.centimeter) 
        self.stats["file0/hists/%s"%histErecRrecName].Fill(radialR2/(units.meter*units.meter), recE)
        self.stats["file0/hists/%s"%histErecZrecName].Fill(recZ/units.meter, recE)

        self.stats["file0/hists/%s"%prfErecRrecName].Fill(radialR2/(units.meter*units.meter), recE)
        self.stats["file0/hists/%s"%prfErecZrecName].Fill(recZ/units.meter, recE)
        
        self.stats["file0/hists/%s"%prfQsumRtrueName].Fill(radialTrueR2/(units.meter*units.meter), self.totalObsQ)
        self.stats["file0/hists/%s"%prfQsumZtrueName].Fill(self.scintZ/units.meter, self.totalObsQ)

        self.stats["file0/hists/%s"%prfEratioRtrueName].Fill(radialTrueR2/(units.meter*units.meter), recE/self.qEdep)
        self.stats["file0/hists/%s"%prfEratioZtrueName].Fill(self.scintZ/units.meter, recE/self.qEdep)
        self.stats["file0/hists/%s"%prfRbiasRtrueName].Fill(radialTrueR2/(units.meter*units.meter), (radialR-radialTrueR)/units.centimeter)
        self.stats["file0/hists/%s"%prfZbiasZtrueName].Fill(self.scintZ/units.meter, (recZ-self.scintZ)/units.centimeter)
        self.stats["file0/hists/%s"%histRbiasRtrueName].Fill(radialTrueR2/(units.meter*units.meter), (radialR-radialTrueR)/units.centimeter)
        self.stats["file0/hists/%s"%histZbiasZtrueName].Fill(self.scintZ/units.meter, (recZ-self.scintZ)/units.centimeter)

        self.stats["file0/hists/%s"%vtxQualityName].Fill(vtxQuality)

        if self.firstEntry:
            self.stats["file0/hists/%s"%prfqmuRName] = TProfile(prfqmuRName, 
                                "Q_{obs}/Q_{exp} v.s R [m]",
                                60, 0.0, 6.0, -1.0, 500.0, "e")
    
            self.stats["file0/hists/%s"%prfqmuThetaName]=TProfile(prfqmuThetaName, 
                                "Q_{obs}/Q_{exp} (#cos#theta)",
                                50, 0.0, 1.0, -1.0, 500.0, "e")
    
            self.stats["file0/hists/%s"%prfqmuPMTName] = TProfile(prfqmuPMTName, 
                                "Q_{obs}/Q_{exp} (PMTLocalId)",
                                192, 0.0, 192, -1.0, 500.0, "e")
            self.stats["file0/hists/%s"%prfqmuQobsName]=TProfile(prfqmuQobsName,
                                 "Q_{obs}/Q_{exp} (Qobs)",
                                 50, 0.0, 50., -1.0, 500.0, "e")
    
            self.stats["file0/hists/%s"%histChi2TestName] = TH1F(histChi2TestName, 
                         "#chi^{2} test", 150, 0, 300)
   
            self.stats["file0/hists/recPmtExpQPrf"]=TProfile("recPmtExpQPrf", 
               "expected charge profile for each PMT", 192, 0, 192, 0.0, 50, "e")

        self.vertex = CLHEP.Hep3Vector(recX*units.mm, recY*units.mm, recZ*units.mm)

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
        site = self.siteIds[self.siteName]
        detector = self.detIds[self.detName]
        qtool = self.tool('IReconHelperTool', 'ExpQCalcTool')
        expq = qtool.expqcalc(site, detector, self.vertex)
       
        for localId in range(0, 192):
            expq[localId] = expq[localId]*recE*3.14*103*103*9000*0.2/4.0/3.14159625
            self.expQ[localId] = expq[localId]
            self.stats["file0/hists/recPmtExpQPrf"].Fill(localId,
                                                    self.expQ[localId])
        #
       
        self.chi2 = 0.0
        svcmode = ServiceMode(recHdr.context(), 0)
        for localId in range(0, 192):
            ring = localId/24 + 1
            column = localId%24 + 1
            pmtId = AdPmtSensor(ring, column, recTrigger.detector().site(),
                                recTrigger.detector().detectorId())
            chanId = self.cableSvc.feeChannelId(pmtId, svcmode)
            pmtCalib = self.calibSvc.pmtCalibData(pmtId, svcmode)
            if pmtCalib == None:
                self.error("No calib data for pmt local ID:%I" %localId)
                return FAILURE
                
            if pmtCalib.m_status != PmtCalibData.kGood:
                continue

            pmtPos = self.pmtSvc.get(pmtId.fullPackedData()).localPosition()
            pmtNorm = self.pmtSvc.get(pmtId.fullPackedData()).localDirection()
            distVec = ROOT.TVector3( recX - pmtPos.x(), 
                                     recY - pmtPos.y(),
                                     recZ - pmtPos.z() )
            dist = distVec.Mag()
            costheta =(distVec.x()*pmtNorm.x() + \
                       distVec.y()*pmtNorm.y() + \
                       distVec.z()*pmtNorm.z() )/dist
            dist = dist/1000.
            self.stats["file0/hists/%s"%prfqmuRName].Fill(dist, 
                                    self.obsQ[localId]/self.expQ[localId])
            self.stats["file0/hists/%s"%prfqmuThetaName].Fill(costheta, 
                                    self.obsQ[localId]/self.expQ[localId])
            self.stats["file0/hists/%s"%prfqmuQobsName].Fill(self.obsQ[localId],
                                    self.obsQ[localId]/self.expQ[localId])
            self.stats["file0/hists/%s"%prfqmuPMTName].Fill(localId, 
                                    self.obsQ[localId]/self.expQ[localId])
            self.chi2 += math.pow(self.obsQ[localId]-self.expQ[localId], 2) \
                   / self.expQ[localId]

        self.stats["file0/hists/%s"%histChi2TestName].Fill(self.chi2) 
#
        self.firstEntry = False
        return SUCCESS

        
    def finalize(self):        
        print "Finalizing ReconAnaAlg",self.name()
        status = DybPythonAlg.finalize(self)
        return status

def configure(argv = []):
    # Process module options
    import sys, getopt
    opts,args = getopt.getopt(argv,"o:")
    statsOutput = "recResultExample.root"
    for opt,arg in opts:
        if opt == "-o":
            statsOutput = arg
    from StatisticsSvc.StatisticsSvcConf import StatisticsSvc
    statsSvc = StatisticsSvc()
    statsSvc.Output = {"file0":statsOutput}
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
