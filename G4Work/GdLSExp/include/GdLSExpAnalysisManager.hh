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
#ifndef GdLSExpAnalysisManager_h
#define GdLSExpAnalysisManager_h 1

//---------------------------------------------------------------------------
//
// ClassName:   GdLSExpAnalysisManager
//
// Description: Singleton class to hold analysis parameters and build histograms.
//              User cannot access to the constructor.
//              The pointer of the only existing object can be got via
//              GdLSExpAnalysisManager::GetInstance() static method.
//              The first invokation of this static method makes
//              the singleton object.
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include <vector>
#include "GdLSExpAnalysisManagerMessenger.hh"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TObjString.h"
#include <string>
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
using namespace std;

class GdLSExpAnalysisManager
{

public:
  // With description

  static GdLSExpAnalysisManager* getInstance();
  static void dispose();

private:

  GdLSExpAnalysisManager();
  ~GdLSExpAnalysisManager();

public: // Without description

  void bookEventTree();

  void BeginOfRun();
  void EndOfRun();

  void BeginOfEvent();
  void EndOfEvent();

  void setInitMom(G4ThreeVector i) { m_initMom = i; }
  G4ThreeVector getInitMom() { return m_initMom; }

  void setFinalMom(G4ThreeVector i) { m_finalMom = i; }
  G4ThreeVector getFinalMom() { return m_finalMom; }

  void setFileName(TString i) { m_fileName = i; }
  void setSeed(TString i) { m_seed = std::atoi(i); }
  int getSeed() { return m_seed; }

  //event
  void SetEventID(Int_t i) { m_eventID = i; }
  void SetNPhotons(Int_t i) { m_nPhotons = i; }
  void SetPhotonE(Int_t i, Float_t j) { m_photonE[i] = j; }
  void SetDepE(Float_t i) { m_depE = i; }
  void SetQuenchedDepE(Float_t i) { m_quenchedDepE = i; }
  void SetAngle(Float_t i) { m_angle = i; }
  void SetIfTrig(Int_t i) { m_ifTrig = i; }

  void SetComptE(Int_t i, Float_t j) { m_elecE[i] = j; }
  Float_t GetComptE(Int_t i) { return m_elecE[i]; }

  void SetPrimaryE(Int_t i, Float_t j) { m_elecE[i] = j; }
  Float_t GetPrimaryE(Int_t i) { return m_elecE[i]; }

  void SetNTrack(Int_t i) { m_nTrack = i; }
  Int_t GetNTrack() { return m_nTrack; }

  void SetBremGamma(Int_t i) { m_isBremGamma[i] = 1; }
  Int_t GetBremGamma(Int_t i) { return m_isBremGamma[i]; }

  void SetNGamma(Int_t i) { m_nGamma = i; }
  Int_t GetNGamma() { return m_nGamma; }

  void SetGammaE(Int_t i, Float_t j) { m_gammaE[i] = j; }
  Float_t GetGammaE(Int_t i) { return m_gammaE[i]; }

  void SetPType(Int_t i, Int_t j) { m_pType[i] = j; }
  Int_t pType(Int_t i) {return m_pType[i]; }

  void SetParent(Int_t i, Int_t j) { m_parentID[i] = j; }
  Int_t parentID(Int_t i) { return m_parentID[i]; }

  void SetGamma(Int_t i) { m_isGamma[i] = 1; }
  Int_t isGamma(Int_t i) { return m_isGamma[i];  }

  void SetPositron(Int_t i) { m_isPositron[i] = 1; }
  Int_t isPositron(Int_t i) { return m_isPositron[i];  }

  void SetKE(Int_t i, Float_t j) { m_KE [i] = j; }
  Float_t KE(Int_t i) { return m_KE[i]; }

  void FillElectrondE(double dE) { m_electrondE->Fill(dE); }
  void FillPositrondE(double dE) { m_positrondE->Fill(dE); }
  void FillElectrondEdx(double dEdx) { m_electrondEdx->Fill(dEdx); }
  void FillPositrondEdx(double dEdx) { m_positrondEdx->Fill(dEdx); }
  void FilleBrem(double gammaE) { m_eBrem->Fill(gammaE); }
  void FillLowEnBrem(double gammaE) { m_LowEnBrem->Fill(gammaE); }
  void FilldEdx2D(double dE, double dx) { m_dEdx2D->Fill(dE, dx); }
  void FilldEStopPower2D(double dE, double dE_dx) { m_dEStopPower2D->Fill(dE, dE_dx); }

  void AddGammadE(Float_t j)  { m_totGammadE += j; }
  void IncreasePositronNumber() { m_positronNum += 1; }
  void IncreaseElectronNumber() { m_electronNum += 1; }

private:

  // MEMBERS
  static GdLSExpAnalysisManager* fManager;
  GdLSExpAnalysisManagerMessenger* messenger;

  TFile* newfile;

  TString m_fileName;

  int m_seed;

  TTree* evtTree;
  Int_t m_eventID;
  Int_t m_nPhotons;
  Int_t m_ifTrig;
  Float_t m_photonE[2000000];
  Float_t m_depE;
  Float_t m_quenchedDepE;
  Float_t m_angle;
  G4ThreeVector m_initMom;
  G4ThreeVector m_finalMom;

  TTree* comptonTree;
  Float_t m_elecE[50000];
  Int_t   m_nTrack;
  Int_t   m_nTrackTrue;
  Int_t   m_isBremGamma[2000];
  Float_t m_sumE;
  Int_t   m_nGamma;
  Int_t   m_nGammaTrue;
  Float_t m_sumGammaE;
  Float_t m_gammaE[2000];

  Int_t m_parentID[2000];
  Int_t m_isGamma[2000];
  Int_t m_isPositron[2000];
  Float_t m_KE[2000];
  Int_t m_pType[2000];
  Int_t m_isPrimary[2000];
  Float_t m_sumKE;
  Int_t m_nPrimaryTrack;
  Int_t m_totTrack;
  Float_t m_totGammadE;

  Int_t m_positronNum;
  Int_t m_electronNum;
  
  TH1F* m_electrondE;
  TH1F* m_electrondEdx;
  TH1F* m_positrondE;
  TH1F* m_positrondEdx;
  TH1F* m_eBrem;
  TH1F* m_LowEnBrem;
  TH1F* m_sumGammadE;
  TH2D* m_dEdx2D;
  TH2D* m_dEStopPower2D;
};

#endif
