#ifndef GdLSExpDetectorHit_h
#define GdLSExpDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GdLSExpDetectorHit : public G4VHit
{
public:
  
  GdLSExpDetectorHit();
  ~GdLSExpDetectorHit();
  GdLSExpDetectorHit(const GdLSExpDetectorHit&);
  const GdLSExpDetectorHit& operator=(const GdLSExpDetectorHit&);
  int operator==(const GdLSExpDetectorHit&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
public:
  void SetDetType    (G4int type)      { m_detType = type; };
  void SetTrackID    (G4int track)      { m_trackID = track; };
  void SetPDG    (G4int pdg)      { m_pdg = pdg; };
  void SetKE    (G4double ke)      { m_KE = ke; };
  void SetEdep   (G4double de)      { m_edep = de; };
  void SetQuenchedEdep (G4double qde)       { m_quenchedE = qde;};

  G4int GetDetType()      { return m_detType; };
  G4int GetTrackID()       { return m_trackID; };
  G4int GetPDG()       { return m_pdg; };      
  G4double GetKE()       { return m_KE; };      
  G4double GetEdep()       { return m_edep; };      
  G4double GetQuenchedEdep()  { return m_quenchedE;};

private:
  G4int         m_detType;
  G4int         m_trackID;
  G4int         m_pdg;
  G4double      m_KE;
  G4double      m_edep;
  G4double      m_quenchedE;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<GdLSExpDetectorHit> GdLSExpDetectorHitsCollection;

extern G4Allocator<GdLSExpDetectorHit> GdLSExpDetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* GdLSExpDetectorHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GdLSExpDetectorHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void GdLSExpDetectorHit::operator delete(void *aHit)
{
  GdLSExpDetectorHitAllocator.FreeSingle((GdLSExpDetectorHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
