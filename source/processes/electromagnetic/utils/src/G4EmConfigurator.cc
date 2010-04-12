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
// $Id: G4EmConfigurator.cc,v 1.7 2010-04-12 11:44:40 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class 
//
// File name:     G4EmConfigurator
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 14.07.2008
//
// Modifications:
//
// Class Description:
//
// This class provides configuration EM models for 
// particles/processes/regions
//

// -------------------------------------------------------------------
//

#include "G4EmConfigurator.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4VProcess.hh"
#include "G4ProcessVector.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4DummyModel.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VEmProcess.hh"
#include "G4VMultipleScattering.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4EmConfigurator::G4EmConfigurator(G4int val):verbose(val)
{
  index = -10;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4EmConfigurator::~G4EmConfigurator()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EmConfigurator::SetExtraEmModel(const G4String& particleName,
				       const G4String& processName,
				       G4VEmModel* mod,
				       const G4String& regionName,
				       G4double emin,
				       G4double emax,
				       G4VEmFluctuationModel* fm)
{
  if(mod || fm) {
    models.push_back(mod);
    flucModels.push_back(fm);
  } else {
    models.push_back(new G4DummyModel());
    flucModels.push_back(0);
  }

  particles.push_back(particleName);
  processes.push_back(processName);
  regions.push_back(regionName);
  lowEnergy.push_back(emin);
  highEnergy.push_back(emax);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EmConfigurator::AddModels()
{
  size_t n = models.size();
  if(n > 0) {
    if(0 < verbose) {
      G4cout << " G4EmConfigurator::AddModels n= " << n << G4endl;
    }
    for(size_t i=0; i<n; ++i) {
      if(models[i]) {
        G4Region* reg = FindRegion(regions[i]);
        if(reg) {
	  --index;
	  SetModelForRegion(models[i],flucModels[i],reg,
			    particles[i],processes[i],
			    lowEnergy[i],highEnergy[i]);
	}
      }
    }
  }
  Clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EmConfigurator::SetModelForRegion(G4VEmModel* mod,
					 G4VEmFluctuationModel* fm,
                                         G4Region* reg,
					 const G4String& particleName,
					 const G4String& processName,
					 G4double emin, G4double emax)
{
  //  G4cout << " G4EmConfigurator::SetModelForRegion" << G4endl;
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = 
    G4ParticleTable::GetParticleTable()->GetIterator(); 

  theParticleIterator->reset();
  while( (*theParticleIterator)() ) {
    const G4ParticleDefinition* part = theParticleIterator->value();

    //G4cout << particleName << " " << part->GetParticleName() << G4endl;

    if((part->GetParticleName() == particleName) ||
       (particleName == "all") ||
       (particleName == "charged" && part->GetPDGCharge() != 0.0)) {

      // search for process
      G4ProcessManager* pmanager = part->GetProcessManager();
      G4ProcessVector* plist = pmanager->GetProcessList();
      G4int np = pmanager->GetProcessListLength();
  
      //G4cout << processName << " in list of " << np << G4endl;

      G4VProcess* proc = 0;
      for(G4int i=0; i<np; i++) {
	if(processName == (*plist)[i]->GetProcessName()) {
	  proc = (*plist)[i];
	  break;
	}
      }
      if(!proc) {
	G4cout << "### G4EmConfigurator WARNING: fails to find a process <"
	       << processName << "> for " << particleName << G4endl;
        return;	

      } 

      if(mod) {
	if(!UpdateModelEnergyRange(mod, emin,emax)) { return; }
      }
      // classify process
      G4int ii = proc->GetProcessSubType();
      if(10 == ii && mod) {
	G4VMultipleScattering* p = static_cast<G4VMultipleScattering*>(proc);
	p->AddEmModel(index,mod,reg);
	if(1 < verbose) {
	  G4cout << "### Added msc model order= " << index << " for " 
		 << particleName << " and " << processName << G4endl;
	}
	return;
      } else if(2 <= ii && 4 >= ii) {
	G4VEnergyLossProcess* p = static_cast<G4VEnergyLossProcess*>(proc);
        if(!mod && fm) {
	  p->SetFluctModel(fm);
	} else { 
	  p->AddEmModel(index,mod,fm,reg);
	  if(1 < verbose) {
	    G4cout << "### Added eloss model order= " << index << " for " 
		   << particleName << " and " << processName << G4endl;
	  }
	}
	return;
      } else if(mod) {
	G4VEmProcess* p = static_cast<G4VEmProcess*>(proc);
	p->AddEmModel(index,mod,reg);
	if(1 < verbose) {
	  G4cout << "### Added em model order= " << index << " for " 
		 << particleName << " and " << processName << G4endl;
	}
	return;
      } else {
	return;
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void 
G4EmConfigurator::PrepareModels(const G4ParticleDefinition* aParticle,
				G4VEnergyLossProcess* p)
{
  size_t n = particles.size();
  if(n > 0) {
    if(1 < verbose) {
      G4cout << " G4EmConfigurator::PrepareModels for EnegryLoss n= " 
	     << n << G4endl;
    }
    G4String particleName = aParticle->GetParticleName(); 
    G4String processName  = p->GetProcessName(); 
    for(size_t i=0; i<n; ++i) {
      if(processName == processes[i]) {
        if((particleName == particles[i]) ||
	   (particles[i] == "all") ||
	   (particles[i] == "charged" && aParticle->GetPDGCharge() != 0.0)) {
	  G4Region* reg = FindRegion(regions[i]);
	  if(reg) {
	    --index;
	    G4VEmModel* mod = models[i];
	    G4VEmFluctuationModel* fm = flucModels[i];
	    if(mod) {
	      if(!UpdateModelEnergyRange(mod, lowEnergy[i], highEnergy[i])) 
		{ return; }
	    }
	    if(!mod && fm) {
	      p->SetFluctModel(fm);
	    } else { 
	      p->AddEmModel(index,mod,fm,reg);
	      if(1 < verbose) {
		G4cout << "### Added eloss model order= " << index << " for " 
		       << particleName << " and " << processName << G4endl;
	      }
	    }
	    return;
	  }
	}
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void 
G4EmConfigurator::PrepareModels(const G4ParticleDefinition* aParticle,
				G4VEmProcess* p)
{
  size_t n = particles.size();
  if(n > 0) {
    if(1 < verbose) {
      G4cout << " G4EmConfigurator::PrepareModels for EM process n= " 
	     << n << G4endl;
    }
    G4String particleName = aParticle->GetParticleName(); 
    G4String processName  = p->GetProcessName(); 
    for(size_t i=0; i<n; ++i) {
      if(processName == processes[i]) {
        if((particleName == particles[i]) ||
	   (particles[i] == "all") ||
	   (particles[i] == "charged" && aParticle->GetPDGCharge() != 0.0)) {
	  G4Region* reg = FindRegion(regions[i]);
	  if(reg) {
	    --index;
	    G4VEmModel* mod = models[i];
	    if(mod) {
	      if(!UpdateModelEnergyRange(mod, lowEnergy[i], highEnergy[i])) 
		{ return; }
	      p->AddEmModel(index,mod,reg);
	      if(1 < verbose) {
		G4cout << "### Added em model order= " << index << " for " 
		       << particleName << " and " << processName << G4endl;
	      }
	    }
	    return;
	  }
	}
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void 
G4EmConfigurator::PrepareModels(const G4ParticleDefinition* aParticle,
				G4VMultipleScattering* p)
{
  size_t n = particles.size();
  if(n > 0) {
    if(1 < verbose) {
      G4cout << " G4EmConfigurator::PrepareModels for MSC process n= " 
	     << n << G4endl;
    }
    G4String particleName = aParticle->GetParticleName(); 
    G4String processName  = p->GetProcessName(); 
    for(size_t i=0; i<n; ++i) {
      if(processName == processes[i]) {
        if((particleName == particles[i]) ||
	   (particles[i] == "all") ||
	   (particles[i] == "charged" && aParticle->GetPDGCharge() != 0.0)) {
	  G4Region* reg = FindRegion(regions[i]);
	  if(reg) {
	    --index;
	    G4VEmModel* mod = models[i];
	    if(mod) {
	      if(!UpdateModelEnergyRange(mod, lowEnergy[i], highEnergy[i])) 
		{ return; }
	      p->AddEmModel(index,mod,reg);
	      G4cout << "### Added msc model order= " << index << " for " 
		     << particleName << " and " << processName << G4endl;
	    }
	    return;
	  }
	}
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4EmConfigurator::Clear()
{
  particles.clear();
  processes.clear();
  models.clear();
  flucModels.clear();
  regions.clear();
  lowEnergy.clear();
  highEnergy.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Region* G4EmConfigurator::FindRegion(const G4String& regionName)
{
  // search for region
  G4Region* reg = 0;
  G4RegionStore* regStore = G4RegionStore::GetInstance();
  G4String r = regionName;
  if(r == "" || r == "world" || r == "World") { 
    r = "DefaultRegionForTheWorld";
  }
  reg = regStore->GetRegion(r, true); 
  if(!reg) {
    G4cout << "### G4EmConfigurator WARNING: fails to find a region <"
	   << r << G4endl;
  } else if(verbose > 1) {
    G4cout << "### G4EmConfigurator finds out G4Region <" << r << ">" 
	   << G4endl;
  } 
  return reg;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4EmConfigurator::UpdateModelEnergyRange(G4VEmModel* mod, 
						G4double emin, G4double emax)
{
  // energy limits
  G4double e1 = std::max(emin,mod->LowEnergyLimit());
  G4double e2 = std::min(emax,mod->HighEnergyLimit());
  if(e2 <= e1) {
    G4cout << "### G4EmConfigurator WARNING: empty energy interval"
	   << " for <" << mod->GetName() 
	   << ">  Emin(MeV)= " << e1/CLHEP::MeV 
	   << ">  Emax(MeV)= " << e2/CLHEP::MeV 
	   << G4endl;
    return false;	
  }
  mod->SetLowEnergyLimit(e1);
  mod->SetHighEnergyLimit(e2);
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
