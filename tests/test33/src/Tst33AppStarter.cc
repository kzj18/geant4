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
//
// $Id: Tst33AppStarter.cc,v 1.2 2002-10-29 16:37:10 dressel Exp $
// GEANT4 tag 
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// Tst33AppStarter.cc
//
// ----------------------------------------------------------------------

#include "Tst33AppStarter.hh"
#include "Tst33PhysicsList.hh"
#include "Tst33PrimaryGeneratorAction.hh"
#include "Tst33DetectorConstruction.hh"
#include "G4UImanager.hh"
#include "G4ParallelGeometrySampler.hh"
#include "G4MassGeometrySampler.hh"
#include "Tst33VGeometry.hh"
#include "Tst33SlobedConcreteShield.hh"
#include "Tst33ConcreteShield.hh"
#include "Tst33ParallelGeometry.hh"
#include "G4CellScorerStore.hh"
#include "G4CellStoreScorer.hh"
#include "Tst33IStoreBuilder.hh"
#include "Tst33ScorerBuilder.hh"
#include "Tst33VEventAction.hh"
#include "Tst33VisRunAction.hh"
#include "Tst33VisEventAction.hh"
#include "Tst33TimedEventAction.hh"
#include "G4VIStore.hh"
#include "G4CellScorer.hh"
#include "G4ScoreTable.hh"
#include "Tst33TimedApplication.hh"
#include "Tst33VisApplication.hh"


Tst33AppStarter::Tst33AppStarter()
  : 
  fMessenger(*this),
  fMassGeometry(0),
  fSampleGeometry(0),
  fParallelGeometry(0),
  fApp(0),
  fDetectorConstruction((new Tst33DetectorConstruction)),
  fSampler(0),
  fScorer(0),
  fCell_19_Scorer(0),
  fIStore(0),
  fConfigured(false),
  fWeightroulette(false),
  fTime(0)
{
  fRunManager.SetUserInitialization(new Tst33PhysicsList); 
  fRunManager.SetUserAction(new Tst33PrimaryGeneratorAction);  
  fRunManager.SetUserInitialization(fDetectorConstruction);
}
Tst33AppStarter::~Tst33AppStarter()
{
  if (fSampler) {
    delete fSampler;
  }
  if (fParallelGeometry) {
    delete fParallelGeometry;
  }
}

void Tst33AppStarter::CreateMassGeometry() {
  if (fMassGeometry) {
    G4std::G4cout << "Tst33AppStarter::CreateMassGeometry: a geometry already exists!" << G4endl;
  } 
  else {
    fMassGeometry = new Tst33SlobedConcreteShield;
    fDetectorConstruction->SetWorldVolume(&fMassGeometry->
					  GetWorldVolume());
    fSampleGeometry = fMassGeometry;
    fSampler = new G4MassGeometrySampler("neutron"); 
  }
}

void Tst33AppStarter::CreateParallelGeometry() {
  if (fMassGeometry) {
    G4std::G4Exception("Tst33AppStarter::CreateParallelGeometry: a eometry already exists!");
  } 
  else {
    fMassGeometry = new Tst33ConcreteShield;
    fDetectorConstruction->SetWorldVolume(&fMassGeometry->
					  GetWorldVolume());
    fParallelGeometry = new Tst33ParallelGeometry;
    fSampleGeometry = fParallelGeometry;
    fSampler = new
      G4ParallelGeometrySampler(fParallelGeometry->GetWorldVolume(),
				"neutron");
  }
}

G4bool Tst33AppStarter::IsGeo_n_App(){
  G4bool geo_n_app = true;
  if (!fMassGeometry) {
    G4std::G4cout << "Tst33AppStarter::IsGeo_n_App: no geometry!" << G4endl;
    geo_n_app = false;
  }
  if (!fApp) {
    G4std::G4cout << "Tst33AppStarter::IsGeo_n_App: no application!" << G4endl;
    geo_n_app = false;
  }
  return geo_n_app;
}

G4bool Tst33AppStarter::CheckCreateScorer() {
  G4bool createscorer = true;
  if (!IsGeo_n_App()) {
    G4std::G4cout << "Tst33AppStarter::CheckCreateScorer: !IsGeo_n_App()!" << G4endl;
    createscorer = false;
  }
  if (fScorer) {
    G4std::G4cout << "Tst33AppStarter::CheckCreateScorer: scorer already exists!" << G4endl;
    createscorer = false;
  }
  return createscorer;
}

void Tst33AppStarter::CreateScorer() {
  if (CheckCreateScorer()) { 
    Tst33ScorerBuilder sb;
    fScorerStore = sb.CreateScorer(fSampleGeometry,
				   &fCell_19_Scorer);
    fScorer = new G4CellStoreScorer(*fScorerStore);
    fSampler->PrepareScoring(fScorer);
    fEventAction->SetCell_19_Scorer(fCell_19_Scorer);
  }
}

void Tst33AppStarter::DeleteScorers() {
  delete fScorer;
  fScorer = 0;
  fScorerStore->DeleteAllScorers();
  fCell_19_Scorer = 0;
}

G4bool Tst33AppStarter::CheckCreateIStore() {
  G4bool createistore = true;
  if (!IsGeo_n_App()) {
    G4std::G4cout << "Tst33AppStarter::CheckCreateIStore: !IsGeo_n_App()!" << G4endl;
    createistore = false;
  }  
  if (fIStore) {
    G4std::G4cout << "Tst33AppStarter::CheckCreateIStore: an importance store already exists!" << G4endl;
    createistore = false;
  }
  return createistore;
}

void Tst33AppStarter::CreateIStore() {
  if (CheckCreateIStore()) {
    Tst33IStoreBuilder ib;
    fIStore = ib.CreateIStore(fSampleGeometry);
    fSampler->PrepareImportanceSampling(fIStore);
  }
}

G4bool Tst33AppStarter::CheckCreateWeightRoulette() {
  G4bool createWR = true;
  if (!IsGeo_n_App()) {
    G4std::G4cout << "Tst33AppStarter::CheckCreateWeightRoulette: !IsGeo_n_App()!" << G4endl;
    createWR = false;
  }  
  if (fWeightroulette) {
    G4std::G4cout << "Tst33AppStarter::CheckCreateWeightRoulette: weigyht roulette already exists!" << G4endl;
    createWR = false;
  }
  return createWR;
}


void Tst33AppStarter::CreateWeightRoulette() {
  if (CheckCreateWeightRoulette()) {
    fWeightroulette = true;
    fSampler->PrepareWeightRoulett();
  }
}


G4bool Tst33AppStarter::CheckCreateApp() {
  G4bool  createapp(true);
  if (!fMassGeometry) {
    G4std::G4cout << "Tst33AppStarter::CheckApp: geometry not defined!" << G4endl;
    createapp = false;
  }
  if (fApp) {
    G4std::G4cout << "Tst33AppStarter::CheckApp: an application alredy exists!" << G4endl;
    createapp = false;
  }   
  return createapp;
}

void Tst33AppStarter::CreateVisApplication(){
  if (CheckCreateApp()) {
    fApp = new Tst33VisApplication;
    fRunAction = fApp->CreateRunAction();
    fRunManager.SetUserAction(fRunAction);
    fEventAction = fApp->CreateEventAction();
    fRunManager.SetUserAction(fEventAction);    
    fRunManager.Initialize();
    G4UImanager::GetUIpointer()->ApplyCommand("/control/execute vis.mac");
  }
}

void Tst33AppStarter::CreateTimedApplication(G4int time) {
  if (CheckCreateApp()) {
    fTime = time;
    fApp = new Tst33TimedApplication(fTime);
    fEventAction = fApp->CreateEventAction();
    fRunManager.SetUserAction(fEventAction);    
    fRunManager.Initialize();
  }
}


void Tst33AppStarter::ClearSampling() {
  if (!fSampler) {
    G4std::G4cout << "Tst33AppStarter::ClearSampling: no sampler!" << G4endl;
  }
  else {
    fSampler->ClearSampling();
    DeleteScorers();
    delete fIStore;
    fIStore = 0;
    fEventAction->Clear();
    fWeightroulette = false;
    fConfigured = false;
  }
}

void Tst33AppStarter::ConfigureSampling(){
  if (!fConfigured) {
    if (!IsGeo_n_App()) {
      G4cout << "ConfigureSampling no geometry or application given!" << G4endl;
    }
    else {
      fConfigured = true;
      fSampler->Configure();
    }
  }
  else {
    G4std::G4cout << "Tst33AppStarter::ConfigureSampling(): already configured!" << G4endl;
  }
}

void Tst33AppStarter::Run(G4int nevents) {
  if (!fConfigured) {
    G4cout << "Not configured yet!" << G4endl;
  } 
  else {
    G4int n = nevents;
    if (fTime >0 ) {
      n = 10000000;
    }
    fRunManager.BeamOn(n);
  }	
}

void Tst33AppStarter::PostRun() {
  if (fConfigured) {
    if (fScorerStore) {
      G4ScoreTable sp(fIStore);
      sp.Print(fScorerStore->GetMapGeometryCellCellScorer(), &G4std::G4cout);
    }
  }
  else {
    G4std::G4cout << "Tst33AppStarter::PostRun: not configured!" << G4endl;
  }
}







