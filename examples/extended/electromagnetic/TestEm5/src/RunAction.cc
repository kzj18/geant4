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
// $Id: RunAction.cc,v 1.6 2004-06-18 09:47:49 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleGun.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin,
                     HistoManager* histo)
:detector(det), primary(kin), histoManager(histo)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //initialisation
  EnergyDeposit  = EnergyDeposit2  = 0.;
  TrakLenCharged = TrakLenCharged2 = 0.;
  TrakLenNeutral = TrakLenNeutral2 = 0.;
  nbStepsCharged = nbStepsCharged2 = 0.;
  nbStepsNeutral = nbStepsNeutral2 = 0.;
  MscProjecTheta = MscProjecTheta2 = 0.;

  nbGamma = nbElect = nbPosit = nTheta = 0;

  Transmit[0] = Transmit[1] = Reflect[0] = Reflect[1] = 0;

  MscHighland = ComputeMscHighland();

  histoManager->book();

  // save Rndm status
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  // compute mean and rms
  //
  G4int TotNbofEvents = aRun->GetNumberOfEvent();
  if (TotNbofEvents == 0) return;

  EnergyDeposit /= TotNbofEvents; EnergyDeposit2 /= TotNbofEvents;
  G4double rmsEdep = EnergyDeposit2 - EnergyDeposit*EnergyDeposit;
  if (rmsEdep>0.) rmsEdep = sqrt(rmsEdep/TotNbofEvents);
  else            rmsEdep = 0.;

  TrakLenCharged /= TotNbofEvents; TrakLenCharged2 /= TotNbofEvents;
  G4double rmsTLCh = TrakLenCharged2 - TrakLenCharged*TrakLenCharged;
  if (rmsTLCh>0.) rmsTLCh = sqrt(rmsTLCh/TotNbofEvents);
  else            rmsTLCh = 0.;

  TrakLenNeutral /= TotNbofEvents; TrakLenNeutral2 /= TotNbofEvents;
  G4double rmsTLNe = TrakLenNeutral2 - TrakLenNeutral*TrakLenNeutral;
  if (rmsTLNe>0.) rmsTLNe = sqrt(rmsTLNe/TotNbofEvents);
  else            rmsTLNe = 0.;

  nbStepsCharged /= TotNbofEvents; nbStepsCharged2 /= TotNbofEvents;
  G4double rmsStCh = nbStepsCharged2 - nbStepsCharged*nbStepsCharged;
  if (rmsStCh>0.) rmsStCh = sqrt(rmsTLCh/TotNbofEvents);
  else            rmsStCh = 0.;

  nbStepsNeutral /= TotNbofEvents; nbStepsNeutral2 /= TotNbofEvents;
  G4double rmsStNe = nbStepsNeutral2 - nbStepsNeutral*nbStepsNeutral;
  if (rmsStNe>0.) rmsStNe = sqrt(rmsTLCh/TotNbofEvents);
  else            rmsStNe = 0.;

  G4double Gamma = (double)nbGamma/TotNbofEvents;
  G4double Elect = (double)nbElect/TotNbofEvents;
  G4double Posit = (double)nbPosit/TotNbofEvents;

  G4double transmit[2];
  transmit[0] = 100.*Transmit[0]/TotNbofEvents;
  transmit[1] = 100.*Transmit[1]/TotNbofEvents;

  G4double reflect[2];
  reflect[0] = 100.*Reflect[0]/TotNbofEvents;
  reflect[1] = 100.*Reflect[1]/TotNbofEvents;

  G4double rmsMsc = 0.;
  if (nTheta > 0)
    rmsMsc = sqrt(MscProjecTheta2/(G4double)nTheta);


  G4cout << "\n ======================== run summary ======================\n";

  G4int prec = G4cout.precision(4);

  G4cout << "\n Number of Events = " << TotNbofEvents << G4endl;

  G4cout << "\n Total energy deposit in absorber per event = "
         << G4BestUnit(EnergyDeposit,"Energy") << " +- "
         << G4BestUnit(rmsEdep,      "Energy") << G4endl;

  G4cout << "\n Total track length (charged) in absorber per event = "
         << G4BestUnit(TrakLenCharged,"Length") << " +- "
         << G4BestUnit(rmsTLCh,       "Length") << G4endl;

  G4cout << " Total track length (neutral) in absorber per event = "
         << G4BestUnit(TrakLenNeutral,"Length") << " +- "
         << G4BestUnit(rmsTLNe,       "Length") << G4endl;

  G4cout << "\n Number of steps (charged) in absorber per event = "
         << nbStepsCharged << " +- " << rmsStCh << G4endl;

  G4cout << " Number of steps (neutral) in absorber per event = "
         << nbStepsNeutral << " +- " << rmsStNe << G4endl;

  G4cout << "\n Number of secondaries per event : Gammas = " << Gamma
         << ";   electrons = " << Elect
  	 << ";   positrons = " << Posit << G4endl;

  G4cout << "\n Number of events with the primary particle transmitted = "
         << transmit[1] << " %" << G4endl;

  G4cout << " Number of events with at least  1 particle transmitted "
         << "(same charge as primary) = " << transmit[0] << " %" << G4endl;

  G4cout << "\n Number of events with the primary particle reflected = "
         << reflect[1] << " %" << G4endl;

  G4cout << " Number of events with at least  1 particle reflected "
         << "(same charge as primary) = " << reflect[0] << " %" << G4endl;

  // compute width of the Gaussian central part of the MultipleScattering
  //
  G4cout << "\n MultipleScattering: rms proj angle of transmit primary particle = "
         << rmsMsc/mrad << " mrad " << G4endl;

  G4cout << " MultipleScattering: computed theta0 (Highland formula)          = "
 	 << MscHighland/mrad << " mrad" << G4endl;

  G4cout.precision(prec);

  histoManager->save();

  // show Rndm status
  HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double RunAction::ComputeMscHighland()
{
  //compute the width of the Gaussian central part of the MultipleScattering
  //projected angular distribution.
  //Eur. Phys. Jour. C15 (2000) page 166, formule 23.9

  G4double t = (detector->GetAbsorberThickness())
              /(detector->GetAbsorberMaterial()->GetRadlen());
  if (t < DBL_MIN) return 0.;

  G4ParticleGun* particle = primary->GetParticleGun();
  G4double T = particle->GetParticleEnergy();
  G4double M = particle->GetParticleDefinition()->GetPDGMass();
  G4double z = abs(particle->GetParticleDefinition()->GetPDGCharge()/eplus);

  G4double bpc = T*(T+2*M)/(T+M);
  G4double teta0 = 13.6*MeV*z*sqrt(t)*(1.+0.038*log(t))/bpc;
  return teta0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
