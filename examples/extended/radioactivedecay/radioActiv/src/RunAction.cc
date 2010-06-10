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
//
// $Id: RunAction.cc,v 1.1 2010-06-10 18:56:24 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "RunAction.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager* histo, PrimaryGeneratorAction* kin)
:histoManager(histo), primary(kin)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  decayCount = 0;
  for (G4int i=0; i<3; i++) Ebalance[i] = Pbalance[i] = 0. ;
       
  //histograms
  //
  histoManager->book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::ParticleCount(G4String name, G4double Ekin)
{
  particleCount[name]++;
  Emean[name] += Ekin;
  if (Ekin < Emin[name]) Emin[name] = Ekin;
  if (Ekin > Emax[name]) Emax[name] = Ekin;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::Balance(G4double Ebal, G4double Pbal)
{
  decayCount++;
  Ebalance[0] += Ebal;
  if (Ebal < Ebalance[1]) Ebalance[1] = Ebal;
  if (Ebal > Ebalance[2]) Ebalance[2] = Ebal;
  
  Pbalance[0] += Pbal;
  if (Pbal < Pbalance[1]) Pbalance[1] = Pbal;
  if (Pbal > Pbalance[2]) Pbalance[2] = Pbal;    
}
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
 G4int nbEvents = run->GetNumberOfEvent();
 if (nbEvents == 0) return;
 
 G4ParticleDefinition* particle = primary->GetParticleGun()
                                         ->GetParticleDefinition();
 G4String partName = particle->GetParticleName();
 G4double eprimary = primary->GetParticleGun()->GetParticleEnergy();
 
 G4cout << "\n ======================== run summary ======================";  
 G4cout << "\n The run was " << nbEvents << " " << partName << " of "
        << G4BestUnit(eprimary,"Energy");
 G4cout << "\n ===========================================================\n";
 G4cout << G4endl;

 G4int prec = 4, wid = prec + 2;
 G4int dfprec = G4cout.precision(prec);
      
 //particle count
 //
 G4cout << " Nb of generated particles: \n" << G4endl;
     
 std::map<G4String,G4int>::iterator it;               
 for (it = particleCount.begin(); it != particleCount.end(); it++) { 
    G4String name = it->first;
    G4int count   = it->second;
    G4double eMean = Emean[name]/count;
    G4double eMin = Emin[name], eMax = Emax[name];    
         
    G4cout << "  " << std::setw(13) << name << ": " << std::setw(7) << count
           << "  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy")
	   << "  Emin = "  << std::setw(wid) << G4BestUnit(eMin,  "Energy")
	   << "  Emax = "  << std::setw(wid) << G4BestUnit(eMax,  "Energy") 
           << G4endl;
 }
 
 //energy momentum balance
 //
 G4double Ebmean = Ebalance[0]/decayCount;
 G4double Pbmean = Pbalance[0]/decayCount;
  
 G4cout << "\n Energy and momentum balance : initial state - final state"
        << "\n (excluding gamma desexcitation) : \n"  
        << G4endl;
         
 G4cout 
 << "  Energy:   mean = " << std::setw(wid) << G4BestUnit(Ebmean, "Energy")
	    << "  min = " << std::setw(wid) << G4BestUnit(Ebalance[1], "Energy")
	    << "  max = " << std::setw(wid) << G4BestUnit(Ebalance[2], "Energy")
            << G4endl;
	   
 G4cout 
 << "  Momentum: mean = " << std::setw(wid) << G4BestUnit(Pbmean, "Energy")
	    << "  min = " << std::setw(wid) << G4BestUnit(Pbalance[1], "Energy")
	    << "  max = " << std::setw(wid) << G4BestUnit(Pbalance[2], "Energy")
            << "\n" << G4endl;
	   	         
 // remove all contents in particleCount
 // 
 particleCount.clear(); 
 Emean.clear();  Emin.clear(); Emax.clear();

 // restore default precision
 // 
 G4cout.precision(dfprec);
            
 //save histograms
 //      
 histoManager->save();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
