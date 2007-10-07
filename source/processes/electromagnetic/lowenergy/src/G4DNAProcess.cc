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
// $Id: G4DNAProcess.cc,v 1.1 2007-10-07 12:56:54 pia Exp $
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
// Geant4-DNA dummy final state for test purpose
// Further documentation available from http://www.ge.infn.it/geant4/dna

// -------------------------------------------------------------------

#include "G4DNAProcess.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4VParticleChange.hh"
#include "G4DynamicParticle.hh"
#include "G4ThreeVector.hh"
#include "G4FinalStateProduct.hh"
#include <vector>

template <class TCrossSection,class TFinalState>
G4double G4DNAProcess<TCrossSection,TFinalState>::GetMeanFreePath(const G4Track& track, 
								  G4double previousStepSize, 
								  G4ForceCondition* condition)
{ 
  G4double meanFreePath = DBL_MAX;
  G4double cross = crossSection.CalculateCrossSection(track);
  if (cross > 0.0) meanFreePath = 1.0/cross;
  return meanFreePath;
}

								  
template <class TCrossSection,class TFinalState>
G4VParticleChange* G4DNAProcess<TCrossSection,TFinalState>::PostStepDoIt(const G4Track& track, const G4Step& step)
{
  aParticleChange.Initialize(track);
  
  // Interaction product
  const G4FinalStateProduct& product = finalState.GenerateFinalState(track,step);
  
  // Number of secondary products to be generated
  G4int nSecondaries  = product.NumberOfSecondaries();
  aParticleChange.SetNumberOfSecondaries(nSecondaries);
  
  // Secondaries
  for (G4int l = 0; l<nSecondaries; l++ )
    {
      G4DynamicParticle* particle = product.GetSecondaries()[l];
      if (particle != 0) 
	{
	  aParticleChange.AddSecondary(particle);
	}
    }
  
  // Take care of incident particle to be killed, if necessary; dump its energy deposit locally
  G4double deposit = product.GetEnergyDeposit();
  if (deposit > 0.0) aParticleChange.ProposeLocalEnergyDeposit(deposit);
  
  if (product.GetKillParticleStatus())
    {
      aParticleChange.ProposeTrackStatus(fStopAndKill);
      aParticleChange.ProposeEnergy(0.);
      aParticleChange.ProposeMomentumDirection( 0., 0., 0. );
    }
  else
    {
      // Modify incident particle kinematics taking into account the generated products
      
      // ---- MGP ---- Temporary: assume at most one secondary product
      //               Sebastien: please check if consistent with current models or generalize
      
      // Primary particle momentum and kinetic energy
      G4ThreeVector primaryMomentum = track.GetMomentum();
      G4double primaryKineticEnergy = track.GetKineticEnergy();
      
      // Secondary product momentum and energy
      
      G4double secondaryKineticEnergy = 0.;
      if (nSecondaries == 1)
	{
	  G4DynamicParticle* secondary = product.GetSecondaries()[0];
	  secondaryKineticEnergy = secondary->GetKineticEnergy();
	  
	  // Calculate new primary particle kinetic energy
	  G4double finalKinEnergy = primaryKineticEnergy - secondaryKineticEnergy - deposit;
	  
	  if (finalKinEnergy <= 0.0) 
	    {
	      aParticleChange.ProposeTrackStatus(fStopAndKill);
	      aParticleChange.ProposeEnergy(0.);
	      aParticleChange.ProposeMomentumDirection( 0., 0., 0. );
	    } 
	  else 
	    {
	      G4ThreeVector secondaryMomentum = secondary->GetMomentum();
	      
	      // Calculate new primary particle momentum
	      G4ThreeVector finalMomentum = primaryMomentum - secondaryMomentum;
	      G4ThreeVector finalDirection = finalMomentum.unit();
	      aParticleChange.ProposeMomentumDirection(finalDirection);
	      aParticleChange.ProposeEnergy(finalKinEnergy);
	    }
	}
      
    }
  return G4VDiscreteProcess::PostStepDoIt(track,step );
}

