
#ifndef GdLSExpGeneratorMessenger_h
#define GdLSExpGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class GdLSExpPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

///-------------------------------------------------------
/// gdlsGenerator messenger. Class for communication for generators
///--------------------------------------------------
class GdLSExpGeneratorMessenger:public G4UImessenger
{
  public:
    GdLSExpGeneratorMessenger(GdLSExpPrimaryGeneratorAction*);
    ~GdLSExpGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
  
  private:
    GdLSExpPrimaryGeneratorAction* gdlsGenerator;
    G4UIdirectory*             generatorDir;

    G4UIcmdWithADouble*   energyCmd;
    G4UIcmdWithAString*   pNameCmd;
};

#endif             
