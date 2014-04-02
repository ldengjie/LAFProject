//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "GdLSExpAnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProductionCutsTable.hh"
#include "TFile.h"
#include "TTree.h"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace CLHEP;
GdLSExpAnalysisManager* GdLSExpAnalysisManager::fManager = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpAnalysisManager* GdLSExpAnalysisManager::getInstance()
{
  if(!fManager) {
    fManager = new GdLSExpAnalysisManager();
  }
  return fManager;
}
void GdLSExpAnalysisManager::dispose()
{
  delete fManager;
  fManager = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpAnalysisManager::GdLSExpAnalysisManager()
{ 
  evtTree = 0;
  comptonTree = 0;

  messenger = new GdLSExpAnalysisManagerMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpAnalysisManager::~GdLSExpAnalysisManager()
{
}

void GdLSExpAnalysisManager::bookEventTree()
{

  evtTree = new TTree("evt", "evtTreeInfo"); //record infor of a step that will generate an ion.
  evtTree->Branch("nPhotons", &m_nPhotons, "nPhotons/I");
  evtTree->Branch("photonE", m_photonE, "photonE[nPhotons]/F");
  evtTree->Branch("depE", &m_depE, "depE/F");
  evtTree->Branch("quenchedDepE", &m_quenchedDepE, "quenchedDepE/F");
  evtTree->Branch("angle", &m_angle, "angle/F");
  evtTree->Branch("ifTrig", &m_ifTrig, "ifTrig/I");

  comptonTree = new TTree("compton", "comptonInfo");
  comptonTree->Branch("nTrackTrue", &m_nTrackTrue, "nTrackTrue/I");
  comptonTree->Branch("nTrack", &m_nTrack, "nTrack/I");
  comptonTree->Branch("elecE", m_elecE, "elecE[nTrack]/F");
  comptonTree->Branch("sumE", &m_sumE, "sumE/F");
  comptonTree->Branch("positronNum", &m_positronNum, "positronNum/I");
  comptonTree->Branch("electronNum", &m_electronNum, "electronNum/I");

  comptonTree->Branch("nGammaTrue", &m_nGammaTrue, "nGammaTrue/I");
  comptonTree->Branch("nGamma", &m_nGamma, "nGamma/I");
  comptonTree->Branch("sumGammaE", &m_sumGammaE, "sumGammaE/F");
  comptonTree->Branch("gammaE", m_gammaE, "gammaE[nGamma]/F");

  comptonTree->Branch("totTrack", &m_totTrack, "totTrack/I");
  comptonTree->Branch("nPrimaryTrack", &m_nPrimaryTrack, "nPrimaryTrack/I");
  comptonTree->Branch("KE", m_KE, "KE[totTrack]/F");
  comptonTree->Branch("isPrimary", m_isPrimary, "isPrimary[totTrack]/I");
  comptonTree->Branch("sumKE", &m_sumKE, "sumKE/F");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GdLSExpAnalysisManager::BeginOfRun()
{
  G4cout << "GdLSExpAnalysisManager: Histograms are booked and the run has been started" << G4endl;
  newfile = new TFile(m_fileName, "recreate");
  bookEventTree();
  m_electrondE = new TH1F("electrondE", "electrondE", 1000, 0, 1); // MeV
  m_positrondE = new TH1F("positrondE", "positrondE", 1000, 0, 1);
  m_electrondEdx = new TH1F("electrondEdx", "electrondEdx", 10000, 0, 100); // MeV/cm
  m_positrondEdx = new TH1F("positrondEdx", "positrondEdx", 10000, 0, 100);
  m_sumGammadE = new TH1F("sumGammadE", "sumGammadE", 10000, 0, 1);
  m_dEdx2D= new TH2D("dEdx2D", "dEdx2D", 2000, 0, 1.0, 2000, 0, 1.0);
  m_dEStopPower2D= new TH2D("dEStopPower2D", "dEStopPower2D", 1000, 0, 2.0, 4000, 0, 100.0);

  m_eBrem = new TH1F("eBrem", "eBrem", 1000, 0, 10);
  m_LowEnBrem = new TH1F("LowEnBrem", "LowEnBrem", 1000, 0, 10);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GdLSExpAnalysisManager::EndOfRun()
{
  evtTree->Write();
  comptonTree->Write();
  m_electrondE->Write();
  m_positrondE->Write();
  m_electrondEdx->Write();
  m_positrondEdx->Write();
  m_eBrem->Write();
  m_LowEnBrem->Write();
  m_sumGammadE->Write();
  m_dEdx2D->Write();
  m_dEStopPower2D->Write();
  newfile->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GdLSExpAnalysisManager::BeginOfEvent()
{
  m_nPhotons = 0;
  m_depE = 0;
  m_quenchedDepE = 0;
  m_angle = 0;
  m_ifTrig = 0;
  for(int i = 0; i < 2000000; i++) {
    m_photonE[i] = 0;
  }

  m_nTrack = 0;
  m_nTrackTrue = 0;
  m_sumE = 0;
  for(int i = 0; i < 50000; i++) {
    m_elecE[i] = 0;
  }
  for(int i=0; i<2000; i++) {
    m_isBremGamma[i] = 0;
  }

  m_nGamma = 0;
  m_nGammaTrue = 0;
  m_sumGammaE = 0;
  for(int i=0; i<2000; i++) {
    m_gammaE[i] = 0;
  }
  
  for(int i=0; i<2000; i++) {
    m_pType[i] = 0;
    m_KE[i] = 0.0;
    m_isGamma[i] = 0;
    m_isPositron[i] = 0;
    m_parentID[i] = 0;
    m_isPrimary[i] = 0;
  }
  m_nPrimaryTrack = 0;
  m_totTrack= 0;
  m_sumKE = 0.;

  m_totGammadE = 0.;

  m_positronNum= 0.;
  m_electronNum= 0.;

  //G4ProductionCutsTable::GetProductionCutsTable()->DumpCouples();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GdLSExpAnalysisManager::EndOfEvent()
{
  for(int i=0; i<m_nTrack; i++) {
    m_sumE += m_elecE[i];
    if(m_elecE[i]!=0.) { 
      m_nTrackTrue++;
      //G4cout << Form("%5d %10.5f", i+2, m_elecE[i]) << G4endl;
    }
  }
  //cout << "sumE: " << m_sumE << G4endl;

  for(int i=0; i<m_nGamma; i++) {
    m_sumGammaE += m_gammaE[i];
    if(m_gammaE[i]!=0.) m_nGammaTrue++;
  }

  //
  for(int i=0; i<2000; i++ ) {
    if( m_pType[i]!=0 ) m_totTrack++;
    if( !(m_pType[i]==1 || m_pType[i]==2) ) continue;

    Int_t parentId = m_parentID[i];
    while( m_isGamma[parentId] || m_isPositron[parentId] ) { // parent is a gamma or a positron
      if(m_isGamma[parentId]) {
        parentId = m_parentID[parentId]; // gamma's parent
      }

      if(m_isPositron[parentId]) {
        parentId = m_parentID[parentId]; // positron's parent
        if(m_isGamma[parentId]) { // positron's parent is gamma
          parentId = m_parentID[parentId];
        }
        else {
          break;
        }
      }
    }

    if(parentId==0) { // trace back to the origin gamma
      m_sumKE += m_KE[i];
      m_isPrimary[i] = 1;
      m_nPrimaryTrack++;
      //G4cout << Form("%5d %10.5f %10.5f", i, m_KE[i], m_elecE[i]) << G4endl;
    }
  }
  //cout << "sumKE: " << m_sumKE << G4endl;

//  if(m_sumE>6.) G4cout << "m_sumE>6.! m_sumE: " << m_sumE << G4endl;

//  //if(m_sumE<1.16) {
//    G4cout << "nTrack, sumE: " << m_nTrack << ",  " << m_sumE << G4endl;
//    for(int i=0; i<m_nTrack; i++) {
//      G4cout << Form("%6.3f", m_elecE[i]); 
//    }
//  //}
//  G4cout << G4endl;
  m_sumGammadE->Fill(m_totGammadE);
  //G4cout << "m_totGammadE: " << m_totGammadE << " MeV" << G4endl;
  evtTree->Fill();
  comptonTree->Fill();
}
