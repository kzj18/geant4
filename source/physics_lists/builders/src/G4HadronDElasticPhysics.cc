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
// $Id: G4HadronDElasticPhysics.cc,v 1.7 2010-07-29 10:52:14 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4HadronDElasticPhysics
//
// Author: 11 April 2006 V. Ivanchenko
//
// Modified:
// 05.07.2006 V.Ivanchenko define process by particle name; 
//                         fix problem of initialisation of HP
// 24.07.2006 V.Ivanchenko add G4NeutronHPElasticData 
// 10.08.2006 V.Ivanchenko separate neutrons from other particles
// 17.11.2006 V.Ivanchenko do not redefine G4HadronElastic default parameters
// 19.02.2007 V.Ivanchenko set QModelLowLimit and LowestEnergyLimit to zero
// 19.02.2007 A.Howard set QModelLowLimit and LowestEnergyLimit to zero 
//                     for neutrons
// 06.03.2007 V.Ivanchenko use updated interface to G4UElasticCrossSection
// 03.06.2010 V.Ivanchenko cleanup constructors and ConstructProcess method
//
//----------------------------------------------------------------------------
//
// Diffuse optical model for sampling scattering
// BBG cross sections for p, pi+-
// XS cross sections for n
// LHEP cross sections for other particles

#include "G4HadronDElasticPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4HadronicProcess.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4Neutron.hh"

#include "G4WHadronElasticProcess.hh"
#include "G4HadronElastic.hh"
#include "G4CHIPSElastic.hh"
#include "G4ElasticHadrNucleusHE.hh"
#include "G4AntiNuclElastic.hh"

#include "G4BGGNucleonElasticXS.hh"
#include "G4BGGPionElasticXS.hh"
#include "G4NeutronElasticXS.hh"
#include "G4CHIPSElasticXS.hh"

#include "G4ComponentAntiNuclNuclearXS.hh"  
#include "G4CrossSectionElastic.hh"
#include "G4DiffuseElastic.hh"

G4HadronDElasticPhysics::G4HadronDElasticPhysics(G4int ver)
  : G4VPhysicsConstructor("hElasticDIFFUSE"), verbose(ver), 
    wasActivated(false)
{
  if(verbose > 1) { 
    G4cout << "### G4HadronDElasticPhysics: " << GetPhysicsName() 
	   << G4endl; 
  }
}

G4HadronDElasticPhysics::~G4HadronDElasticPhysics()
{}

void G4HadronDElasticPhysics::ConstructParticle()
{
  // G4cout << "G4HadronDElasticPhysics::ConstructParticle" << G4endl;
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  //  Construct light ions
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();  
}

void G4HadronDElasticPhysics::ConstructProcess()
{
  if(wasActivated) return;
  wasActivated = true;

  G4double elimitAntiNuc = 100*MeV;
  if(verbose > 1) {
    G4cout << "### HadronDElasticPhysics Construct Processes " 
	   << " for anti-neuclei " 
	   << elimitAntiNuc/GeV << " GeV"	   << G4endl;
  }

  G4AntiNuclElastic* anuc = new G4AntiNuclElastic();
  anuc->SetMinEnergy(elimitAntiNuc);
  G4CrossSectionElastic* anucxs = 
    new G4CrossSectionElastic(anuc->GetComponentCrossSection());

  G4HadronElastic* lhep0 = new G4HadronElastic();
  G4HadronElastic* lhep2 = new G4HadronElastic();
  lhep2->SetMaxEnergy(elimitAntiNuc);

  G4DiffuseElastic* model = 0;

  theParticleIterator->reset();
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String pname = particle->GetParticleName();
    if(pname == "anti_lambda"  ||
       pname == "anti_neutron" ||
       pname == "anti_omega-"  || 
       pname == "anti_sigma-"  || 
       pname == "anti_sigma+"  || 
       pname == "anti_xi-"  || 
       pname == "anti_xi0"  || 
       pname == "lambda"    || 
       pname == "omega-"    || 
       pname == "sigma-"    || 
       pname == "sigma+"    || 
       pname == "xi-"       || 
       pname == "alpha"     ||
       pname == "deuteron"  ||
       pname == "triton"    
       ) {
      
      G4WHadronElasticProcess* hel = new G4WHadronElasticProcess();
      hel->RegisterMe(lhep0);
      pmanager->AddDiscreteProcess(hel);
      if(verbose > 1) {
	G4cout << "### HadronDElasticPhysics: " << hel->GetProcessName()
	       << " added for " << particle->GetParticleName() << G4endl;
      }

    } else if(pname == "proton") {   

      G4WHadronElasticProcess* hel = new G4WHadronElasticProcess();
      hel->AddDataSet(new G4BGGNucleonElasticXS(particle));
      //hel->AddDataSet(new G4CHIPSElasticXS());
      model = new G4DiffuseElastic();
      hel->RegisterMe(model);
      pmanager->AddDiscreteProcess(hel);
      if(verbose > 1) {
	G4cout << "### HadronDElasticPhysics: " << hel->GetProcessName()
	       << " added for " << particle->GetParticleName() << G4endl;
      }

    } else if(pname == "neutron") {   

      G4WHadronElasticProcess* hel = new G4WHadronElasticProcess();
      hel->AddDataSet(new G4NeutronElasticXS());
      model = new G4DiffuseElastic();
      hel->RegisterMe(model);
      pmanager->AddDiscreteProcess(hel);
      if(verbose > 1) {
	G4cout << "### HadronDElasticPhysics: " 
	       << hel->GetProcessName()
	       << " added for " << particle->GetParticleName() << G4endl;
      }

    } else if (pname == "pi+" || pname == "pi-") { 

      G4WHadronElasticProcess* hel = new G4WHadronElasticProcess();
      hel->AddDataSet(new G4BGGPionElasticXS(particle));
      model = new G4DiffuseElastic();
      hel->RegisterMe(model);
      pmanager->AddDiscreteProcess(hel);
      if(verbose > 1) {
	G4cout << "### HadronDElasticPhysics: " << hel->GetProcessName()
	       << " added for " << particle->GetParticleName() << G4endl;
      }

    } else if(pname == "kaon-"     || 
	      pname == "kaon+"     || 
	      pname == "kaon0S"    || 
	      pname == "kaon0L" 
	      ) {
      
      G4WHadronElasticProcess* hel = new G4WHadronElasticProcess();
      hel->AddDataSet(new G4CHIPSElasticXS());
      model = new G4DiffuseElastic();
      hel->RegisterMe(model);
      pmanager->AddDiscreteProcess(hel);
      if(verbose > 1) {
	G4cout << "### HadronElasticPhysics: " << hel->GetProcessName()
	       << " added for " << particle->GetParticleName() << G4endl;
      }
    } else if(
       pname == "He3"            ||
       pname == "anti_proton"    || 
       pname == "anti_alpha"     ||
       pname == "anti_deuteron"  ||
       pname == "anti_triton"    ||
       pname == "anti_He3"       ) {

      G4WHadronElasticProcess* hel = new G4WHadronElasticProcess();
      hel->AddDataSet(anucxs);
      hel->RegisterMe(lhep2);
      hel->RegisterMe(anuc);
      pmanager->AddDiscreteProcess(hel);
    }
  }

  //G4double elimit = 1.0*GeV;

  if(verbose > 1) {
    G4cout << "### HadronDElasticPhysics Construct Processes " << G4endl;
  }
}


