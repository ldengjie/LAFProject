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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GdLSExpPhysicsList.hh"

#include "DsPhysConsElectroNu.h"
#include "DsPhysConsEM.h"
#include "DsPhysConsGeneral.h"
#include "DsPhysConsHadron.h"
#include "DsPhysConsIon.h"
#include "DsPhysConsOptical.h"

#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"

#include "G4ParticleTypes.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4Decay.hh"
#include "G4RadioactiveDecay.hh"
#include "DsG4RadioactiveDecay.hh"
#include "G4ProcessManager.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ParticlePropertyTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4DecayPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GdLSExpPhysicsList::GdLSExpPhysicsList() : G4VModularPhysicsList()
{
  cutForGamma     = 1.0*mm;
  cutForElectron  = 0.1*mm;
  cutForPositron  = 0.1*mm;
  

  SetVerboseLevel(1);

  //default physics
  electroNuList = new DsPhysConsElectroNu();
  emPhysicsList = new DsPhysConsEM();
  generalList = new DsPhysConsGeneral();
  hadronList = new DsPhysConsHadron();
  ionList = new DsPhysConsIon();
  //opticalList = new DsPhysConsOptical();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GdLSExpPhysicsList::~GdLSExpPhysicsList()
{
  delete electroNuList;
  delete emPhysicsList;
  delete generalList;
  delete hadronList;
  delete ionList;
  //delete opticalList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GdLSExpPhysicsList::ConstructParticle()
{
  {   // All bosons
        G4BosonConstructor con;
        con.ConstructParticle();
    }
    {   // All leptons
        G4LeptonConstructor con;
        con.ConstructParticle();
    }
    { /* Adopted a subset of mesons to avoid segv at the very end of
       * the run (seems to be related with B mesons and hLowEIoni???).
       * Carbon-copied the meson list from the example in
       * advanced/underground/physics. Jianglai 05/08/2006
       */
        G4PionPlus::PionPlusDefinition();
        G4PionMinus::PionMinusDefinition();
        G4PionZero::PionZeroDefinition();
        G4KaonPlus::KaonPlusDefinition();
        G4KaonMinus::KaonMinusDefinition();
        G4Eta::EtaDefinition();
        G4EtaPrime::EtaPrimeDefinition();
        G4KaonZero::KaonZeroDefinition();
        G4AntiKaonZero::AntiKaonZeroDefinition();
        G4KaonZeroLong::KaonZeroLongDefinition();
        G4KaonZeroShort::KaonZeroShortDefinition();
    }
    {   // All baryons
        G4BaryonConstructor con;
        con.ConstructParticle();
    }
    {   // All ions
        G4IonConstructor con;
        con.ConstructParticle();
    }

    {   // all short lived (resonaces and quarks)
        G4ShortLivedConstructor con;
        con.ConstructParticle();
    }

    /// Special hook: change the PDG encoded value for optical photons to be unique and useful.
    G4ParticlePropertyTable* propTable = G4ParticlePropertyTable::GetParticlePropertyTable();
    assert(propTable);
    G4ParticlePropertyData* photonData = propTable->GetParticleProperty(G4OpticalPhoton::Definition());
    assert(photonData);
    photonData->SetPDGEncoding(20022);
    photonData->SetAntiPDGEncoding(20022);
    if(propTable->SetParticleProperty(*photonData))
      G4cout << "Set PDG code for opticalphoton to " << G4OpticalPhoton::Definition()->GetPDGEncoding() << G4endl;
    else
      G4cout << "Failed to reset PDG code on opticalphoton.. it's still set to "
                << G4OpticalPhoton::Definition()->GetPDGEncoding() << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GdLSExpPhysicsList::ConstructProcess()
{
  AddTransportation();
  
  electroNuList->ConstructProcess();
  emPhysicsList->ConstructProcess();
  generalList->ConstructProcess();
  hadronList->ConstructProcess();
  ionList->ConstructProcess();
  //opticalList->ConstructProcess();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GdLSExpPhysicsList::SetCuts()
{
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  //this->SetCutsWithDefault();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GdLSExpPhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GdLSExpPhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GdLSExpPhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
