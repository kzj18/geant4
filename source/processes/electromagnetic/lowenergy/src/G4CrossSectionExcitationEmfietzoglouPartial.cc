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
// $Id: G4CrossSectionExcitationEmfietzoglouPartial.cc,v 1.1 2007-10-15 08:57:54 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// Contact Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// Reference: TNS Geant4-DNA paper
// Reference for implementation model: NIM. 155, pp. 145-156, 1978

// History:
// -----------
// Date         Name              Modification
// 28 Apr 2007  M.G. Pia          Created in compliance with design described in TNS paper
//
// -------------------------------------------------------------------

// Class description:
// Geant4-DNA Cross total cross section for electron elastic scattering in water
// Reference: TNS Geant4-DNA paper
// S. Chauvie et al., Geant4 physics processes for microdosimetry simulation:
// design foundation and implementation of the first set of models,
// IEEE Trans. Nucl. Sci., vol. 54, no. 6, Dec. 2007.
// Further documentation available from http://www.ge.infn.it/geant4/dna

// -------------------------------------------------------------------


#include "G4CrossSectionExcitationEmfietzoglouPartial.hh"
#include "G4Track.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"

#include "Randomize.hh"

#include <deque>

G4CrossSectionExcitationEmfietzoglouPartial::G4CrossSectionExcitationEmfietzoglouPartial()
{

  nLevels = waterExcitation.NumberOfLevels();
  //G4cout << "Water excitation energy: number of levels = " << nLevels << G4endl;
}


G4CrossSectionExcitationEmfietzoglouPartial::~G4CrossSectionExcitationEmfietzoglouPartial()
{ }
 
G4double G4CrossSectionExcitationEmfietzoglouPartial::CrossSection(G4double t, G4int level)
{
  //                 Aj                        T
  // Sigma(T) = ------------- (Bj /  T) ln(Cj ---) [1 - Bj / T]^Pj
  //             2 pi alpha0                   R
  //
  // Sigma is the macroscopic cross section = N sigma, where N = number of target particles per unit volume
  // and sigma is the microscopic cross section
  // T      is the incoming electron kinetic energy
  // alpha0 is the Bohr Radius (Bohr_radius)
  // Aj, Bj, Cj & Pj are parameters that can be found in Emfietzoglou's papers
  //
  //
  // From Phys. Med. Biol. 48 (2003) 2355-2371, D.Emfietzoglou,
  // Monte Carlo Simulation of the energy loss of low energy electrons in liquid Water
  
  // Scaling for macroscopic cross section: number of water moleculs per unit volume
  // (capra) const G4double sigma0 = (10. / 3.343e22) * cm2;
  const G4double density = 3.34192e+19 * mm3;

  const G4double aj[]={0.0205, 0.0209, 0.0130, 0.0026, 0.0025};
  const G4double cj[]={4.9801, 3.3850, 2.8095, 1.9242, 3.4624};
  const G4double pj[]={0.4757, 0.3483, 0.4443, 0.3429, 0.4379};
  const G4double r = 13.6 * eV;
  
  G4double sigma = 0.;
  
  G4double exc = waterExcitation.ExcitationEnergy(level);
  // G4cout << "Water excitation energy " << exc/keV << " level = " << level << G4endl;

  if (t >= exc)
    {
      G4double excitationSigma = ( aj[level] / (2.*pi*Bohr_radius)) 
	* (exc / t) 
	* log(cj[level]*(t/r)) 
	* pow((1.- (exc/t)), pj[level]);
      sigma = excitationSigma / density;
    }
  return sigma;
}

G4int G4CrossSectionExcitationEmfietzoglouPartial::RandomSelect(G4double k)
{
  G4int i = nLevels;
  G4double value = 0.;
  std::deque<double> values;
  
  // ---- MGP ---- The following algorithm is wrong: it works is the cross section 
  // is a monotone increasing function.
  // The algorithm should be corrected by building the cumulative function 
  // of the cross section and comparing a random number in the range 0-1 against
  // the cumulative value at each bin 

  while (i > 0)
    {
      i--;
      G4double partial = CrossSection(k,i);
      values.push_front(partial);
      value += partial;
    }

    value *= G4UniformRand();
    
    i = nLevels;

    while (i > 0)
      {
	i--;
	if (values[i] > value) return i;
	value -= values[i];
      }
    
    return 0;
}

G4double G4CrossSectionExcitationEmfietzoglouPartial::Sum(G4double k)
{
  G4double totalCrossSection = 0.;

  for (G4int i=0; i<nLevels; i++)
    {
      totalCrossSection += CrossSection(k,i);
    }
  return totalCrossSection;
}
