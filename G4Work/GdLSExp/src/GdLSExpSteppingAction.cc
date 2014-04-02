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
#include "G4ios.hh"

#include "G4LossTableManager.hh"
#include "GdLSExpSteppingAction.hh"
#include "GdLSExpAnalysisManager.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4UImanager.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"

#ifdef G4_USE_G4BESTUNIT_FOR_VERBOSE
#include "G4UnitsTable.hh"
#else
#define G4BestUnit(a,b) a
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpSteppingAction::GdLSExpSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpSteppingAction::~GdLSExpSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GdLSExpSteppingAction::UserSteppingAction(const G4Step* fStep) 
{
  G4Track* fTrack = fStep->GetTrack();
  G4int StepNo = fTrack->GetCurrentStepNumber();
  if(StepNo >= 1000000) fTrack->SetTrackStatus(fStopAndKill);

  if(fTrack->GetTrackID() == 1) {
    if(StepNo == 1) {
      GdLSExpAnalysisManager::getInstance()->setInitMom(fTrack->GetMomentum());
    }
    
    if(fTrack->GetNextVolume() != 0) { 
      if((fTrack->GetNextVolume()->GetName()).find("World") != std::string::npos) {
        GdLSExpAnalysisManager::getInstance()->setFinalMom(fTrack->GetMomentum());
        GdLSExpAnalysisManager::getInstance()->SetIfTrig(1);
      }
    }
  } 
    
    G4ParticleDefinition* pname = fTrack->GetDefinition();
    G4String spname=pname->GetParticleName();
    // record dE/dx
    //if( spname == "e-" ) {
      //G4double dE = fStep->GetDeltaEnergy();
      G4double dE = fStep->GetTotalEnergyDeposit();
      G4double dx = fStep->GetStepLength();
      //G4cout << "StepNo, dE, dx" << Form("%5d %10.5f %10.5f %10.5f", StepNo, dE, dE_tot, dx) << G4endl;
      G4double dE_dx = dE / dx;
      GdLSExpAnalysisManager::getInstance()->FillElectrondE(dE/MeV);

      if(fTrack->GetDefinition() == G4Gamma::Gamma() && dE > 0)
      { 
        G4LossTableManager* manager = G4LossTableManager::Instance();
        G4double range = manager->GetRange(G4Electron::Electron(), dE, fTrack->GetMaterialCutsCouple());
        dE_dx = dE / range;

        GdLSExpAnalysisManager::getInstance()->AddGammadE(dE/MeV);
        //if( dE_dx/(MeV/cm)>15 && dE_dx/(MeV/cm)<150.) {
        //  G4cout << "spname, track ID: " << spname << ", " << fTrack->GetTrackID() << endl;
        //  G4cout << "track KE: " << fTrack->GetKineticEnergy() << endl;
        //  G4cout <<"dE, dx, range, corrected dE_dx = "<< dE/MeV << "MeV,  " 
        //         << dx/cm << "cm, " << range/cm << "cm, " 
        //         << dE_dx/(MeV/cm) << "MeV/cm" <<G4endl;
        //  if( fTrack->GetParentID()!=0 ) { 
        //    G4cout << "track status, StepNo, creator process: " << fTrack->GetTrackStatus() 
        //           << ", " << StepNo << ", " << fTrack->GetCreatorProcess()->GetProcessName() << endl;
        //  }
        //}
      }
      else {
        GdLSExpAnalysisManager::getInstance()->FillElectrondEdx(dE_dx / (MeV/cm));
        GdLSExpAnalysisManager::getInstance()->FilldEdx2D(dE/MeV, dx/cm);
        GdLSExpAnalysisManager::getInstance()->FilldEStopPower2D(dE/MeV, dE_dx/(MeV/cm) );

        //if( (dE_dx/(MeV/cm)>15 && dE_dx/(MeV/cm)<150) || fTrack->GetTrackStatus()==2) {
        //  G4cout << "spname, track ID: " << spname << ", " << fTrack->GetTrackID() << endl;
        //  G4cout << "track KE: " << fTrack->GetKineticEnergy() << endl;
        //  G4cout <<"dE, dx, dE_dx: " << dE/MeV << "MeV,  " 
        //         << dx/cm << "cm, " 
        //         << dE_dx/(MeV/cm) << "MeV/cm" <<G4endl;
        //  G4cout << "track status, StepNo, creator process: " << fTrack->GetTrackStatus() 
        //         << ", " << StepNo << ", " << fTrack->GetCreatorProcess()->GetProcessName() << endl;
        //}
      }

    //}
    //if( spname == "e+" ) {
    //  G4double dE = fStep->GetDeltaEnergy();
    //  G4double dE_tot = fStep->GetTotalEnergyDeposit();
    //  G4double dx = fStep->GetStepLength()/cm;
    //  //G4cout << "StepNo, dE, dx" << Form("%5d %10.5f %10.5f %10.5f", StepNo, dE, dE_tot, dx) << G4endl;
    //  GdLSExpAnalysisManager::getInstance()->FillPositrondE(dE_tot);
    //  GdLSExpAnalysisManager::getInstance()->FillPositrondEdx(dE_tot/dx);
    //}
    
    // record brem gamma

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
