//////////////////////////////////////////////////////////////////////////
// $Id: G4BREPSolidTorusTest.cc,v 1.4 2000-08-28 08:58:04 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//////////////////////////////////////////////////////////////////////////
//
// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// BREP solid test, create by L. Broglia, 20/10/98
// modification of old G4Gerep test
//



#include "G4Timer.hh"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "g4std/fstream"
#include "G4ios.hh" 
#include "G4BREPSolid.hh"
#include "G4BREPSolidTorus.hh"



G4int main(G4int argc, char **argv)
{
  G4Timer timer;

  G4ThreeVector tStart(19000,0,10.1);
  G4ThreeVector tDir(-1,0,0);
  G4ThreeVector tStart2(0,0,10);
  G4ThreeVector pt(0,0,50);
  G4ThreeVector pt2(100000,0,50);
  double Dist;
  
  G4cout << "\n ============   Torus test   ================";

  G4BREPSolidTorus *MyTorBox = new G4BREPSolidTorus ("MyTorBox"          ,
						     G4ThreeVector(0,0,0),
						     G4ThreeVector(0,0,1),
						     G4ThreeVector(1,0,0),     
						     0.0                 ,
						     100.0                );

  
  G4cout << "\n\nTorus created ! ";
  G4cout << "\nDir =  -1,0,0";
  G4cout << "\nStart 19000,1,0";
  Dist = MyTorBox->DistanceToIn(tStart, tDir);
  G4cout << "\nDist to in : " << Dist;
  MyTorBox->Reset();

  Dist = MyTorBox->DistanceToOut(tStart2, tDir);  
  G4cout << "\nStart 0,0,0";
  G4cout << "\nDist to out: " << Dist ;

  Dist = MyTorBox->DistanceToOut(pt);  
  G4cout << "\nPoint 0,0,50";
  G4cout << "\nDist to out: " << Dist ;

  Dist = MyTorBox->DistanceToIn(pt2);  
  G4cout << "\nPoint 100000,0,50";
  G4cout << "\nDist to in: " << Dist ;

  G4cout << G4endl << G4endl;

  return EXIT_SUCCESS;
}
