// $Id: MyDetectorConstruction.cc,v 1.1 2006-05-11 04:35:32 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   MyDetectorConstruction.cc
//
//                                         2005 Q
// ====================================================================
#include "MyDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

// ====================================================================
//
// class description
//
// ====================================================================


////////////////////////////////////////////////
MyDetectorConstruction::MyDetectorConstruction()
  : scoreVoxel(0)
////////////////////////////////////////////////
{
}

/////////////////////////////////////////////////
MyDetectorConstruction::~MyDetectorConstruction()
/////////////////////////////////////////////////
{
}

//////////////////////////////////////////////////////
G4VPhysicalVolume* MyDetectorConstruction::Construct()
//////////////////////////////////////////////////////
{
  G4Material* mate;
  G4VisAttributes* va;

  // ==============================================================
  // world volume
  // ==============================================================
  G4Box* areaSolid= new G4Box("area", 25.*cm, 25.*cm, 1.1*m);
  G4Material* vacuum= G4Material::GetMaterial("Vacuum");
  G4LogicalVolume* areaLV= new G4LogicalVolume(areaSolid, vacuum, "area");
  G4PVPlacement* area= new G4PVPlacement(0, G4ThreeVector(), "area", 
					 areaLV, 0, false, 0);
  // vis. attributes
  va= new G4VisAttributes(G4Color(1.,1.,1.));
  va-> SetVisibility(false);
  areaLV-> SetVisAttributes(va);
  
  // ==============================================================
  // phantom
  // ==============================================================
  // water phantom
  const double dxyphantom= 40.*cm;
  const double dzphantom= 50.*cm;
  
  G4Box* sphantom= new G4Box("phantom", 
			     dxyphantom/2., dxyphantom/2., dzphantom/2.);
  G4Material* water= G4Material::GetMaterial("Water");
  G4LogicalVolume* lphantom= new G4LogicalVolume(sphantom, 
						 water, "phantom");
  G4PVPlacement* phantom= new G4PVPlacement(0, 
					    G4ThreeVector(0.,0., dzphantom/2.),
					    lphantom, "phantom", 
					    areaLV, false, 0);

  va= new G4VisAttributes(G4Color(0.,0.1,0.8));
  lphantom-> SetVisAttributes(va);

  // score voxels
  const G4double dvoxel= 2.*mm;
  const G4double dvoxel_y= 20.*mm;

  G4Box* svoxel= new G4Box("voxel", dvoxel/2., dvoxel_y/2., dvoxel/2.);
  scoreVoxel= new G4LogicalVolume(svoxel, water, "voxel");
  va= new G4VisAttributes(G4Color(0.,0.8,0.8));
  va-> SetVisibility(false);
  scoreVoxel-> SetVisAttributes(va);

  G4int ix, iz;
  G4int index=0;
  for (iz=0; iz<200; iz++) {
    for (ix=-40; ix<=40; ix++) {
      G4double x0= ix*dvoxel;
      G4double z0= -dzphantom/2.+(iz+0.5)*dvoxel;
      G4PVPlacement* pvoxel= new 
	G4PVPlacement(0, G4ThreeVector(x0, 0., z0),
		      scoreVoxel, "voxel", lphantom, 
		      false, index);
      index++;
    }
  }

  return area;
}


/////////////////////////////////////////////////////////////////////////
void MyDetectorConstruction::SetSDtoScoreVoxel(G4VSensitiveDetector* asd)
/////////////////////////////////////////////////////////////////////////
{
  if(scoreVoxel) {
    scoreVoxel-> SetSensitiveDetector(asd);
  }
}

