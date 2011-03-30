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
#ifndef G4IonsKoxCrossSection_h
#define G4IonsKoxCrossSection_h
//
// Class Description
// Implementation of Kox formulas 
// Kox et al. Phys. Rev. C 35 1678 (1987); 
// Total Reaction Cross Section for Nucleus-nucles reactions.
//
// Class Description - End
// 18-Sep-2003 First version is written by T. Koi
// 12-Nov-2003 Set upper limit at 10 GeV/n
// 12-Nov-2003 Insted of the lower limit, 
//             0 is returned to a partilce with energy lowae than 10 MeV/n 

#include "globals.hh"
#include "G4Proton.hh"

#include "G4VCrossSectionDataSet.hh"

class G4IonsKoxCrossSection : public G4VCrossSectionDataSet
{
public:

  G4IonsKoxCrossSection();

  ~G4IonsKoxCrossSection();

  virtual
  G4bool IsApplicable(const G4DynamicParticle* aDP, const G4Element*);

  virtual
  G4bool IsIsoApplicable(const G4DynamicParticle* aDP,
			 G4int /*ZZ*/, G4int /*AA*/); 

  virtual
  G4double GetCrossSection(const G4DynamicParticle*, 
			   const G4Element*, G4double aTemperature);


  virtual
  G4double GetZandACrossSection(const G4DynamicParticle*, G4int ZZ,
				G4int AA, G4double aTemperature);

  virtual
  void BuildPhysicsTable(const G4ParticleDefinition&);

  virtual
  void DumpPhysicsTable(const G4ParticleDefinition&);

private:
  const G4double upperLimit; 
  const G4double lowerLimit; 
  const G4double r0;
  const G4double rc;

  G4double calEcm ( G4double , G4double , G4double ); 
  G4double calCeValue ( G4double ); 
};

#endif
