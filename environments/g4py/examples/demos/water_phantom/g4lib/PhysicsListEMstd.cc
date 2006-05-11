// $Id: PhysicsListEMstd.cc,v 1.1 2006-05-11 04:35:32 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   PhysicsListEMstd.cc
//
//   Physics list for electron/positron/gamma
//   EM-standard package w/ default parameters
//
// ====================================================================
#include "PhysicsListEMstd.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4MultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

// ====================================================================
//
// class description
//
// ====================================================================

////////////////////////////////////
PhysicsListEMstd::PhysicsListEMstd()
  : G4VPhysicsConstructor("EM-std")
////////////////////////////////////
{
}


/////////////////////////////////////
PhysicsListEMstd::~PhysicsListEMstd()
/////////////////////////////////////
{
}


//////////////////////////////////////////
void PhysicsListEMstd::ConstructParticle()
//////////////////////////////////////////
{
}


/////////////////////////////////////////
void PhysicsListEMstd::ConstructProcess()
/////////////////////////////////////////
{
  G4ProcessManager* pm;

  // ---------------------------------------------------------- 
  // gamma physics
  // ----------------------------------------------------------
  pm= G4Gamma::Gamma()-> GetProcessManager();
  pm-> AddDiscreteProcess(new G4PhotoElectricEffect);
  pm-> AddDiscreteProcess(new G4ComptonScattering);
  pm-> AddDiscreteProcess(new G4GammaConversion);

  // ----------------------------------------------------------
  // electron physics
  // ----------------------------------------------------------
  G4MultipleScattering* msc=    new G4MultipleScattering;
  G4eIonisation*        eion=   new G4eIonisation;
  G4eBremsstrahlung*    ebrems= new G4eBremsstrahlung;

  pm= G4Electron::Electron()->GetProcessManager();
  pm-> AddProcess(msc,    ordInActive,           1, 1);
  pm-> AddProcess(eion,   ordInActive,           2, 2);
  pm-> AddProcess(ebrems, ordInActive, ordInActive, 3);

  // ----------------------------------------------------------
  // positron physics
  // ----------------------------------------------------------
  msc=    new G4MultipleScattering;
  eion=   new G4eIonisation;
  ebrems= new G4eBremsstrahlung;
  G4eplusAnnihilation* annihilation= new G4eplusAnnihilation;

  pm= G4Positron::Positron()-> GetProcessManager();
  pm-> AddProcess(msc,          ordInActive, 1,           1);
  pm-> AddProcess(eion,         ordInActive, 2,           2);
  pm-> AddProcess(ebrems,       ordInActive, ordInActive, 3);
  pm-> AddProcess(annihilation, 0,           ordInActive, 4);

}

