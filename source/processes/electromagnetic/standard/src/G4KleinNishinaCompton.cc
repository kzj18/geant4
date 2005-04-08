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
// $Id: G4KleinNishinaCompton.cc,v 1.3 2005-04-08 12:39:58 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4KleinNishinaCompton
//
// Author:        Vladimir Ivanchenko on base of Michel Maire code
//
// Creation date: 15.03.2005
//
// Modifications:
//
//
// Class Description:
//
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4KleinNishinaCompton.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4DataVector.hh"
#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4KleinNishinaCompton::G4KleinNishinaCompton(const G4ParticleDefinition*,
                                             const G4String& nam)
  : G4VEmModel(nam)
{
  theGamma = G4Gamma::Gamma();
  theElectron = G4Electron::Electron();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4KleinNishinaCompton::~G4KleinNishinaCompton()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4KleinNishinaCompton::Initialise(const G4ParticleDefinition*,
                                       const G4DataVector&)
{
  if(pParticleChange)
    fParticleChange = reinterpret_cast<G4ParticleChangeForLoss*>(pParticleChange);
  else
    fParticleChange = new G4ParticleChangeForLoss();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4KleinNishinaCompton::ComputeCrossSectionPerAtom(
                                       const G4ParticleDefinition*,
                                             G4double GammaEnergy,
                                             G4double Z, G4double,
                                             G4double, G4double)
{
  G4double CrossSection = 0.0 ;
  if ( Z < 0.9999 )                 return CrossSection;
  if ( GammaEnergy < 0.1*keV      ) return CrossSection;
  if ( GammaEnergy > (100.*GeV/Z) ) return CrossSection;

  static const G4double a = 20.0 , b = 230.0 , c = 440.0;
  
  static const G4double
    d1= 2.7965e-1*barn, d2=-1.8300e-1*barn, d3= 6.7527   *barn, d4=-1.9798e+1*barn,
    e1= 1.9756e-5*barn, e2=-1.0205e-2*barn, e3=-7.3913e-2*barn, e4= 2.7079e-2*barn,
    f1=-3.9178e-7*barn, f2= 6.8241e-5*barn, f3= 6.0480e-5*barn, f4= 3.0274e-4*barn;
     
  G4double p1Z = Z*(d1 + e1*Z + f1*Z*Z), p2Z = Z*(d2 + e2*Z + f2*Z*Z),
           p3Z = Z*(d3 + e3*Z + f3*Z*Z), p4Z = Z*(d4 + e4*Z + f4*Z*Z);

  G4double T0  = 15.0*keV; 
  if (Z < 1.5) T0 = 40.0*keV; 

  G4double X   = max(GammaEnergy, T0) / electron_mass_c2;
  CrossSection = p1Z*std::log(1.+2.*X)/X
               + (p2Z + p3Z*X + p4Z*X*X)/(1. + a*X + b*X*X + c*X*X*X);
		
  //  modification for low energy. (special case for Hydrogen)
  if (GammaEnergy < T0) {
    G4double dT0 = 1.*keV;
    X = (T0+dT0) / electron_mass_c2 ;
    G4double sigma = p1Z*log(1.+2*X)/X
                    + (p2Z + p3Z*X + p4Z*X*X)/(1. + a*X + b*X*X + c*X*X*X);
    G4double   c1 = -T0*(sigma-CrossSection)/(CrossSection*dT0);             
    G4double   c2 = 0.150; 
    if (Z > 1.5) c2 = 0.375-0.0556*log(Z);
    G4double    y = log(GammaEnergy/T0);
    CrossSection *= exp(-y*(c1+c2*y));          
  }
  //  G4cout << "e= " << GammaEnergy << " Z= " << Z << " cross= " << CrossSection << G4endl;
  return CrossSection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

std::vector<G4DynamicParticle*>* G4KleinNishinaCompton::SampleSecondaries(
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle* aDynamicGamma,
                                   G4double,
                                   G4double)
{
  // The scattered gamma energy is sampled according to Klein - Nishina formula.
  // The random number techniques of Butcher & Messel are used 
  // (Nuc Phys 20(1960),15).
  // Note : Effects due to binding of atomic electrons are negliged.
 
  G4double gamEnergy0 = aDynamicGamma->GetKineticEnergy();
  G4double E0_m = gamEnergy0 / electron_mass_c2 ;

  G4ThreeVector gamDirection0 = aDynamicGamma->GetMomentumDirection();

  //
  // sample the energy rate of the scattered gamma 
  //

  G4double epsilon, epsilonsq, onecost, sint2, greject ;

  G4double epsilon0   = 1./(1. + 2.*E0_m);
  G4double epsilon0sq = epsilon0*epsilon0;
  G4double alpha1     = - log(epsilon0);
  G4double alpha2     = 0.5*(1.- epsilon0sq);

  do {
    if ( alpha1/(alpha1+alpha2) > G4UniformRand() ) {
      epsilon   = exp(-alpha1*G4UniformRand());   // epsilon0**r
      epsilonsq = epsilon*epsilon; 

    } else {
      epsilonsq = epsilon0sq + (1.- epsilon0sq)*G4UniformRand();
      epsilon   = sqrt(epsilonsq);
    };

    onecost = (1.- epsilon)/(epsilon*E0_m);
    sint2   = onecost*(2.-onecost);
    greject = 1. - epsilon*sint2/(1.+ epsilonsq);

  } while (greject < G4UniformRand());
 
  //
  // scattered gamma angles. ( Z - axis along the parent gamma)
  //

  G4double cosTeta = 1. - onecost; 
  G4double sinTeta = sqrt (sint2);
  G4double Phi     = twopi * G4UniformRand();
  G4double dirx = sinTeta*cos(Phi), diry = sinTeta*sin(Phi), dirz = cosTeta;

  //
  // update G4VParticleChange for the scattered gamma
  //
   
  G4ThreeVector gamDirection1 ( dirx,diry,dirz );
  gamDirection1.rotateUz(gamDirection0);
  G4double gamEnergy1 = epsilon*gamEnergy0;
  fParticleChange->SetProposedKineticEnergy(gamEnergy1);
  if(gamEnergy1 > DBL_MIN) 
    fParticleChange->SetProposedMomentumDirection(gamDirection1);
  else 
    fParticleChange->ProposeTrackStatus(fStopAndKill);
  
  std::vector<G4DynamicParticle*>* fvect = new std::vector<G4DynamicParticle*>;
 
  //
  // kinematic of the scattered electron
  //

  G4double eKinEnergy = gamEnergy0 - gamEnergy1;

  if(eKinEnergy > DBL_MIN) {
    G4ThreeVector eDirection = gamEnergy0*gamDirection0 - gamEnergy1*gamDirection1;
    eDirection = eDirection.unit();

    // create G4DynamicParticle object for the electron.
    G4DynamicParticle* dp = new G4DynamicParticle(theElectron,eDirection,eKinEnergy);
    fvect->push_back(dp);
  }
  return fvect;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


