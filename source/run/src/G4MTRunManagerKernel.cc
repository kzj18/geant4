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

#include "G4MTRunManagerKernel.hh"
#include "G4RegionStore.hh"
#include "G4StateManager.hh"

G4MTRunManagerKernel::G4MTRunManagerKernel() : G4RunManagerKernel(masterRMK)
{
    //This version of the constructor should never be called in sequential mode!
#ifndef G4MULTITHREADED
    G4ExceptionDescription msg;
    msg<<"Geant4 code is compiled without multi-threading support (-DG4MULTITHREADED is set to off).";
    msg<<" This type of RunManager can only be used in mult-threaded applications.";
    G4Exception("G4RunManagerKernel::G4RunManagerKernel()","Run0035",FatalException,msg);
#endif

}

G4MTRunManagerKernel::~G4MTRunManagerKernel()
{
}

void G4MTRunManagerKernel::SetupShadowProcess() const
{
    //Behavior is the same as base class (sequential mode)
    //ShadowProcess pointer == process poitner
    G4RunManagerKernel::SetupShadowProcess();
}