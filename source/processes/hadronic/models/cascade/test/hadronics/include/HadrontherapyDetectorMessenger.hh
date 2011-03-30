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
// $Id: HadrontherapyDetectorMessenger.hh; May 2005
// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the INFN, Catania, Italy
// (b) INFN Section of Genova, Genova, Italy
// 
// * cirrone@lns.infn.it
// ----------------------------------------------------------------------------
#ifndef HadrontherapyDetectorMessenger_h
#define HadrontherapyDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class HadrontherapyDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class HadrontherapyDetectorMessenger: public G4UImessenger
{
  public:
  HadrontherapyDetectorMessenger(HadrontherapyDetectorConstruction* );
  ~HadrontherapyDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
private:

  // Pointer to the detector component
  HadrontherapyDetectorConstruction* hadrontherapyDetector;
  
  G4UIdirectory* modulatorDir; // Control of the modulator 
  G4UIdirectory* beamLineDir;  // Control of the beam line
 
  G4UIdirectory* rangeShifterDir; 
  // Control of the range shifter component of the beam line

  G4UIdirectory* firstScatteringFoilDir;
  // Control of the first scattering foil component of the beam line
  
  G4UIdirectory* secondScatteringFoilDir;
  // Control of the first scattering foil component of the beam line
  
  G4UIdirectory* rangeStopperDir;
  // Control of the range stopper component of the beam line
  
  G4UIdirectory* finalCollimatorDir;
  // Control of the final collimator component of the beam line
  
  G4UIcmdWithADoubleAndUnit* modulatorAngleCmd;
  // UI command to rotate the modulator wheel

  G4UIcmdWithAString*   rangeShifterMatCmd;
  // UI command to set the material of the rangeShifter component of 
  // the beam line 

  G4UIcmdWithADoubleAndUnit* rangeShifterXSizeCmd;
  // UI command to set half of the X size of the rangeShifter component of 
  // the beam line 

  G4UIcmdWithADoubleAndUnit* rangeShifterXPositionCmd;
  // UI command to change the X position of the rangeShifter component of 
  // the beam line 

  G4UIcmdWithADoubleAndUnit* firstScatteringFoilXSizeCmd;
  // UI command to set half X size of the first scattering foil of 
  // the beam line 

  G4UIcmdWithADoubleAndUnit* secondScatteringFoilXSizeCmd;
  // UI command to set half X size of the second scattering foil 
  // the beam line 

  G4UIcmdWithADoubleAndUnit* outerRadiusStopperCmd;
  // UI command to set the outer radius of the range stopper component of 
  // the beam line 

  G4UIcmdWithADoubleAndUnit* innerRadiusFinalCollimatorCmd;
  // UI command to set the inner radius of the final collimator component of 
  // the beam line 
};
#endif

