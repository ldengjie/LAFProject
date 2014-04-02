#define USE_CUSTOM_CERENKOV
#define USE_CUSTOM_SCINTILLATION

#include "DsPhysConsOptical.h"
#include "DsG4OpRayleigh.h"

#ifdef USE_CUSTOM_CERENKOV
#include "DsG4Cerenkov.h"
#else
#include "G4Cerenkov.hh"
#endif

#ifdef USE_CUSTOM_SCINTILLATION
#include "DsG4Scintillation.h"
#else
#include "G4Scintillation.hh"
#endif

#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
//#include "DsG4OpBoundaryProcess.h"
#include "G4ProcessManager.hh"
#include "G4FastSimulationManagerProcess.hh"

DsPhysConsOptical::DsPhysConsOptical(const G4String& name): G4VPhysicsConstructor(name)
{
    m_cerenMaxPhotonPerStep = 300;

    m_doReemission = true;
    m_doScintAndCeren = true;

    m_useCerenkov=true;
    m_applyWaterQe=true;

    m_useScintillation=true;
    m_useRayleigh=true;
    m_useAbsorption=true;
    m_useFastMu300nsTrick=false;
    m_ScintillationYieldFactor = 1.0;
   
    m_birksConstant1 = 6.5e-3*g/cm2/MeV;
    m_birksConstant2 = 1.5e-6*(g/cm2/MeV)*(g/cm2/MeV);

    m_slowerTime = 190*ns; 
    m_slowerRatio = 0.01;

    m_cerenPhotonScaleWeight = 3.125;
    m_scintPhotonScaleWeight = 3.125;
}

DsPhysConsOptical::~DsPhysConsOptical()
{
}

void DsPhysConsOptical::ConstructParticle()
{
}

void DsPhysConsOptical::ConstructProcess()
{
#ifdef USE_CUSTOM_CERENKOV
    
    DsG4Cerenkov* cerenkov = 0;
    if (m_useCerenkov) {
        cerenkov = new DsG4Cerenkov();
        cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
        cerenkov->SetApplyPreQE(m_cerenPhotonScaleWeight>1.);
        cerenkov->SetPreQE(1./m_cerenPhotonScaleWeight);
        
        // wangzhe   Give user a handle to control it.   
        cerenkov->SetApplyWaterQe(m_applyWaterQe);
        // wz
        cerenkov->SetTrackSecondariesFirst(true);
    }
#else
    
    G4Cerenkov* cerenkov = 0;
    if (m_useCerenkov) {
        cerenkov = new G4Cerenkov();
        cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
        cerenkov->SetTrackSecondariesFirst(true);
    }
#endif

#ifdef USE_CUSTOM_SCINTILLATION
    DsG4Scintillation* scint = 0;
   
    scint = new DsG4Scintillation();
    scint->SetBirksConstant1(m_birksConstant1);
    scint->SetBirksConstant2(m_birksConstant2);
    scint->SetSlowerTimeConstant(m_slowerTime);
    scint->SetSlowerRatio(m_slowerRatio);
    scint->SetDoReemission(m_doReemission);
    scint->SetDoBothProcess(m_doScintAndCeren);
    scint->SetApplyPreQE(m_scintPhotonScaleWeight>1.);
    scint->SetPreQE(1./m_scintPhotonScaleWeight);
    scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); //1.);
    scint->SetUseFastMu300nsTrick(m_useFastMu300nsTrick);
    scint->SetTrackSecondariesFirst(true);
    if (!m_useScintillation) {
        scint->SetNoOp();
    }
#else  // standard G4 scint
    G4Scintillation* scint = 0;
    if (m_useScintillation) {
  
        scint = new G4Scintillation();
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); // 1.);
        scint->SetTrackSecondariesFirst(true);
    }
#endif

    G4OpAbsorption* absorb = 0;
    if (m_useAbsorption) {
        absorb = new G4OpAbsorption();
    }

    DsG4OpRayleigh* rayleigh = 0;
    if (m_useRayleigh) {
        rayleigh = new DsG4OpRayleigh();
	//        rayleigh->SetVerboseLevel(2);
    }

    G4OpBoundaryProcess* boundproc = new G4OpBoundaryProcess();
    //DsG4OpBoundaryProcess* boundproc = new DsG4OpBoundaryProcess();
    boundproc->SetModel(unified);

    G4FastSimulationManagerProcess* fast_sim_man
        = new G4FastSimulationManagerProcess("fast_sim_man");
    
    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {

        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
    
        // Caution: as of G4.9, Cerenkov becomes a Discrete Process.
        // This code assumes a version of G4Cerenkov from before this version.

        if(cerenkov && cerenkov->IsApplicable(*particle)) {
            pmanager->AddProcess(cerenkov);
            pmanager->SetProcessOrdering(cerenkov, idxPostStep);
        }

        if(scint && scint->IsApplicable(*particle)) {
            pmanager->AddProcess(scint);
            pmanager->SetProcessOrderingToLast(scint, idxAtRest);
            pmanager->SetProcessOrderingToLast(scint, idxPostStep);
        }

        if (particle == G4OpticalPhoton::Definition()) {
            if (absorb)
                pmanager->AddDiscreteProcess(absorb);
            if (rayleigh)
                pmanager->AddDiscreteProcess(rayleigh);
            pmanager->AddDiscreteProcess(boundproc);
            //pmanager->AddDiscreteProcess(pee);
            pmanager->AddDiscreteProcess(fast_sim_man);
        }
    }
}
