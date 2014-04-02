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

// **********************************************************************

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "GdLSExpParticleGun.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "GdLSExpAnalysisManager.hh"
#include "GdLSExpPrimaryGeneratorAction.hh"
#include "GdLSExpGeneratorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GdLSExpPrimaryGeneratorAction::GdLSExpPrimaryGeneratorAction()
{
  particleGun = new GdLSExpParticleGun (1);
  generatorMessenger = new GdLSExpGeneratorMessenger(this);
  
  m_energy = 1.0;
  m_particleName = "gamma";
}

GdLSExpPrimaryGeneratorAction::~GdLSExpPrimaryGeneratorAction()
{
  delete particleGun;
  delete generatorMessenger;
}

void GdLSExpPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //G4String particleName;
  GdLSExpAnalysisManager* analysis = GdLSExpAnalysisManager::getInstance();
  int nSeed = analysis->getSeed();
  if(anEvent->GetEventID() == 0) {
    for(int i = 0; i < nSeed; i++) {
      G4UniformRand();
    }
  }
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(m_particleName);
  //G4IonTable *theIonTable = (G4IonTable *)(G4ParticleTable::GetParticleTable()->GetIonTable());
  //G4ParticleDefinition* particle = theIonTable->GetIon(2,8,0.0);
  
  particleGun->SetParticleDefinition(particle);

  //G4double xpos = 25.0*cm;
  //G4double ypos = -2.5*mm + 5*G4UniformRand()*mm;
  G4double xpos = 0.0*cm;
  G4double ypos = 0.0*mm;
  G4double zpos = 0.0*mm;
  particleGun->SetParticlePosition(G4ThreeVector(xpos, ypos, zpos));
      
  //G4double pMag = 0.662*MeV;
  //G4double pMag = 1.173*MeV;
  //G4double pMag = 0.2*MeV;
  //G4double pMag = 0.4*MeV;
  //G4double pMag = 0.6*MeV;
  //G4double pMag = 0.8*MeV;
  //G4double pMag = 1.0*MeV;
  //G4double pMag = 1.2*MeV;
  //G4double pMag = 1.5*MeV;
  //G4double pMag = 2.0*MeV;
  //G4double pMag = 4.0*MeV;
  G4double pMag = m_energy * MeV;
  
  //randomize cos(theta)
  G4double costheta = 0.;
  //randomize phi
  G4double phi = 180.;
  phi = phi*degree;
  G4double sintheta = sqrt(1.-costheta*costheta);
  //computer 3-vector momentum
  G4ParticleMomentum aMomentum; 

  //************************
  //pMag = 0.0;
  //************************
  aMomentum[0] = pMag*sintheta*cos(phi);
  aMomentum[1] = pMag*sintheta*sin(phi);
  aMomentum[2] = pMag*costheta;  
  
  //use ParticleGun to generate event 
  particleGun->SetParticleMomentum(aMomentum);
  particleGun->GeneratePrimaryVertex(anEvent);
}

