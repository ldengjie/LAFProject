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
// CHANGE HISTORY
// --------------
#include "G4ios.hh"
#include "GdLSExpEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4UImanager.hh"
#include "GdLSExpAnalysisManager.hh"

extern G4bool drawEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpEventAction::GdLSExpEventAction()
  : drawFlag("all")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpEventAction::~GdLSExpEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GdLSExpEventAction::BeginOfEventAction(const G4Event* evt)
{
  //if(evt->GetEventID()%1000 == 0) G4cout << "Begin of Event --> " << evt->GetEventID() << G4endl;
  G4cout << "Begin of Event --> " << evt->GetEventID() << G4endl;

  //G4UImanager* uiMgr = G4UImanager::GetUIpointer();
  //if(evt->GetEventID() == 79)
  // uiMgr->ApplyCommand("/tracking/verbose 5");
  //else
  //  uiMgr->ApplyCommand("/tracking/verbose 0");

  GdLSExpAnalysisManager::getInstance()->BeginOfEvent();

  GdLSExpAnalysisManager::getInstance()->SetEventID(evt->GetEventID());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GdLSExpEventAction::EndOfEventAction(const G4Event* evt)
{
  GdLSExpAnalysisManager::getInstance()->EndOfEvent();

  // visualisation
#ifdef G4VIS_USE
  G4int event_id       = evt->GetEventID();
  if (event_id < 100 && G4VVisManager::GetConcreteInstance()) {
    G4TrajectoryContainer * trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
    for (G4int i=0; i<n_trajectories; i++) {
      G4Trajectory* trj = (G4Trajectory *)
        ((*(evt->GetTrajectoryContainer()))[i]);
      if (drawFlag == "all") {
        trj->DrawTrajectory(0);
      } else if (drawFlag == "charged" && trj->GetCharge() != 0.) {
        trj->DrawTrajectory(0);
      }
    }
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

