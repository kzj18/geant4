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
// $Id: DetectorConstruction.hh,v 1.13 2006-06-29 16:51:06 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;

     const G4int MaxAbsor = 10;			// 0 + 9  
     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction();
 ~DetectorConstruction();

public:
  
  void SetNbOfAbsor     (G4int);      
  void SetAbsorMaterial (G4int,const G4String&);     
  void SetAbsorThickness(G4int,G4double);
                
  void SetAbsorSizeYZ   (G4double);          
  void SetNbOfDivisions (G4int,G4int);
    
  void SetMagField      (G4double);
     
  G4VPhysicalVolume* Construct();
  void               UpdateGeometry();
     
public:

  G4int       GetNbOfAbsor()             {return fNbOfAbsor;}     
  G4Material* GetAbsorMaterial (G4int i) {return fAbsorMaterial[i];};
  G4double    GetAbsorThickness(G4int i) {return fAbsorThickness[i];};      
  G4double    GetXfront        (G4int i) {return fXfront[i];};
            
  G4double GetAbsorSizeX()               {return fAbsorSizeX;}; 
  G4double GetAbsorSizeYZ()              {return fAbsorSizeYZ;};
  
  G4int  GetNbOfDivisions(G4int i)       {return fNbOfDivisions[i];}; 
  
  void PrintParameters();
   
private:

  G4int              fNbOfAbsor;
  G4Material*        fAbsorMaterial [MaxAbsor];
  G4double           fAbsorThickness[MaxAbsor];
  G4double           fXfront[MaxAbsor];  

  G4int              fNbOfDivisions[MaxAbsor];

  G4double           fAbsorSizeX;
  G4double           fAbsorSizeYZ;
  G4Material*        fDefaultMaterial;  
  
  G4VPhysicalVolume* fPhysiWorld;
  
  G4UniformMagField* fMagField;

  DetectorMessenger* fDetectorMessenger;

private:

  void DefineMaterials();
  void ComputeParameters();
  G4VPhysicalVolume* ConstructVolumes();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

