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
#include "GdLSExpDetectorConstruction.hh"
#include "GdLSExpDetectorSD.hh"

#include "G4UImanager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include <sstream>
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GdLSExpDetectorConstruction::GdLSExpDetectorConstruction()
:solidWorld(0),  logicWorld(0),  physiWorld(0),
 solidTarget(0), logicTarget(0), physiTarget(0), 
 solidAcrylic(0),logicAcrylic(0),physiAcrylic(0), 
 solidCoverMom(0),logicCoverMom(0),physiCoverMom(0), 
 solidCover(0),logicCover(0),physiCover(0), 
 solidTopCoverMom(0),logicTopCoverMom(0),physiTopCoverMom(0), 
 solidTopCover(0),logicTopCover(0),physiTopCover(0), 
 solidPMT(0),logicPMT(0),physiPMT(0), 
 Air(0), GdLS(0), ESR(0), Tyvek(0), Acrylic(0),Teflon(0),Galactic(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GdLSExpDetectorConstruction::~GdLSExpDetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void GdLSExpDetectorConstruction::DefineMaterials()
{

//--------- Material definition ---------
  G4Isotope* aIsotope1 = new G4Isotope("Gd_152", 64, 152, 152.0*g/mole);
  G4Isotope* aIsotope2 = new G4Isotope("Gd_154", 64, 154, 154.0*g/mole);
  G4Isotope* aIsotope3 = new G4Isotope("Gd_155", 64, 155, 155.0*g/mole);
  G4Isotope* aIsotope4 = new G4Isotope("Gd_156", 64, 156, 156.0*g/mole);
  G4Isotope* aIsotope5 = new G4Isotope("Gd_157", 64, 157, 157.0*g/mole);
  G4Isotope* aIsotope6 = new G4Isotope("Gd_158", 64, 158, 158.0*g/mole);
  G4Isotope* aIsotope7 = new G4Isotope("Gd_160", 64, 160, 160.0*g/mole);

  G4Element* Gd = new G4Element("Gadolinium", "Gd", 7);
  Gd->AddIsotope(aIsotope1, 100.0*0.002*152/157.318*perCent);
  Gd->AddIsotope(aIsotope2, 100.0*0.022*154/157.318*perCent);
  Gd->AddIsotope(aIsotope3, 100.0*0.149*155/157.318*perCent);
  Gd->AddIsotope(aIsotope4, 100.0*0.206*156/157.318*perCent);
  Gd->AddIsotope(aIsotope5, 100.0*0.157*157/157.318*perCent);
  Gd->AddIsotope(aIsotope6, 100.0*0.247*158/157.318*perCent);
  Gd->AddIsotope(aIsotope7, 100.0*0.217*160/157.318*perCent);

  G4Element* C = new G4Element("Carbon", "C" , 6., 12.01*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H" , 1., 1.01*g/mole);
  G4Element* O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
  G4Element* N  = new G4Element("Nitrogen", "N", 7., 14.01*g/mole);
  G4Element* Si = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
  G4Element* Al = new G4Element("Aluminium", "Al", 13., 26.98*g/mole);
  G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.845*g/mole);
  G4Element* Mg = new G4Element("Magnesium", "Mg", 12., 24.305*g/mole);
  G4Element* Ca = new G4Element("Calcium", "Ca", 20., 40.078*g/mole);
  G4Element* Na = new G4Element("Sodium", "Na", 11., 22.98977*g/mole);
  G4Element* K = new G4Element("Potassium", "K", 19., 39.0983*g/mole);
  G4Element* S = new G4Element("Sulfur", "S", 16., 32.066*g/mole);
  G4Element* F = new G4Element("Fluorine", "F", 9., 18.998*g/mole);
   
  Galactic = new G4Material("Galactic", 1., 1.01*g/mole, universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);
 
  Air = new G4Material("Air", 1.29*mg/cm3, 2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
  airMPT->AddProperty("RINDEX",    AirEnergy, AirRefIndex,     4);
  airMPT->AddProperty("ABSLENGTH", AirEnergy, AirAbsLength,  4);

  Air->SetMaterialPropertiesTable(airMPT);

  GdLS  = new G4Material("GdDopedLS", 0.8602*g/cm3, 6);
  GdLS->AddElement(C,  0.87705);
  GdLS->AddElement(H,  0.12051);
  GdLS->AddElement(O,  0.00109);
  GdLS->AddElement(Gd, 0.0010315);
  GdLS->AddElement(N,  0.00027);
  GdLS->AddElement(S,  0.00005);

  G4MaterialPropertiesTable* GdLSMPT = new G4MaterialPropertiesTable();
  GdLSMPT->AddProperty("RINDEX",   GdLSRefIndexEnergy, GdLSRefIndex, 18);
  GdLSMPT->AddProperty("ABSLENGTH", GdLSABSEnergy, GdLSABSLength, 497);
  GdLSMPT->AddProperty("FASTCOMPONENT", GdLSComEnergy, GdLSFastComponent, 275);
  GdLSMPT->AddProperty("SLOWCOMPONENT", GdLSComEnergy, GdLSFastComponent, 275);
  GdLSMPT->AddProperty("REEMISSIONPROB", GdLSReemEnergy, GdLSReem, 28);
  GdLSMPT->AddProperty("RAYLEIGH", GdLSRayEnergy, GdLSRayLength, 11);
  GdLSMPT->AddProperty("SCINTILLATIONYIELD", component, GdLSLY,2);
  GdLSMPT->AddProperty("RESOLUTIONSCALE", component, GdLSResolutionScale,2);
  GdLSMPT->AddProperty("GammaFASTTIMECONSTANT", component, GdLSFastTimeConstant,2);
  GdLSMPT->AddProperty("GammaSLOWTIMECONSTANT", component, GdLSSlowTimeConstant,2);
  GdLSMPT->AddProperty("GammaYIELDRATIO", component, GdLSYieldRatio,2);

  GdLS->SetMaterialPropertiesTable(GdLSMPT);

 // LS = new G4Material("LS", 0.859*g/cm3, 5);
 // LS->AddElement(C, 0.87924);
 // LS->AddElement(H, 0.1201);
 // LS->AddElement(O, 0.00034);
 // LS->AddElement(N, 0.00027);
 // LS->AddElement(S, 0.00005);

  ESR = new G4Material("ESR", 1.0*g/cm3, 2);
  ESR->AddElement(H, 2.*1.00794/(12.0107+2.*1.00794));
  ESR->AddElement(C, 12.0107/(12.0107+2.*1.00794));

  G4MaterialPropertiesTable* ESRMPT = new G4MaterialPropertiesTable();
  ESRMPT->AddProperty("ABSLENGTH", ESREnergy, ESRAbsorptionLength, 31);

  ESR->SetMaterialPropertiesTable(ESRMPT);

  Tyvek = new G4Material("Tyvek", 0.94*g/cm3, 2);
  Tyvek->AddElement(C, 2);
  Tyvek->AddElement(H, 4);

  G4MaterialPropertiesTable* TyvekMPT = new G4MaterialPropertiesTable();
  TyvekMPT->AddProperty("ABSLENGTH", TyvekEnergy, TyvekABSLength, 4);
  
  Tyvek->SetMaterialPropertiesTable(TyvekMPT);

  Acrylic = new G4Material("Acrylic", 1.18*g/cm3, 3);
  Acrylic->AddElement(C, 0.59984);
  Acrylic->AddElement(H, 0.08055);
  Acrylic->AddElement(O, 0.31961);

  G4MaterialPropertiesTable* AcrylicMPT = new G4MaterialPropertiesTable();
  AcrylicMPT->AddProperty("ABSLENGTH", AcrylicAbsEnergy, AcrylicAbsLength, 9);
  AcrylicMPT->AddProperty("RAYLEIGH", AcrylicRayEnergy, AcrylicRayLength, 11);
  AcrylicMPT->AddProperty("RINDEX", AcrylicRefEnergy, AcrylicRefIndex, 18);

  Acrylic->SetMaterialPropertiesTable(AcrylicMPT);

  Teflon = new G4Material("Teflon", 2.2*g/cm3, 2);
  Teflon->AddElement(C, 0.759814);
  Teflon->AddElement(F, 0.240186);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* GdLSExpDetectorConstruction::Construct()
{
  DefineMaterials();

  //------------------------------ 
  // World
  //------------------------------ 

  solidWorld= new G4Box("sWorld", 11.0*m, 11.0*m, 11.0*m);
  logicWorld= new G4LogicalVolume( solidWorld, Galactic, "lWorld", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
				 "pWorld",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0);              // no field specific to volume
	

  solidAcrylic = new G4Tubs("sAcrylic", 0*cm, 1010*cm, 1010*cm, 0, twopi);
  logicAcrylic = new G4LogicalVolume(solidAcrylic, Acrylic, "lAcrylic",0,0,0);
  physiAcrylic = new G4PVPlacement(0,              // no rotation
                                  G4ThreeVector(0,0,0), // at (x,y,z)
                                  logicAcrylic,    // its logical volume                                 
                                  "pAcrylic",       // its name
                                  logicWorld,      // its mother  volume
                                  false,           // no boolean operations
                                  0);              // no particular field

  solidTarget = new G4Tubs("sTarget", 0*cm, 1000*cm, 1000*cm, 0, twopi);
  logicTarget = new G4LogicalVolume(solidTarget, GdLS, "lTarget",0,0,0);
  physiTarget = new G4PVPlacement(0,              // no rotation
                                  G4ThreeVector(0,0,0), // at (x,y,z)
                                  logicTarget,    // its logical volume                                 
                                  "pTarget",       // its name
                                  logicAcrylic,      // its mother  volume
                                  false,           // no boolean operations
                                  0);              // no particular field

  //solidCoverMom = new G4Tubs("sCoverMom", 2.6*cm, 2.7*cm, 2.6*cm, 0, twopi);
  //logicCoverMom = new G4LogicalVolume(solidCoverMom, Acrylic, "lCoverMom",0,0,0);
  //physiCoverMom = new G4PVPlacement(0,              // no rotation
  //                                G4ThreeVector(0,0,0), // at (x,y,z)
  //                                logicCoverMom,    // its logical volume                                 
  //                                "pCoverMom",       // its name
  //                                logicWorld,      // its mother  volume
  //                                false,           // no boolean operations
  //                                0);              // no particular field

  // Cover
//  solidCover = new G4Tubs("sCover", 2.6*cm, 2.7*cm, 2.6*cm, 0, twopi);
//  logicCover = new G4LogicalVolume(solidCover, Teflon, "lCover",0,0,0);
//  physiCover = new G4PVPlacement(0,              // no rotation
//                                  G4ThreeVector(0,0,0), // at (x,y,z)
//                                  logicCover,    // its logical volume                                 
//                                  "pCover",       // its name
//                                  logicWorld,      // its mother  volume
//                                  false,           // no boolean operations
//                                  0);              // no particular field

  solidPMT = new G4Tubs("sPMT", 0, 1020*cm, 0.5*cm, 0, twopi);
  logicPMT = new G4LogicalVolume(solidPMT, Air, "lPMT",0,0,0);
  physiPMT = new G4PVPlacement(0,              // no rotation
                               G4ThreeVector(0,0,-1050*cm), // at (x,y,z)
                               logicPMT,    // its logical volume                                 
                               "pPMT",       // its name
                               logicWorld,      // its mother  volume
                               false,           // no boolean operations
                               0);              // no particular field


  //solidTopCoverMom = new G4Tubs("sTopCoverMom", 0.*cm, 2.7*cm, 0.1*cm, 0, twopi);
  //logicTopCoverMom = new G4LogicalVolume(solidTopCoverMom, Acrylic, "lTopCoverMom",0,0,0);
  //physiTopCoverMom = new G4PVPlacement(0,              // no rotation
  //                                G4ThreeVector(0,0,2.7*cm), // at (x,y,z)
  //                                logicTopCoverMom,    // its logical volume                                 
  //                                "pTopCoverMom",       // its name
  //                                logicWorld,      // its mother  volume
  //                                false,           // no boolean operations
  //                                0);              // no particular field

  // Top Cover
//  solidTopCover = new G4Tubs("sTopCover", 0.*cm, 2.7*cm, 0.05*cm, 0, twopi);
//  logicTopCover = new G4LogicalVolume(solidTopCover, ESR, "lTopCover",0,0,0);
//  physiTopCover = new G4PVPlacement(0,              // no rotation
//                                  G4ThreeVector(0,0,2.65*cm), // at (x,y,z)
//                                  logicTopCover,    // its logical volume                                 
//                                  "pTopCover",       // its name
//                                  logicWorld,      // its mother  volume
//                                  false,           // no boolean operations
//                                  0);              // no particular field
//
//
//  // Cover surface
//  G4OpticalSurface * CoverOpSurface = new G4OpticalSurface("CoverOpSurface");    
//  G4LogicalBorderSurface* CoverSurface = new G4LogicalBorderSurface("CoverSurface",
//                                             physiAcrylic, 
//                                             physiCover,
//                                             CoverOpSurface);
//  
//  G4OpticalSurface * TopCoverOpSurface = new G4OpticalSurface("TopCoverOpSurface");     
//  G4LogicalBorderSurface* TopCoverSurface = new G4LogicalBorderSurface("TopCoverSurface",
//                                             physiAcrylic, 
//                                             physiTopCover,
//                                             TopCoverOpSurface);
//
//  CoverOpSurface->SetModel(unified);
//  CoverOpSurface->SetType(dielectric_metal);
//  CoverOpSurface->SetFinish(polished);       

//  G4MaterialPropertiesTable* CoverOpticalMPT  = new G4MaterialPropertiesTable();
//  //CoverOpticalMPT->AddProperty("SPECULARLOBECONSTANT", TeflonEnergy, TeflonSpecularlobe, 4);
//  //CoverOpticalMPT->AddProperty("SPECULARSPIKECONSTANT", TeflonEnergy, TeflonSpecularspike, 4);
//  //CoverOpticalMPT->AddProperty("BACKSCATTERCONSTANT", TeflonEnergy, TeflonBackscatter, 4);
//  //CoverOpticalMPT->AddProperty("REFLECTIVITY", TeflonEnergy, TeflonReflectivity, 4);
//  CoverOpticalMPT->AddProperty("SPECULARLOBECONSTANT", ESREnergy, ESRSpecularlobe, 31);
//  CoverOpticalMPT->AddProperty("SPECULARSPIKECONSTANT", ESREnergy, ESRSpecularspike, 31);
//  CoverOpticalMPT->AddProperty("BACKSCATTERCONSTANT", ESREnergy, ESRBackscatter, 31);
//  CoverOpticalMPT->AddProperty("REFLECTIVITY", ESREnergy, ESRReflectivity, 31);
//
//  CoverOpSurface->SetMaterialPropertiesTable(CoverOpticalMPT);
//
//  TopCoverOpSurface->SetModel(unified);
//  TopCoverOpSurface->SetType(dielectric_metal);
//  TopCoverOpSurface->SetFinish(polished);
//
//  G4MaterialPropertiesTable* TopCoverOpticalMPT  = new G4MaterialPropertiesTable();
//  TopCoverOpticalMPT->AddProperty("REFLECTIVITY", ESREnergy, ESRReflectivity, 31);
//  TopCoverOpticalMPT->AddProperty("SPECULARLOBECONSTANT", ESREnergy, ESRSpecularlobe, 31);
//  TopCoverOpticalMPT->AddProperty("SPECULARSPIKECONSTANT", ESREnergy, ESRSpecularspike, 31);
//  TopCoverOpticalMPT->AddProperty("BACKSCATTERCONSTANT", ESREnergy, ESRBackscatter, 31);
//
//  TopCoverOpSurface->SetMaterialPropertiesTable(TopCoverOpticalMPT);

  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  GdLSExpDetectorSD* aDetectorSD = new GdLSExpDetectorSD("GdLSExpSD");
  SDman->AddNewDetector( aDetectorSD );
  logicTarget->SetSensitiveDetector( aDetectorSD );
  logicPMT->SetSensitiveDetector( aDetectorSD );
  

  //--------- Visualization attributes -------------------------------
  //logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes* TargetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  logicTarget ->SetVisAttributes(TargetVisAtt);
  G4VisAttributes* DetectorVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,.0));
  logicAcrylic->SetVisAttributes(DetectorVisAtt);
  //G4VisAttributes* PoolVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  //logicCover->SetVisAttributes(PoolVisAtt);
  //G4VisAttributes* RockVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  //logicTopCover->SetVisAttributes(RockVisAtt);

  //------------ set the incident position ------

 // get the pointer to the User Interface manager 

  return physiWorld;
}

