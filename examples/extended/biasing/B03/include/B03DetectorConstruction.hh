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
// $Id: B03DetectorConstruction.hh,v 1.3 2002-08-29 15:37:25 dressel Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef B03DetectorConstruction_hh
#define B03DetectorConstruction_hh B03DetectorConstruction_hh

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4IStore;

class B03DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  B03DetectorConstruction();
  ~B03DetectorConstruction();
  
  G4VPhysicalVolume* Construct();
  G4IStore* GetIStore();

private:
  G4IStore *fIStore;
};

#endif
