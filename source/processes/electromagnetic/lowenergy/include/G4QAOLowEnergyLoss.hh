// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//      For information related to this code contact:
//      CERN, IT Division, ASD group
//      History: New Implementation
//    
//      ---------- G4QAOLowEnergyLoss physics process -------
//                  by Stephane Chauvie, 21 May 2000 
//
// Modified:
// 23/05/2000 MGP  Made compliant to design
//  
// Class Description:
// Quantal Harmonic Oscillator Model for energy loss
// of slow antiprotons 
// Class Description - End

// ************************************************************
// ------------------------------------------------------------

 
#ifndef G4QAOLowEnergyLoss_hh
#define G4QAOLowEnergyLoss_hh 1

#include "G4VhEnergyLossModel.hh"
#include "globals.hh"

class G4DynamicParticle;
class G4Material;
class G4ParticleDefinition;

class G4QAOLowEnergyLoss : public G4VhEnergyLossModel
{
public: 
  
  G4QAOLowEnergyLoss(); 
  
  ~G4QAOLowEnergyLoss();
  
  virtual G4double LowEnergyLimit() const {return 50*keV;};
  // returns the lower limit for model validity
  
  virtual G4double HighEnergyLimit() const;
  // returns the higher limit for model validity
 
  virtual G4bool IsInCharge(G4double energy, 
			    const G4ParticleDefinition* particleDefinition,
			    const G4Material* material) const;
  // returns true if the model is applyable at that energy for
  // thet particle for that material
  
  virtual G4double EnergyLoss(const G4DynamicParticle* particle,
			      const G4Material* material) const;
  // returns the energy loss via the quantal harmonic
  // oscillator model 
  
private:
  
  // hide assignment operator 
  G4QAOLowEnergyLoss & operator=( G4QAOLowEnergyLoss &right);
  G4QAOLowEnergyLoss( G4QAOLowEnergyLoss&);
  
private:
  
  // get number of shell, energy and oscillator strenghts for material
  G4int GetNumberOfShell(const G4Material* material) const;
  G4double GetShellEnergy(const G4Material* material,G4int nbOfTheShell) const; 
  G4double GetShellStrength(const G4Material* material,G4int nbOfTheShell) const;
  
  // calculate stopping number for L's term
  G4double GetL0(G4double normEnergy) const;
  // terms in Z^2
  G4double GetL1(G4double normEnergy) const;
  // terms in Z^3
  G4double GetL2(G4double normEnergy) const;
  // terms in Z^4
  
private:
  
  //  material at now avaliable for the model
  static const G4String materialAvailable[6];
  
  // number, energy and oscillator strenghts
  // for an harmonic oscillator model of material
  static const G4int nbofShellForMaterial[6];
  static  G4double alShellEnergy[3];
  static  G4double alShellStrength[3];
  static  G4double siShellEnergy[3];
  static  G4double siShellStrength[3];
  static  G4double cuShellEnergy[4];
  static  G4double cuShellStrength[4];
  static  G4double taShellEnergy[6];
  static  G4double taShellStrength[6];
  static  G4double auShellEnergy[6];
  static  G4double auShellStrength[6];
  static  G4double ptShellEnergy[6];
  static  G4double ptShellStrength[6];
  
  G4int numberOfMaterials;

  //  variable for calculation of stopping number of L's term
  static const G4double L0[67][2];
  static const G4double L1[22][2];
  static const G4double L2[14][2];
  
  
}; 

#endif
