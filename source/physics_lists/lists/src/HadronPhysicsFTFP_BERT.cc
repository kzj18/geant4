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
// $Id: HadronPhysicsFTFP_BERT.cc,v 1.4 2010-06-15 11:03:50 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   
//
// Author: 2007 Gunter Folger
//   created from HadronPhysicsFTFP
//
// Modified:
//
//----------------------------------------------------------------------------
//
#include <iomanip>   

#include "HadronPhysicsFTFP_BERT.hh"

#include "globals.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ChipsKaonMinusInelasticXS.hh"
#include "G4ChipsKaonPlusInelasticXS.hh"
#include "G4ChipsKaonZeroInelasticXS.hh"
#include "G4CrossSectionDataSetRegistry.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(HadronPhysicsFTFP_BERT);

HadronPhysicsFTFP_BERT::HadronPhysicsFTFP_BERT(G4int)
    :  G4VPhysicsConstructor("hInelastic FTFP_BERT")
    , theNeutrons(0)
    , theBertiniNeutron(0)
    , theFTFPNeutron(0)
    , theLEPNeutron(0)
    , thePiK(0)
    , theBertiniPiK(0)
    , theFTFPPiK(0)
    , thePro(0)
    , theBertiniPro(0)
    , theFTFPPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , QuasiElastic(false)
    , ChipsKaonMinus(0)
    , ChipsKaonPlus(0)
    , ChipsKaonZero(0)
{}

HadronPhysicsFTFP_BERT::HadronPhysicsFTFP_BERT(const G4String& name, G4bool quasiElastic)
    :  G4VPhysicsConstructor(name) 
    , theNeutrons(0)
    , theBertiniNeutron(0)
    , theFTFPNeutron(0)
    , theLEPNeutron(0)
    , thePiK(0)
    , theBertiniPiK(0)
    , theFTFPPiK(0)
    , thePro(0)
    , theBertiniPro(0)
    , theFTFPPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , QuasiElastic(quasiElastic)
    , ChipsKaonMinus(0)
    , ChipsKaonPlus(0)
    , ChipsKaonZero(0)
{}

void HadronPhysicsFTFP_BERT::CreateModels()
{

  theNeutrons=new G4NeutronBuilder;
  theFTFPNeutron=new G4FTFPNeutronBuilder(QuasiElastic);
  theNeutrons->RegisterMe(theFTFPNeutron);
  theNeutrons->RegisterMe(theBertiniNeutron=new G4BertiniNeutronBuilder);
  theBertiniNeutron->SetMinEnergy(0.0*GeV);
  theBertiniNeutron->SetMaxEnergy(5*GeV);
  theNeutrons->RegisterMe(theLEPNeutron=new G4LEPNeutronBuilder);
  theLEPNeutron->SetMinInelasticEnergy(0.0*eV);   // no inelastic from LEP
  theLEPNeutron->SetMaxInelasticEnergy(0.0*eV);  

  thePro=new G4ProtonBuilder;
  theFTFPPro=new G4FTFPProtonBuilder(QuasiElastic);
  thePro->RegisterMe(theFTFPPro);
  thePro->RegisterMe(theBertiniPro=new G4BertiniProtonBuilder);
  theBertiniPro->SetMaxEnergy(5*GeV);

  thePiK=new G4PiKBuilder;
  theFTFPPiK=new G4FTFPPiKBuilder(QuasiElastic);
  thePiK->RegisterMe(theFTFPPiK);
  thePiK->RegisterMe(theBertiniPiK=new G4BertiniPiKBuilder);
  theBertiniPiK->SetMaxEnergy(5*GeV);
  
  theHyperon=new G4HyperonFTFPBuilder;
    
  theAntiBaryon=new G4AntiBarionBuilder;
  theAntiBaryon->RegisterMe(theFTFPAntiBaryon=new  G4FTFPAntiBarionBuilder(QuasiElastic));
}

HadronPhysicsFTFP_BERT::~HadronPhysicsFTFP_BERT()
{
  delete theNeutrons;
  delete theBertiniNeutron;
  delete theFTFPNeutron;
  delete theLEPNeutron;    

  delete thePiK;
  delete theBertiniPiK;
  delete theFTFPPiK;
    
  delete thePro;
  delete theBertiniPro;
  delete theFTFPPro;    
    
  delete theHyperon;
  delete theAntiBaryon;
  delete theFTFPAntiBaryon;
}

void HadronPhysicsFTFP_BERT::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"
void HadronPhysicsFTFP_BERT::ConstructProcess()
{
  CreateModels();
  theNeutrons->Build();
  thePro->Build();
  thePiK->Build();

  // use CHIPS cross sections also for Kaons
  ChipsKaonMinus = G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonMinusInelasticXS::Default_Name());
  ChipsKaonPlus = G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonPlusInelasticXS::Default_Name());
  ChipsKaonZero = G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonZeroInelasticXS::Default_Name());
  //
    
  FindInelasticProcess(G4KaonMinus::KaonMinus())->AddDataSet(ChipsKaonMinus);
  FindInelasticProcess(G4KaonPlus::KaonPlus())->AddDataSet(ChipsKaonPlus);
  FindInelasticProcess(G4KaonZeroShort::KaonZeroShort())->AddDataSet(ChipsKaonZero );
  FindInelasticProcess(G4KaonZeroLong::KaonZeroLong())->AddDataSet(ChipsKaonZero );
    
    
  theHyperon->Build();
  theAntiBaryon->Build();
}
G4HadronicProcess* 
HadronPhysicsFTFP_BERT::FindInelasticProcess(const G4ParticleDefinition* p)
{
  G4HadronicProcess* had = 0;
  if(p) {
     G4ProcessVector*  pvec = p->GetProcessManager()->GetProcessList();
     size_t n = pvec->size();
     if(0 < n) {
       for(size_t i=0; i<n; ++i) {
	 if(fHadronInelastic == ((*pvec)[i])->GetProcessSubType()) {
	   had = static_cast<G4HadronicProcess*>((*pvec)[i]);
	   break;
	 }
       }
     }
  }
  return had;
}

