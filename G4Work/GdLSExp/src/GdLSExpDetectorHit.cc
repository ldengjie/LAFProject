#include "GdLSExpDetectorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<GdLSExpDetectorHit> GdLSExpDetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GdLSExpDetectorHit::GdLSExpDetectorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GdLSExpDetectorHit::~GdLSExpDetectorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GdLSExpDetectorHit::GdLSExpDetectorHit(const GdLSExpDetectorHit& right)
  : G4VHit()
{
  m_detType   = right.m_detType;
  m_trackID   = right.m_trackID;
  m_pdg       = right.m_pdg;
  m_KE        = right.m_KE;
  m_edep      = right.m_edep;
  m_quenchedE = right.m_quenchedE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const GdLSExpDetectorHit& GdLSExpDetectorHit::operator=(const GdLSExpDetectorHit& right)
{
  m_detType   = right.m_detType;
  m_trackID   = right.m_trackID;
  m_pdg       = right.m_pdg;
  m_KE        = right.m_KE;
  m_edep      = right.m_edep;
  m_quenchedE = right.m_quenchedE;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int GdLSExpDetectorHit::operator==(const GdLSExpDetectorHit& right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
