// generated by Geant4 Geometry Editor at Mon Sep 11 13:12:50 GMT+00:00 2000

#ifndef ZIIIDetectorConstruction_H

#define ZIIIDetectorConstruction_H 1

class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"

class ZIIIDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ZIIIDetectorConstruction();
    ~ZIIIDetectorConstruction();
     const 
     G4VPhysicalVolume* GetCalorimeter() {return physiCalorimeter;};

  private:
    G4VPhysicalVolume* Construct();
  //     G4Box*             solidCalorimeter;  //pointer to the solid Tracker
  //   G4LogicalVolume*   logicCalorimeter;  //pointer to the logical Tracker
   G4VPhysicalVolume* physiCalorimeter;  //pointer to the physical Tracker



};

#endif

