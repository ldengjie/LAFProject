#include "GdLSExpDetectorSD.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4LossTableManager.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "GdLSExpAnalysisManager.hh"

GdLSExpDetectorSD::GdLSExpDetectorSD( G4String name )
  : G4VSensitiveDetector(name)
{
  collectionName.insert("GdLSExpDetectorHitsCollection");
}

GdLSExpDetectorSD::~GdLSExpDetectorSD()
{
}

void GdLSExpDetectorSD::Initialize(G4HCofThisEvent* HCE)
{
  GdLSCollection = new GdLSExpDetectorHitsCollection
                      (SensitiveDetectorName,collectionName[0]);

  static G4int HCID = -1;
  if(HCID < 0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection(HCID, GdLSCollection); 

}

G4bool GdLSExpDetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  const G4VPhysicalVolume* physVol
    = aStep->GetPreStepPoint()->GetPhysicalVolume();

  G4Track* fTrack = aStep->GetTrack();

  G4Material* fMaterial = fTrack->GetMaterial();

  const G4DynamicParticle* fParticle = fTrack->GetDynamicParticle();

  G4String volName = physVol->GetName();

  if(volName.find("Target") != std::string::npos) {
    if(edep == 0.) return false;

    G4double dE = edep;
    G4double dx = aStep->GetStepLength();
    G4double dE_dx = dE/dx;
    if(fTrack->GetDefinition() == G4Gamma::Gamma() && dE > 0)
    { 
      G4LossTableManager* manager = G4LossTableManager::Instance();
      dE_dx = dE/manager->GetRange(G4Electron::Electron(), dE, fTrack->GetMaterialCutsCouple());
    }
    
    G4double delta = dE_dx/fMaterial->GetDensity();//get scintillator density 

    G4double birk1 = 6.5e-3*g/cm2/MeV;
    if(fabs(fParticle->GetCharge())>1.5)//for particle charge greater than 1.
        birk1 = 0.57*birk1;
    
    G4double birk2 = 1.5e-6*(g/cm2/MeV)*(g/cm2/MeV);
   
    G4double QuenchedTotalEnergyDeposit
        = edep/(1+birk1*delta+birk2*delta*delta);

    GdLSExpDetectorHit* hit = new GdLSExpDetectorHit();
    hit->SetDetType( 0 );
    hit->SetTrackID( fTrack->GetTrackID() );
    hit->SetPDG( fTrack->GetDefinition()->GetPDGEncoding() );
    hit->SetKE( fTrack->GetKineticEnergy() );
    hit->SetEdep( edep );
    hit->SetQuenchedEdep( QuenchedTotalEnergyDeposit );

    G4int ihit = GdLSCollection->insert( hit );
    return true;
  }

  if(volName.find("PMT") != std::string::npos) {

    if( fTrack->GetDefinition()->GetPDGEncoding() != 20022) return false;
    GdLSExpDetectorHit* hit = new GdLSExpDetectorHit();
    hit->SetDetType( 1 );
    hit->SetTrackID( fTrack->GetTrackID() );
    hit->SetPDG( fTrack->GetDefinition()->GetPDGEncoding() );
    hit->SetKE( fTrack->GetKineticEnergy() );
    hit->SetEdep( 0 );
    hit->SetQuenchedEdep( 0 );

    G4int ihit = GdLSCollection->insert( hit );
    return true;
  }

  return false;
}

void GdLSExpDetectorSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  G4double totalEdep = 0;
  G4double totalQuenchedEdep = 0;
  G4int nPhotons = 0;
  G4double photonE = 0;
  std::vector<int> vTrackID;
  vTrackID.clear();

  G4int NbHits = GdLSCollection->entries();
  for (G4int i=0;i<NbHits;i++) {
    if( (*GdLSCollection)[i]->GetDetType() == 0 ) {
      totalEdep += (*GdLSCollection)[i]->GetEdep();
      totalQuenchedEdep += (*GdLSCollection)[i]->GetQuenchedEdep();
    }
    if( (*GdLSCollection)[i]->GetDetType() == 1 ) {
      G4int trackID = (*GdLSCollection)[i]->GetTrackID();
      if(find(vTrackID.begin(), vTrackID.end(), trackID) == vTrackID.end()) {
        vTrackID.push_back(trackID);
        photonE = (*GdLSCollection)[i]->GetKE();
        GdLSExpAnalysisManager::getInstance()->SetPhotonE(nPhotons, photonE);
        nPhotons++;
      }
    }
  }

  GdLSExpAnalysisManager::getInstance()->SetNPhotons(nPhotons);
  GdLSExpAnalysisManager::getInstance()->SetDepE(totalEdep);
  GdLSExpAnalysisManager::getInstance()->SetQuenchedDepE(totalQuenchedEdep);

  G4ThreeVector p1 = GdLSExpAnalysisManager::getInstance()->getInitMom();
  G4ThreeVector p2 = GdLSExpAnalysisManager::getInstance()->getFinalMom();
  G4double angle = p1.angle(p2);

  GdLSExpAnalysisManager::getInstance()->SetAngle(angle);
}
