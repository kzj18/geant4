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
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4PhotoElectricAngularGeneratorStandard
//
// Creation date: 10 May 2004
//
// Modifications: 
// 10 May 2003     P. Rodrigues    First implementation acording with new design
//
// Class Description: 
//
// Concrete class for PhotoElectric Electron Angular Distribution Generation 
// This model is a re-implementation of the Photolectric angular distribution
// developed my M. Maire for the Standard EM Physics G4PhotoElectricEffect 
//
// Class Description: End 
//
// -------------------------------------------------------------------
//
//    

#include "G4PhotoElectricAngularGeneratorStandard.hh"
#include "Randomize.hh"

//    

G4PhotoElectricAngularGeneratorStandard::G4PhotoElectricAngularGeneratorStandard(const G4String& name):G4VPhotoElectricAngularDistribution(name)
{;}

//    

G4PhotoElectricAngularGeneratorStandard::~G4PhotoElectricAngularGeneratorStandard() 
{;}

//

G4ThreeVector G4PhotoElectricAngularGeneratorStandard::GetPhotoElectronDirection(G4ThreeVector direction, G4double eKineticEnergy)
{

  // Compute Theta distribution of the emitted electron, with respect to the
  // incident Gamma.
  // The Sauter-Gavrila distribution for the K-shell is used. (adapted from G4PhotoElectricEffect)

  G4double costeta = 1.;
  G4double Phi     = twopi * G4UniformRand();
  G4double cosphi = std::cos(Phi);
  G4double sinphi = std::sin(Phi);
  G4double sinteta = 0;
  G4double gamma   = 1. + eKineticEnergy/electron_mass_c2;

  if (gamma > 5.) {
    G4ThreeVector direction (sinteta*cosphi, sinteta*sinphi, costeta);
    return costeta;
  }

  G4double beta  = std::sqrt(gamma*gamma-1.)/gamma;
  G4double b     = 0.5*gamma*(gamma-1.)*(gamma-2);
    
  G4double rndm,term,greject,grejsup;
  if (gamma < 2.) grejsup = gamma*gamma*(1.+b-beta*b);
  else            grejsup = gamma*gamma*(1.+b+beta*b);
  
  do { rndm = 1.-2*G4UniformRand();
       costeta = (rndm+beta)/(rndm*beta+1.);
       term = 1.-beta*costeta;
       greject = (1.-costeta*costeta)*(1.+b*term)/(term*term);
  } while(greject < G4UniformRand()*grejsup);
       

  sinteta = std::sqrt(1.-costeta*costeta);
  G4ThreeVector photoelectrondirection (sinteta*cosphi, sinteta*sinphi, costeta);
  photoelectrondirection.rotateUz(direction);
  return photoelectrondirection;
}

//

void G4PhotoElectricAngularGeneratorStandard::PrintGeneratorInformation() const
{
  G4cout << "\n" << G4endl;
  G4cout << "" << G4endl;
  G4cout << "Re-implementation of the photolectric angular distribution" << G4endl;
  G4cout << "developed my M. Maire for the Standard EM Physics G4PhotoElectricEffect" << G4endl;
  G4cout << "It computes the theta distribution of the emitted electron, with respect to the" << G4endl;
  G4cout << "incident Gamma, using the Sauter-Gavrila distribution for the K-shell\n" << G4endl;
} 
