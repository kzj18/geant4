///////////////////////////////////////////////////////////////////////////////
// File: CCalEndOfEventAction.cc
// Description: CCalEndOfEventAction provides User actions at end of event
///////////////////////////////////////////////////////////////////////////////
#include "CCalEndOfEventAction.hh"
#include "CCalAnalysis.hh"
#include "CCaloSD.hh"
#include "CCalPrimaryGeneratorAction.hh"
#include "CCalG4HitCollection.hh"
#include "CCalG4Hit.hh"
#include "CCaloOrganization.hh"
#include "CCalSDList.hh"
#include "CCalSteppingAction.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include <iostream>
#include <map>
#include "G4RunManager.hh"
#include "G4UserSteppingAction.hh"

#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"

//#define debug
//#define ddebug


CCalEndOfEventAction::CCalEndOfEventAction (CCalPrimaryGeneratorAction* pg): 
  isInitialized(false),SDnames(0),numberOfSD(0) {

  primaryGenerator = pg;
#ifdef debug
  cout << "Instantiate CCalEndOfEventAction" << endl;
#endif

  cout << "Now Instantiate stepping action" << endl;
  instanciateSteppingAction();
  
  cout << "Get Calorimter organisation" << endl;
  theOrg = new CCaloOrganization;
  cout << "end of instantiation of EndofEventAction" << endl;
}


CCalEndOfEventAction::~CCalEndOfEventAction() {

  if (theOrg)
    delete theOrg;
  if (SDnames)
    delete SDnames;
  cout << "Deleting CCalEndOfEventAction" << endl;
}


void CCalEndOfEventAction::initialize() {

  isInitialized = true;
  numberOfSD = CCalSDList::getInstance()->getNumberOfCaloSD();
#ifdef debug
  cout << "CCalEndOfEventAction look for " << numberOfSD 
       << " calorimeter-like SD" << endl;
#endif
  if (numberOfSD > 0)
    SDnames = new nameType[numberOfSD];
  for (int i=0; i<numberOfSD; i++) {
    SDnames[i] = G4String(CCalSDList::getInstance()->getCaloSDName(i));
#ifdef debug
    cout << "CCalEndOfEventAction: found SD " << i << " name "
	 << SDnames[i] << endl;
#endif
  }       
}


void CCalEndOfEventAction::StartOfEventAction(const G4Event* evt) { 
  G4cout << "\n---> Begin of event: " << evt->GetEventID() << G4endl;
}


void CCalEndOfEventAction::EndOfEventAction(const G4Event* evt){

#ifdef debug
  cout << endl << "=== Begin of EndOfEventAction === " << endl;
#endif

  if (!isInitialized) initialize();

  theSteppingAction->endOfEvent();
  
  //
  // Look for the Hit Collection 
  //  
  G4HCofThisEvent* allHC = evt->GetHCofThisEvent();
  if (allHC == 0) {
#ifdef debug
    cout << "CCalEndOfEventAction: No Hit Collection in this event" 
	 << endl;
#endif
    return;
  }
  	
  //
  // hits info
  //
  
  //Now make summary
  float hcalE[28], ecalE[49], fullE=0.;
  int i = 0;
  for (i = 0; i < 28; i++) {hcalE[i]=0.;}
  for (i = 0; i < 49; i++) {ecalE[i]=0.;}

  float* edep = new float[numberOfSD];
  int nhit=0;
  for (i = 0; i < numberOfSD; i++){

    //
    // Look for the Hit Collection
    //
    edep[i] = 0;
    int caloHCid = G4SDManager::GetSDMpointer()->GetCollectionID(SDnames[i]);

    CCalG4HitCollection* theHC = 
      (CCalG4HitCollection*) allHC->GetHC(caloHCid);
    if (theHC != 0) {

      G4int nentries = theHC->entries();
#ifdef debug
      cout << " There are " << nentries << " hits in " << SDnames[i] << " :" 
	   << endl;
#endif

      if (nentries > 0) {
  
	int j;
	for (j=0; j<nentries; j++){
#ifdef ddebug
	  cout << "Considering hit " << j << endl;
#endif
	  CCalG4Hit* aHit =  (*theHC)[j];
	  float En = aHit->getEnergyDeposit();
	  int unitID = aHit->getUnitID();
	  int id=-1;
	  if (unitID > 0 && unitID < 29) {
	    id = unitID - 1; // HCal
	    hcalE[id] += En/GeV;
	  } else {
	    int i0 = unitID/4096;
	    int i1 = (unitID/64)%64;
	    int i2 = unitID%64;
	    if (i0 == 1 && i1 < 8 && i2 < 8) {
	      id = i1*7 + i2; // ECal
	      ecalE[id] += En/GeV;
	    }
	  }
#ifdef ddebug
	  cout << "Seeing Energy = " << En/MeV << " MeV in Unit " << unitID 
	       << " " << id << endl;
#endif
	  fullE   += En/GeV;
	  edep[i] += En/GeV;
	  nhit++;
	}
#ifdef ddebug
	cout << " ===> Total Energy Deposit in this Calorimeter = " 
	     << edep[i]*1000.0 << "  MeV " << endl; 
#endif
      }
    }
  }

  G4ThreeVector pos = primaryGenerator->GetParticlePosition();
  float ener = primaryGenerator->GetParticleEnergy()/GeV;
  float x    = pos.x()/mm;
  float y    = pos.y()/mm;
  float z    = pos.z()/mm;
  float edec = edep[1];
  float edhc = edep[0];
  delete[] edep;

  CCalAnalysis* analysis = CCalAnalysis::getInstance();
  analysis->InsertEnergy(fullE);
  analysis->InsertEnergyHcal(hcalE);
  analysis->InsertEnergyEcal(ecalE);
  analysis->setNtuple(hcalE, ecalE, ener, x, y, z, fullE, edec, edhc);
  analysis->EndOfEvent(nhit);

  //A.R. Add to visualize tracks
  // extract the trajectories and draw them
  if (G4VVisManager::GetConcreteInstance()) {
    G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
    for (G4int i=0; i<n_trajectories; i++) { 
      G4Trajectory* trj = (G4Trajectory*) ((*(evt->GetTrajectoryContainer()))[i]);
      trj->DrawTrajectory(50); // Draw all tracks
      // if ( trj->GetCharge() != 0. ) trj->DrawTrajectory(50); // Draw only charged tracks
      // if ( trj->GetCharge() == 0. ) trj->DrawTrajectory(50); // Draw only neutral tracks
    }
  }

}


void CCalEndOfEventAction::instanciateSteppingAction(){
	
  G4UserSteppingAction* theUA = const_cast<G4UserSteppingAction*>(G4RunManager::GetRunManager()->GetUserSteppingAction());
        
  if (theUA == 0) {
#ifdef debug
    cout << " CCalEndOfEventAction::instanciateSteppingAction creates"
	 << " CCalSteppingAction" << endl;
#endif
    theSteppingAction = new CCalSteppingAction;  
    G4RunManager::GetRunManager()->SetUserAction(theSteppingAction);
  }   
	
}
