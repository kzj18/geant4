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
// INCL++ intra-nuclear cascade model
// Pekka Kaitaniemi, CEA and Helsinki Institute of Physics
// Davide Mancusi, CEA
// Alain Boudard, CEA
// Sylvie Leray, CEA
// Joseph Cugnon, University of Liege
//
// INCL++ revision: v5.1.5
//
#define INCLXX_IN_GEANT4_MODE 1

#include "globals.hh"

#ifndef G4INCLXXInterface_hh
#define G4INCLXXInterface_hh 1

#include "G4Nucleon.hh"
#include "G4Nucleus.hh"
#include "G4HadronicInteraction.hh"
#include "G4VIntraNuclearTransportModel.hh"
#include "G4KineticTrackVector.hh"
#include "G4FragmentVector.hh"
#include "G4ParticleChange.hh"
#include "G4ReactionProductVector.hh"
#include "G4ReactionProduct.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

// INCL++
#include "G4INCLCascade.hh"

// Geant4 de-excitation
#include "G4ExcitationHandler.hh"

// Binary cascade
#include "G4BinaryLightIonReaction.hh"

#include <fstream>
#include <iostream>

using namespace std;

class G4INCLXXInterfaceStore;

/** \brief INCL++ intra-nuclear cascade with G4ExcitationHandler for de-excitation
 *
 * Interface for INCL++. This interface handles basic hadron bullet particles
 * (protons, neutrons, pions), as well as light ions.
 *
 * Example usage in case of protons:
 * @code
 * G4INCLXXInterface* inclModel = new G4INCLXXInterface;
 * inclModel -> SetMinEnergy(0.0 * MeV); // Set the energy limits
 * inclModel -> SetMaxEnergy(3.0 * GeV);
 *
 * G4ProtonInelasticProcess* protonInelasticProcess = new G4ProtonInelasticProcess(); 
 * G4ProtonInelasticCrossSection* protonInelasticCrossSection =  new G4ProtonInelasticCrossSection(); 
 *
 * protonInelasticProcess -> RegisterMe(inclModel);
 * protonInelasticProcess -> AddDataSet(protonInelasticCrossSection);
 *
 * particle = G4Proton::Proton();
 * processManager = particle -> GetProcessManager();
 * processManager -> AddDiscreteProcess(protonInelasticProcess);
 * @endcode
 * The same setup procedure is needed for neutron, pion and generic-ion
 * inelastic processes as well.
 */
class G4INCLXXInterface : public G4VIntraNuclearTransportModel {
public:
  G4INCLXXInterface(const G4String& name = "INCL++ cascade with G4ExcitationHandler");
  ~G4INCLXXInterface(); // Destructor

  G4int operator==(G4INCLXXInterface& right) {
    return (this == &right);
  }

  G4int operator!=(G4INCLXXInterface& right) {
    return (this != &right);
  }

  G4ReactionProductVector* Propagate(G4KineticTrackVector* theSecondaries, G4V3DNucleus* theNucleus); // Idle

  /**
   * Main method to apply the INCL physics model.
   * @param aTrack the projectile particle
   * @param theNucleus target nucleus
   * @return the output of the INCL physics model
   */
  G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack,  G4Nucleus& theNucleus); 

  void DeleteModel() {
    delete theINCLModel;
    theINCLModel = NULL;
  }

private:
  G4bool AccurateProjectile(const G4HadProjectile &aTrack, const G4Nucleus &theTargetNucleus) const;

  /// \brief Dummy copy constructor to shut up Coverity warnings
  G4INCLXXInterface(const G4INCLXXInterface &rhs);

  /// \brief Dummy assignment operator to shut up Coverity warnings
  G4INCLXXInterface &operator=(G4INCLXXInterface const &rhs);

  /// \brief Convert G4ParticleDefinition to corresponding INCL particle type
  G4INCL::ParticleType toINCLParticleType(G4ParticleDefinition const * const) const;

  /// \brief Convert G4HadProjectile to corresponding INCL particle species
  G4INCL::ParticleSpecies toINCLParticleSpecies(G4HadProjectile const &) const;

  /// \brief Convert G4HadProjectile to corresponding INCL particle kinetic energy
  G4double toINCLKineticEnergy(G4HadProjectile const &) const;

  /// \brief Convert an INCL particle to a G4DynamicParticle
  G4DynamicParticle *toG4Particle(G4int A, G4int Z , G4double kinE, G4double px, G4double py, G4double pz) const;

  /// \brief Convert A and Z to a G4ParticleDefinition
  G4ParticleDefinition *toG4ParticleDefinition (G4int A, G4int Z) const;

  /// \brief Rescale remnant momentum if necessary
  G4double remnant4MomentumScaling(G4double mass,
      G4double kineticE,
      G4double px, G4double py, G4double pz) const;

  G4INCL::INCL *theINCLModel;
  G4HadFinalState theResult;

  G4ExcitationHandler *theExcitationHandler;

  G4HadronicInteraction *theBackupModel;

  G4INCLXXInterfaceStore * const theInterfaceStore;

  G4bool complainedAboutBackupModel;

};

#endif
