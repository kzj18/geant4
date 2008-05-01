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
// -------------------------------------------------------------------
//      GEANT 4 class file 
//
//      CERN, Geneva, Switzerland
//
//      File name:     G4DiscreteGammaDeexcitation
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it)
// 
//      Creation date: 23 October 1998
//
//      Modifications: 
//      8 March 2001, Fan Lei (flei@space.qinetiq.com)
//         Added the following as part if the IC implementation
//            void SetICM(G4bool hl) { _icm = hl; };
//            void SetRDM(G4bool hl) { _rdm = hl; };
//            void SetHL(G4double hl) { _max_hl = hl; };
//         Changed in CreateTransition() from 
//                     return new G4DiscreteGammaTransition(*level);  
//                 To
//                     return new G4DiscreteGammaTransition(*level,Z);
//         Added in CanDoTransition
//                if (level->HalfLife() > _max_hl && !_rdm ) canDo = false;
//      
// -------------------------------------------------------------------

#include "G4DiscreteGammaDeexcitation.hh"
#include "G4DiscreteGammaTransition.hh"
#include "G4NuclearLevelManager.hh"
#include "G4NuclearLevelStore.hh"

#include "G4ios.hh"
#include <fstream>
#include <sstream>


G4DiscreteGammaDeexcitation::G4DiscreteGammaDeexcitation(): 
  _nucleusZ(0), _nucleusA(0), _max_hl(1e-6*second), _icm(false),
  _rdm(false), _levelManager(0)
{
  _tolerance = 0.1 * MeV;
}


G4DiscreteGammaDeexcitation::~G4DiscreteGammaDeexcitation() {}


G4VGammaTransition* G4DiscreteGammaDeexcitation::CreateTransition()
{
  G4Fragment nucleus = GetNucleus();
  G4int A = static_cast<G4int>(nucleus.GetA());
  G4int Z = static_cast<G4int>(nucleus.GetZ());

  if (_nucleusA != A || _nucleusZ != Z) 
    {
      _nucleusA = A;
      _nucleusZ = Z;
      _levelManager = G4NuclearLevelStore::GetInstance()->GetManager(Z,A);
    }



  if (_levelManager->IsValid()) 
    { 
      if (_verbose > 1)
	{
	  G4cout 
	    << "G4DiscreteGammaDeexcitation::CreateTransition - (A,Z) is valid " 
	    << G4endl;
	}
	
      G4double excitation = nucleus.GetExcitationEnergy();
      //      const G4NuclearLevel* level =_levelManager.NearestLevel(excitation, _tolerance);
      const G4NuclearLevel* level =_levelManager->NearestLevel(excitation);
	
      if (level != 0)  
	{ 
	  if (_verbose > 0)
	    G4cout 
	      << "G4DiscreteGammaDeexcitation::CreateTransition - Created from level energy " 
	      << level->Energy() << ", excitation is " 
	      << excitation << G4endl;
	  G4DiscreteGammaTransition* dtransit = new G4DiscreteGammaTransition(*level,Z);
	  dtransit->SetICM(_icm);  
	  return dtransit;
	}
      else 
	{ 
	  if (_verbose > 0)
	    G4cout 
	      << "G4DiscreteGammaDeexcitation::CreateTransition - No transition created from "
	      << excitation << " within tolerance " << _tolerance << G4endl;
	    
	  return 0; 
	}
    }
  else return 0;
}


G4bool G4DiscreteGammaDeexcitation::CanDoTransition() const
{

  G4bool canDo = true;
    
  if (_transition == 0) {
    canDo = false;
    
    if (_verbose > 0)
      G4cout 
	<< "G4DiscreteGammaDeexcitation::CanDoTransition - Null transition " 
	<< G4endl;
  } 
  G4Fragment nucleus = GetNucleus();
  if (canDo)  {
    G4double A = nucleus.GetA();
    G4double Z = nucleus.GetZ();
    if (Z<2 || A<3 || Z>98)
      {
	canDo = false;
	if (_verbose > 0) 
	  G4cout 
	    << "G4DiscreteGammaDeexcitation::CanDoTransition - n/p/H/>Cf"
	    << G4endl;
      }
  }

  G4double excitation = nucleus.GetExcitationEnergy();

  if (canDo) {
    if (excitation <= 0.) {
      canDo = false;
      if (_verbose > 0) 
	G4cout 
	  << "G4DiscreteGammaDeexcitation::CanDoTransition -  Excitation <= 0" 
	  << G4endl;
    } else { 
      if (excitation > _levelManager->MaxLevelEnergy() + _tolerance) canDo = false;
      if (excitation < _levelManager->MinLevelEnergy() - _tolerance) canDo = false;  
      // The following is a protection to avoid looping in case of elements with very low
      // ensdf levels
      if (excitation < _levelManager->MinLevelEnergy() * 0.9) canDo = false;  
  
      if (_verbose > 0) {
	G4cout << "G4DiscreteGammaDeexcitation::CanDoTransition -  Excitation " 
	       << excitation << ", Min-Max are " 
	       << _levelManager->MinLevelEnergy() << " "
	       << _levelManager->MaxLevelEnergy() << G4endl;
      }
    }
  }
  
  if (canDo) {
    const G4NuclearLevel* level = _levelManager->NearestLevel(excitation);  
    if (level != 0) {  
      if (level->HalfLife() > _max_hl && !_rdm ) canDo = false;
    } else {
      canDo = false;
    }
    if (_verbose > 0) {
      G4cout << "G4DiscreteGammaDeexcitation::CanDoTransition -  Halflife " 
	     << level->HalfLife() << ", Calling from RDM " 
	     << (_rdm ? " True " : " False ")  << ", Max-HL = " <<  _max_hl << G4endl;
    }
  }
  if (_verbose > 0) {
    G4cout <<"G4DiscreteGammaDeexcitation::CanDoTransition - CanDo:" 
	   <<  (canDo ? " True " : " False ")  << G4endl; 
  }
  
  return canDo;
      
}

