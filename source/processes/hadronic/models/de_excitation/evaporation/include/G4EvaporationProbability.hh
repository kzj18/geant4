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
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998) 
//
// JMQ & MAC 07/12/2007: New inverse cross sections
//
//J.M. Quesada (Dec 2007-Apr 2008). Rebuilt class. Mayor changes: new inverse cross sections and 
//numerical integration . No Coulomb barrier is needed anymore (implicitely included in inverse cross sections)

#ifndef G4EvaporationProbability_h
#define G4EvaporationProbability_h 1


#include "G4VEmissionProbability.hh"
#include "G4VLevelDensityParameter.hh"
#include "G4EvaporationLevelDensityParameter.hh"


class G4EvaporationProbability : public G4VEmissionProbability
{
public:
  // Only available constructor
  G4EvaporationProbability(const G4int anA, const G4int aZ, const G4double aGamma):
    theA(anA),
    theZ(aZ),
    Gamma(aGamma)
  {
    theEvapLDPptr = new G4EvaporationLevelDensityParameter;
    
  }

  ~G4EvaporationProbability() 
  {
    if (theEvapLDPptr != 0) delete theEvapLDPptr;

  }


	
  G4double GetZ(void) const { return theZ; }
	
  G4double GetA(void) const { return theA;} 

  
  // Default constructor
  G4EvaporationProbability() {}
private:
  // Copy constructor
  G4EvaporationProbability(const G4EvaporationProbability &right);

  const G4EvaporationProbability & operator=(const G4EvaporationProbability &right);
  G4bool operator==(const G4EvaporationProbability &right) const;
  G4bool operator!=(const G4EvaporationProbability &right) const;
  
public:

  G4double EmissionProbability(const G4Fragment & fragment, const G4double anEnergy);

//JMQ: new methods (04/12/07)

  G4double ProbabilityDistributionFunction(const G4double K, const G4Fragment & aFragment);

  G4double CalculateProbability(const G4double MaximalKineticEnergy,const G4Fragment & fragment );

  G4double IntegrateEmissionProbability(const G4double & Low, const G4double & Up, const G4Fragment & aFragment);

  G4double CrossSection(const G4double K, const  G4Fragment & fragment );



  // Data Members

  G4VLevelDensityParameter * theEvapLDPptr;
	
  G4int theA;
  G4int theZ;

//JMQ: (2s+1) mass number of ejectile
  G4double Gamma;

};




#endif
