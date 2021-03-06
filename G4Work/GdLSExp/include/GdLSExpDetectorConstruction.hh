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
#ifndef GdLSExpDetectorConstruction_h
#define GdLSExpDetectorConstruction_h

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "OpticalProperty.icc"


class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GdLSExpDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
     GdLSExpDetectorConstruction();
    ~GdLSExpDetectorConstruction();

  public:
  
     G4VPhysicalVolume* Construct();
     
  private:
     void DefineMaterials();
     
  private: 

     G4Box*             solidWorld;    // pointer to the solid envelope 
     G4LogicalVolume*   logicWorld;    // pointer to the logical envelope
     G4VPhysicalVolume* physiWorld;    // pointer to the physical envelope
     
     G4Tubs*             solidTarget;   // pointer to the solid Target
     G4LogicalVolume*    logicTarget;   // pointer to the logical Target
     G4VPhysicalVolume*  physiTarget;   // pointer to the physical Target

     G4Tubs*             solidAcrylic;
     G4LogicalVolume*    logicAcrylic;
     G4VPhysicalVolume*  physiAcrylic;

     G4Tubs*             solidCoverMom;
     G4LogicalVolume*   logicCoverMom;
     G4VPhysicalVolume* physiCoverMom;
      
     G4Tubs*             solidCover;
     G4LogicalVolume*   logicCover;
     G4VPhysicalVolume* physiCover;

     G4Tubs*             solidTopCoverMom;
     G4LogicalVolume*   logicTopCoverMom;
     G4VPhysicalVolume* physiTopCoverMom;

     G4Tubs*             solidTopCover;
     G4LogicalVolume*   logicTopCover;
     G4VPhysicalVolume* physiTopCover;

     G4Tubs*             solidPMT;
     G4LogicalVolume*   logicPMT;
     G4VPhysicalVolume* physiPMT;

     G4Material* Galactic;          // Default material
     G4Material* Air;          // Default material
     G4Material* GdLS;           // Target material
     G4Material* ESR;         // Detector material
     G4Material* Tyvek;         // Detector material
     G4Material* Acrylic;         // Detector material
     G4Material* Teflon;         // Detector material

};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
