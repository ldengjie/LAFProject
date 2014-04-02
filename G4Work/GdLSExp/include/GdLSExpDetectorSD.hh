#ifndef GdLSExpDetectorSD_h 
#define GdLSExpDetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "GdLSExpDetectorHit.hh"

class GdLSExpDetectorSD : public G4VSensitiveDetector
{

  public:
      GdLSExpDetectorSD(G4String name);
      ~GdLSExpDetectorSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      GdLSExpDetectorHitsCollection* GdLSCollection;
};




#endif
