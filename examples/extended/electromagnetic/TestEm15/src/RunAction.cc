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
// $Id: RunAction.cc,v 1.1 2006-05-09 14:03:04 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
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
#include "G4EmCalculator.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim,
                     HistoManager* histo)
  : detector(det), primary(prim), ProcCounter(0), histoManager(histo)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  // save Rndm status
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  CLHEP::HepRandom::showEngineStatus();

  ProcCounter = new ProcessesCount;
  totalCount = 0;
  
  truePL = truePL2 = geomPL = geomPL2 = 0.;
  lDispl = lDispl2 = psiPrj = psiPrj2 = 0.;
  tetPrj = tetPrj2 = 0.;
  
  histoManager->book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::CountProcesses(G4String procName)
{
   //does the process  already encounted ?
   size_t nbProc = ProcCounter->size();
   size_t i = 0;
   while ((i<nbProc)&&((*ProcCounter)[i]->GetName()!=procName)) i++;
   if (i == nbProc) ProcCounter->push_back( new OneProcessCount(procName));

   (*ProcCounter)[i]->Count();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  G4int  prec = G4cout.precision(5);
    
  G4Material* material = detector->GetMaterial();
  G4double density = material->GetDensity();
   
  G4ParticleDefinition* particle = 
                            primary->GetParticleGun()->GetParticleDefinition();
  G4String Particle = particle->GetParticleName();    
  G4double energy = primary->GetParticleGun()->GetParticleEnergy();
  G4cout << "\n The run consists of " << NbOfEvents << " "<< Particle << " of "
         << G4BestUnit(energy,"Energy") << " through " 
	 << G4BestUnit(detector->GetBoxSize(),"Length") << " of "
	 << material->GetName() << " (density: " 
	 << G4BestUnit(density,"Volumic Mass") << ")" << G4endl;
  
  //frequency of processes
  G4cout << "\n Process calls frequency --->";
  for (size_t i=0; i< ProcCounter->size();i++) {
     G4String procName = (*ProcCounter)[i]->GetName();
     G4int    count    = (*ProcCounter)[i]->GetCounter(); 
     G4cout << "\t" << procName << " = " << count;
  }
  
  if (totalCount == 0) return;
  
  //compute path length and related quantities
  //
  G4double MeanTPL  = truePL /totalCount;     
  G4double MeanTPL2 = truePL2/totalCount;     
  G4double rmsTPL   = std::sqrt(std::fabs(MeanTPL2 - MeanTPL*MeanTPL));
  
  G4double MeanGPL  = geomPL /totalCount;     
  G4double MeanGPL2 = geomPL2/totalCount;     
  G4double rmsGPL   = std::sqrt(std::fabs(MeanGPL2 - MeanGPL*MeanGPL));
  
  G4double MeanLaD  = lDispl /totalCount;     
  G4double MeanLaD2 = lDispl2/totalCount;     
  G4double rmsLaD   = std::sqrt(std::fabs(MeanLaD2 - MeanLaD*MeanLaD));
  
  G4double MeanPsi  = psiPrj /(totalCount);     
  G4double MeanPsi2 = psiPrj2/(totalCount);     
  G4double rmsPsi   = std::sqrt(std::fabs(MeanPsi2 - MeanPsi*MeanPsi));
  
  G4double MeanTeta  = tetPrj /(2*totalCount);     
  G4double MeanTeta2 = tetPrj2/(2*totalCount);     
  G4double rmsTeta   = std::sqrt(std::fabs(MeanTeta2 - MeanTeta*MeanTeta));
         
  G4cout << "\n\n truePathLength :\t" << G4BestUnit(MeanTPL,"Length")
         << " +- "                    << G4BestUnit( rmsTPL,"Length")
         <<   "\n geomPathLength :\t" << G4BestUnit(MeanGPL,"Length")
         << " +- "                    << G4BestUnit( rmsGPL,"Length")
         <<   "\n lateralDisplac :\t" << G4BestUnit(MeanLaD,"Length")
         << " +- "                    << G4BestUnit( rmsLaD,"Length")
         <<   "\n Psi  plane     :\t" << rmsPsi /mrad << " mrad"
         <<   "  ("                   << rmsPsi /deg  << " deg)"	 
         <<   "\n Teta plane     :\t" << rmsTeta/mrad << " mrad"
         <<   "  ("                   << rmsTeta/deg  << " deg)"	 
         << G4endl;
	 

  //cross check from G4EmCalculator
  //
  G4cout << "\n Verification from G4EmCalculator. \n";
  
  G4EmCalculator emCal;
  
  //get transport mean free path (for multiple scattering)
  G4double MSmfp = emCal.GetMeanFreePath(energy,particle,"msc",material);
    
  //get range from restricted dedx
  G4double range = emCal.GetRangeFromRestricteDEDX(energy,particle,material);
  
  //effective facRange
  G4double efFacrange = MeanTPL/std::max(MSmfp, range);
  if (MeanTPL/range >= 0.99) efFacrange = 1.;

  G4cout << "\n transport mean free path :\t" << G4BestUnit(MSmfp,"Length")
         << "\n range from restrict dE/dx:\t" << G4BestUnit(range,"Length")
	 << "\n ---> effective facRange  :\t" << efFacrange
         << G4endl;

  G4cout << "\n compute teta0 from Highland :\t"
 	 << ComputeMscHighland(MeanTPL)/mrad << " mrad" 
	 << "  (" << ComputeMscHighland(MeanTPL)/deg << " deg)" 
	 << G4endl;
	 	 	 
  //restore default format	 
  G4cout.precision(prec);         

  // delete and remove all contents in ProcCounter 
  while (ProcCounter->size()>0){
    OneProcessCount* aProcCount=ProcCounter->back();
    ProcCounter->pop_back();
    delete aProcCount;
  }
  delete ProcCounter;
  
  histoManager->save();

  // show Rndm status
  CLHEP::HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double RunAction::ComputeMscHighland(G4double pathLength)
{
 //compute the width of the Gaussian central part of the MultipleScattering
 //projected angular distribution.
 //Eur. Phys. Jour. C15 (2000) page 166, formule 23.9

 G4double t = pathLength/(detector->GetMaterial()->GetRadlen());
 if (t < DBL_MIN) return 0.;

 G4ParticleGun* particle = primary->GetParticleGun();
 G4double T = particle->GetParticleEnergy();
 G4double M = particle->GetParticleDefinition()->GetPDGMass();
 G4double z = std::abs(particle->GetParticleDefinition()->GetPDGCharge()/eplus);

 G4double bpc = T*(T+2*M)/(T+M);
 G4double teta0 = 13.6*MeV*z*std::sqrt(t)*(1.+0.038*std::log(t))/bpc;
 return teta0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
