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
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#ifdef G4UI_USE_GAG
#include "G4UIGAG.hh"
#endif
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif 


#include "GdLSExpDetectorConstruction.hh"
#include "GdLSExpPhysicsList.hh"
#include "GdLSExpEventAction.hh"
#include "GdLSExpRunAction.hh"
#include "GdLSExpSteppingAction.hh"
#include "GdLSExpTrackingAction.hh"
#include "GdLSExpPrimaryGeneratorAction.hh"
#include "GdLSExpAnalysisManager.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

int main(int argc,char** argv)
{
  // Creation of the analysis manager
  GdLSExpAnalysisManager* messenger = GdLSExpAnalysisManager::getInstance();

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Creation of the analysis manager
  //GdLSExpAnalysisManager* messenger = GdLSExpAnalysisManager::getInstance();
  //CLHEP::HepRandomEngine* engine = & (CLHEP::HepRandom::getTheGenerator())->engine();
  //engine->setSeed(messenger->getSeed(), 0);

  // set mandatory initialization classes

  GdLSExpDetectorConstruction* Detector = new GdLSExpDetectorConstruction;
  runManager->SetUserInitialization(Detector);
  runManager->SetUserInitialization(new GdLSExpPhysicsList);
  //
  // Initialize G4 kernel
  //
  runManager->Initialize();

  // set mandatory user action class
  runManager->SetUserAction(new GdLSExpPrimaryGeneratorAction);
  runManager->SetUserAction(new GdLSExpRunAction);
  runManager->SetUserAction(new GdLSExpEventAction);
  runManager->SetUserAction(new GdLSExpTrackingAction);
  runManager->SetUserAction(new GdLSExpSteppingAction);
  //
  
  // random engine
  CLHEP::HepRandom::setTheSeed(messenger->getSeed());

  //
  G4UIsession* session=0;
  
//  if (argc==1)   // Define UI session for interactive mode.
//    {
//#ifdef G4UI_USE_TCSH
//      session = new G4UIterminal(new G4UItcsh);      
//#else
//      session = new G4UIterminal();
//#endif
//    }
// 
//#ifdef G4VIS_USE
//  // visualization manager
//  G4VisManager* visManager = new G4VisExecutive;
//  visManager->Initialize();
//#endif

  // Initialize G4 kernel
  // do this at run time so the geometry/physics can be changed
  //  runManager->Initialize();

  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  
  if (session)   // Define UI session for interactive mode.
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
      session->SessionStart();
      delete session;
    }
  else           // Batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }
  
  // job termination
//#ifdef G4VIS_USE
//  delete visManager;
//#endif
  GdLSExpAnalysisManager::dispose();
  delete runManager;

  return 0;
}

