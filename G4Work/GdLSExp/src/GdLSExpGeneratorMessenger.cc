///-------------------------------------------------------
///--------------------------------------------------
#include "GdLSExpGeneratorMessenger.hh"
#include "GdLSExpPrimaryGeneratorAction.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"

//-----------------------------------------------------------------------------
GdLSExpGeneratorMessenger::GdLSExpGeneratorMessenger(GdLSExpPrimaryGeneratorAction* generatorAct ):gdlsGenerator(generatorAct) 
{
  //Setup a command directory for detector controls with guidance
  generatorDir = new G4UIdirectory("/gdls/generator/");
  generatorDir->SetGuidance("Generator control");
  
  // the generator command
  pNameCmd = new G4UIcmdWithAString("/gdls/generator/setParticle",this);
  pNameCmd->SetGuidance("Set particle type :");
  pNameCmd->SetDefaultValue("gamma");
  
  energyCmd = new G4UIcmdWithADouble("/gdls/generator/setEnergy",this);
  energyCmd->SetGuidance("Set particle energy :");
  energyCmd->SetDefaultValue(1);
  
}

//-----------------------------------------------------------------------------
GdLSExpGeneratorMessenger::~GdLSExpGeneratorMessenger()
{
 delete pNameCmd;
 delete energyCmd;
 delete generatorDir;
}

//------------------------------------------------------------------------
void GdLSExpGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == pNameCmd) {
    gdlsGenerator->SetParticleName(newValue);
  } 
  else if ( command == energyCmd ) {
    G4double energy = energyCmd->GetNewDoubleValue(newValue);
    gdlsGenerator->SetEnergy(energy);
  } 
}
