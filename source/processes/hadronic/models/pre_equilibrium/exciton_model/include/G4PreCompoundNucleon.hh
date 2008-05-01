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
// by V. Lara
//
//J. M. Quesada (Apr. 2008) . Explicit inclusion of Coulomb barrier has been removed.(NOW implicitely included through cross sections). Fixed bugs . New cross sections and combinatorial factor.


#ifndef G4PreCompoundNucleon_h
#define G4PreCompoundNucleon_h 1

#include "G4PreCompoundFragment.hh"

class G4PreCompoundNucleon : public G4PreCompoundFragment
{
private:
  // default constructor
  G4PreCompoundNucleon() {};

public:
  
  // copy constructor
  G4PreCompoundNucleon(const G4PreCompoundNucleon &right): 
    G4PreCompoundFragment(right) {}

  // constructor  
  G4PreCompoundNucleon(const G4double anA, 
		       const G4double aZ, 		       
		       const G4String & aName): 
    G4PreCompoundFragment(anA,aZ,aName) {}
  
  virtual ~G4PreCompoundNucleon() {}

  // operators  
  const G4PreCompoundNucleon & 
  operator=(const G4PreCompoundNucleon &right) 
  {
    if (&right != this) this->G4PreCompoundFragment::operator=(right);
    return *this;
  }
  
  G4bool operator==(const G4PreCompoundNucleon &right) const 
  { 
    return G4PreCompoundFragment::operator==(right);
  }
    
  G4bool operator!=(const G4PreCompoundNucleon &right) const 
  { 
    return G4PreCompoundFragment::operator!=(right);
  }
    
  virtual G4double ProbabilityDistributionFunction(const G4double eKin,
						   const G4Fragment& aFragment);
  virtual G4double CrossSection(const G4double ekin) ; 
    
protected:

  virtual G4bool IsItPossible(const G4Fragment&) = 0;   

  virtual G4double GetRj(const G4int NumberParticles, const G4int NumberCharged) = 0;  
};

#endif
