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
// $Id: Tst33PVolumeStore.hh,v 1.2 2002-10-29 16:37:09 dressel Exp $
// GEANT4 tag 
//
// ----------------------------------------------------------------------
// Class Tst33PVolumeStore
//
// Class description:
//
// ...

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------

#ifndef Tst33PVolumeStore_hh
#define Tst33PVolumeStore_hh Tst33PVolumeStore_hh

#include "globals.hh"
#include "g4std/set"
#include "G4GeometryCell.hh"
#include "G4GeometryCellComp.hh"

typedef G4std::set< G4GeometryCell, G4GeometryCellComp > Tst33SetGeometryCell;

class Tst33PVolumeStore {
public:
  Tst33PVolumeStore();
  ~Tst33PVolumeStore();
  
  void AddPVolume(const G4GeometryCell &cell);
  const G4VPhysicalVolume *GetPVolume(const G4String &name) const;
  G4String GetPNames() const;

private:
  Tst33SetGeometryCell fSetGeometryCell;
};



#endif
