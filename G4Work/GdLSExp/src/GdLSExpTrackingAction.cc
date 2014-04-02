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

#include "GdLSExpRunAction.hh"
#include "GdLSExpAnalysisManager.hh"
#include "GdLSExpTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"

void GdLSExpTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  // Create trajectory only for primaries
//  if(aTrack->GetParentID()==0)
//  { fpTrackingManager->SetStoreTrajectory(true); }
//  else
//  { fpTrackingManager->SetStoreTrajectory(false); }
  G4ParticleDefinition* pname = aTrack->GetDefinition();
  G4String spname=pname->GetParticleName();

  G4int parentID = aTrack->GetParentID();
  G4int trackID = aTrack->GetTrackID();

  if(parentID==1) {
    // primary electrons/positrons
    Int_t   nTrack = trackID-1;
    Float_t tmpE = GdLSExpAnalysisManager::getInstance()->GetComptE(nTrack-1);
    Float_t elecE = aTrack->GetKineticEnergy();

    // number of positrons
    if(spname=="e+")  { 
      GdLSExpAnalysisManager::getInstance()->IncreasePositronNumber();
      GdLSExpAnalysisManager::getInstance()->SetPrimaryE(nTrack-1, elecE);
      //cout << "find e+, trackID " << trackID << ", energy "<< elecE << endl; 
    }

    // number of electrons
    if(spname=="e-")  { 
      GdLSExpAnalysisManager::getInstance()->IncreaseElectronNumber();
      GdLSExpAnalysisManager::getInstance()->SetPrimaryE(nTrack-1, elecE);
      //cout << "find e+, trackID " << trackID << ", energy "<< elecE << endl; 
    }

    Int_t   maxTrk = GdLSExpAnalysisManager::getInstance()->GetNTrack();
    if(maxTrk<nTrack) { GdLSExpAnalysisManager::getInstance()->SetNTrack(nTrack); }

//    G4cout << "nTrack: "<< nTrack << G4endl;
//    G4cout << "particle, proc, kE, tmpE: " << spname << ", " 
//           << creator_proc << ", " << elecE << ", " << tmpE << G4endl;

    //if(tmpE<elecE) {
    //  GdLSExpAnalysisManager::getInstance()->SetComptE(nTrack-1, elecE);
    //}
  }

//
//  G4String creator_proc = "";
//  // Check if the track has the creator process (not the case for primaries)
//  const G4VProcess* creatorProcess = aTrack->GetCreatorProcess();
//  if(!creatorProcess) creator_proc="";
//  else creator_proc = creatorProcess->GetProcessName();
//
//  // particle Type, e-: 1, e+: 2, gamma: 3
//  Int_t ptype = 0;
//  if(spname == "e-") ptype = 1;
//  if(spname == "e+") ptype = 2;
//  if(spname == "gamma") ptype = 3;
//  GdLSExpAnalysisManager::getInstance()->SetPType(trackID, ptype);
//
//  // parent ID
//  GdLSExpAnalysisManager::getInstance()->SetParent(trackID, parentID);
//
//  // kinetic energy
//  Float_t kE = aTrack->GetKineticEnergy();
//  Float_t tmpkE = GdLSExpAnalysisManager::getInstance()->KE(trackID);
//  if(tmpkE<kE) {
//    GdLSExpAnalysisManager::getInstance()->SetKE(trackID, kE);
//  }
//
//  // gamma ID
//  if(spname == "gamma") GdLSExpAnalysisManager::getInstance()->SetGamma(trackID);
//
//  // positron ID
//  if(spname == "e+") GdLSExpAnalysisManager::getInstance()->SetPositron(trackID);
//
//  // capture gamma
//  if( creator_proc=="nCapture" && spname=="gamma" ) {
//    Int_t   nTrack = trackID-1;
//    Float_t gammaE = aTrack->GetKineticEnergy();
//    GdLSExpAnalysisManager::getInstance()->SetGammaE(nTrack-1, gammaE);
//
//    Int_t   maxTrk = GdLSExpAnalysisManager::getInstance()->GetNGamma();
//    if(maxTrk<nTrack) { GdLSExpAnalysisManager::getInstance()->SetNGamma(nTrack); }
//  }
//
}

void GdLSExpTrackingAction::PostUserTrackingAction(const G4Track* /*aTrack*/)
{
}
