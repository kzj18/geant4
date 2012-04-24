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
// $Id: RunAction.hh,v 1.9 2009-01-22 17:41:43 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class DetectorConstruction;
class PrimaryGeneratorAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{

public:

    RunAction(DetectorConstruction*, PrimaryGeneratorAction*, HistoManager*);
    virtual ~RunAction();

    void BeginOfRunAction(const G4Run*);
    void   EndOfRunAction(const G4Run*);

    void AddEnergy (G4double edep)
                 {fEnergyDeposit += edep; fEnergyDeposit2 += edep*edep;};

    void AddTrakLenCharg (G4double length)
                 {fTrakLenCharged += length; fTrakLenCharged2 += length*length;};

    void AddTrakLenNeutr (G4double length)
                 {fTrakLenNeutral += length; fTrakLenNeutral2 += length*length;};

    void AddMscProjTheta (G4double theta)
                 {if (std::abs(theta) <= fMscThetaCentral) { fMscEntryCentral++;
		    fMscProjecTheta += theta;  fMscProjecTheta2 += theta*theta;}
		 };

    void CountStepsCharg (G4int nSteps)
                 {fNbStepsCharged += nSteps; fNbStepsCharged2 += nSteps*nSteps;};

    void CountStepsNeutr (G4int nSteps)
                 {fNbStepsNeutral += nSteps; fNbStepsNeutral2 += nSteps*nSteps;};

    void CountParticles (G4ParticleDefinition* part)
                 {     if (part == G4Gamma::Gamma())       fNbGamma++ ;
		  else if (part == G4Electron::Electron()) fNbElect++ ;
		  else if (part == G4Positron::Positron()) fNbPosit++ ; };

    void CountTransmit (G4int flag)
                 {     if (flag == 1)  fTransmit[0]++;
		  else if (flag == 2) {fTransmit[0]++; fTransmit[1]++; }};

    void CountReflect (G4int flag)
                 {     if (flag == 1)  fReflect[0]++;
		  else if (flag == 2) {fReflect[0]++; fReflect[1]++; }};

    G4double ComputeMscHighland();
    
    void AddEnergyLeak (G4double eleak, G4int index)
               { fEnergyLeak[index] += eleak; fEnergyLeak2[index] += eleak*eleak;};

  private:
    G4double fEnergyDeposit,  fEnergyDeposit2;
    G4double fTrakLenCharged, fTrakLenCharged2;
    G4double fTrakLenNeutral, fTrakLenNeutral2;
    G4double fNbStepsCharged, fNbStepsCharged2;
    G4double fNbStepsNeutral, fNbStepsNeutral2;
    G4double fMscProjecTheta, fMscProjecTheta2;
    G4double fMscThetaCentral;
    
    G4int    fNbGamma, fNbElect, fNbPosit;
    G4int    fTransmit[2],   fReflect[2];
    G4int    fMscEntryCentral;
    
    G4double fEnergyLeak[2],  fEnergyLeak2[2];

    DetectorConstruction*   fDetector;
    PrimaryGeneratorAction* fPrimary;
    HistoManager*           fHistoManager;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

