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
//
// G4 Neutron capture process -- header file
// F.W. Jones, TRIUMF, 03-DEC-96
//  
// For further comments see G4HadronCaptureProcess.cc.
//
// 27-MAR-97 FWJ: first version for Alpha release
// 14-APR-97 FWJ: cross section data class name changed
//
// 19-MAY-98 FWJ: variant G4HadronCapture process for
// G4CrossSectionDataSet/DataStore class design.
// 29-JUN-98 FWJ: default data set G4HadronCrossSections
//

// Class Description
// Process for capture of neutral hadrons; 
// to be used in your physics list in case you need this physics.
// Class Description - End


#ifndef G4HadronCaptureProcess_h
#define G4HadronCaptureProcess_h 1
 
#include "globals.hh"
#include "G4Element.hh"
#include "G4ElementVector.hh"
#include "G4VDiscreteProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4Gamma.hh"
#include "G4Step.hh"
#include "G4TrackStatus.hh"
#include "G4HadronicProcess.hh"
#include "G4CrossSectionDataStore.hh"
#include "G4HadronCaptureDataSet.hh"


class G4HadronCaptureProcess : public G4HadronicProcess
{
public:

   G4HadronCaptureProcess(const G4String& processName ="LCapture");

   ~G4HadronCaptureProcess();
 
   G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
   {
      SetDispatch(this);
      return G4HadronicProcess::GeneralPostStepDoIt(aTrack, aStep);
   }

   G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

   void DumpPhysicsTable(const G4ParticleDefinition& aParticleType);

private:

   G4double GetMicroscopicCrossSection(const G4DynamicParticle* aParticle,
                                       const G4Element* anElement,
				       G4double aTemp);

};
#endif
