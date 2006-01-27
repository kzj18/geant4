//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id: Tst50AntiProtonZiegler2000.cc,v 1.1 2006-01-27 11:14:57 chauvie Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Author: Stephane Chauvie (chauvie@to.infn.it)
//
// History:
// -----------
// 22 Feb 2006         SC      Created
//
// -------------------------------------------------------------------

#include "Tst50AntiProtonZiegler2000.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4MultipleScattering.hh"
#include "G4AntiProton.hh"
#include "G4hLowEnergyIonisation.hh"
#include "G4hLowEnergyLoss.hh"
#include "G4hSRIM2000p.hh" 
#include "G4StepLimiter.hh"

Tst50AntiProtonZiegler2000::Tst50AntiProtonZiegler2000(const G4String& name): G4VPhysicsConstructor(name)
{ }

Tst50AntiProtonZiegler2000::~Tst50AntiProtonZiegler2000()
{ }

void Tst50AntiProtonZiegler2000::ConstructProcess()
{

  theParticleIterator->reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* manager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
     
      if (particleName == "anti_proton") 
	{
	  // G4VProcess*  multipleScattering= new G4MultipleScattering(); 
	  G4hLowEnergyIonisation* ion = new G4hLowEnergyIonisation();
	  
	  ion -> SetElectronicStoppingPowerModel(particle, "SRIM2000p");
	  // ion -> SetNuclearStoppingPowerModel("Ziegler1985");
          ion -> SetNuclearStoppingOn() ;
          ion -> SetBarkasOn() ;
	  //  manager->AddProcess(multipleScattering,-1,1,1);  	
	  ion -> SetEnlossFluc(false);
          manager -> AddProcess(new G4StepLimiter(),-1,-1,3);
          manager-> AddProcess(ion,-1,2,2);
	}
    }
}
