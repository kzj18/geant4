// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Assembly.hh,v 1.3 2000-08-28 08:57:40 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4Assembly
//
// Class description:
//   
// Object providing access to the final assembled geometry as read for
// example from a STEP file description. It own a vector of placed
// solids which must be initialised through its SetPlacedVector()
// method. Currently, it simply provides a way to retrieve the pointer
// to each placed solid, and to determine the number of solids currently
// placed.

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
// ----------------------------------------------------------------------

#ifndef G4ASSEMBLY_HH
#define G4ASSEMBLY_HH

#include "G4PlacedSolid.hh"
#include "G4OrderedTable.hh"   
#include "G4BREPSolid.hh"

typedef G4RWTPtrOrderedVector<G4PlacedSolid> G4PlacedVector;  

class G4Assembly
{

public: // with description

  G4Assembly();
  ~G4Assembly();
    // Constructor & destructor

  void SetPlacedVector(G4PlacedVector&);
  inline G4PlacedSolid* GetPlacedSolid(G4int solidNumber);
  inline G4int GetNumberOfSolids();

private:  

  G4int           numberOfSolids;
  G4PlacedVector  placedVec;

};

#include "G4Assembly.icc"

#endif
