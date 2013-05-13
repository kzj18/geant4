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
// $Id: B2EventAction.hh 66536 2012-12-19 14:32:36Z ihrivnac $
//
/// \file B2EventAction.hh
/// \brief Definition of the B2EventAction class

#ifndef B2EventAction_h
#define B2EventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"

/// Event action class

class B2EventAction : public G4UserEventAction
{
  public:
    B2EventAction();
    virtual ~B2EventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

    // Set methods
    void SetPrintModulo(G4int value);

  private:
    G4int  fPrintModulo;
};

// inline functions

inline void B2EventAction::SetPrintModulo(G4int value) {
  fPrintModulo = value;
} 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
