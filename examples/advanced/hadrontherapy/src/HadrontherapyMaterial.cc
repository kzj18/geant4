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

#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "Randomize.hh"  
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "HadrontherapyMaterial.hh"

HadrontherapyMaterial::HadrontherapyMaterial()
{;}

HadrontherapyMaterial::~HadrontherapyMaterial()
{;}

void HadrontherapyMaterial::DefineMaterials()
{
  // Define required materials

  G4double A;  // atomic mass
  G4double Z;  // atomic number
  G4double d;  // density
  G4String name;
  G4int ncomponents, natoms;
  G4double fractionmass;
  ; 

  // General elements
 
A = 1.01*g/mole;
G4Element* elH = new G4Element ("Hydrogen","H",Z = 1.,A);
  
A = 14.01*g/mole;
G4Element* elN = new G4Element("Nitrogen","N",Z = 7.,A);

A = 16.00*g/mole;
G4Element* elO = new G4Element("Oxygen","O",Z = 8.,A);


A=26.98*g/mole;
G4Element* elAl = new G4Element("Aluminum","Al", Z = 13.,A);

A = 12.011*g/mole;
G4Element* elC = new G4Element("Carbon","C",Z = 6.,A);
 
A = 22.99*g/mole;
G4Element* elNa = new G4Element("Sodium","Na",Z = 11.,A);
 
A = 24.305*g/mole;
G4Element* elMg = new G4Element("Magnesium","Mg",Z = 12.,A);

A = 30.974*g/mole;
G4Element* elP = new G4Element("Phosphorus","P",Z = 15.,A);
 
A = 32.06*g/mole;
G4Element* elS = new G4Element("Sulfur","S",Z = 16.,A);
 
A = 35.453*g/mole;
G4Element* elCl = new G4Element("Chlorine","Cl",Z = 17.,A);
 
A = 39.098*g/mole;
G4Element* elK = new G4Element("Potassium","K",Z = 19.,A);

 A = 40.08*g/mole;
 G4Element* elCa = new G4Element("Calcium","Ca",Z = 20.,A);
 
 A = 65.38*g/mole;
 G4Element* elZn = new G4Element("Zinc","Zn",Z = 30.,A);
 
 A  =  54.94*g/mole;
 G4Element* elMn   =  new G4Element("Manganese","Mn",Z = 25.,A);
 
 A = 28.09*g/mole;
 G4Element* elSi  = new G4Element("Silicon","Si",Z = 14.,A);
 
 A = 52.00*g/mole;
 G4Element* elCr  = new G4Element("Chromium","Cr",Z = 24.,A);
 
 A = 58.70*g/mole;
 G4Element* elNi  = new G4Element("Nickel","Ni",Z = 28.,A);
 
 A = 55.85*g/mole;
 G4Element* elFe  = new G4Element("Iron","Fe",Z = 26.,A);
 
 A = 183.84* g/mole;
 d = 19.3*g/cm3;
 matW = new G4Material("Tungsten",Z = 74.,A,d);
 
 A = 180.948*g/mole;
 d = 16.6*g/cm3;
 G4Element* elTa = new G4Element("Tantalum","Ta", Z=73.,A); 
 
 A = 63.546*g/mole;
 d = 8.90*g/cm3;
 G4Element* elCu = new G4Element("Copper","Cu", Z=29., A);  

 
// MATERIAL ALUMINUM
 d = 2.700*g/cm3;
 matAl = new G4Material("MatAluminum",d,1);
 matAl->AddElement(elAl,1);
 
// MATERIAL SILICON
 d = 2.330*g/cm3;
 matSi = new G4Material("MatSilicon",d,1);
 matSi->AddElement(elSi,1);
 
// MATERIAL TANTALUM
 d = 16.6*g/cm3;
 matTa = new G4Material("MatTantalum", d, 1);
 matTa -> AddElement(elTa, 1);
 
 // MATERIAL COPPER
 d = 8.90*g/cm3;
 matCu = new G4Material("MatCopper", d, 1);
 matCu -> AddElement(elCu, 1);
  // Vacuum
  G4double density = universe_mean_density;
  G4double pressure = 3.e-18*pascal;
  G4double temperature = 2.73*kelvin;
  A=1.01*g/mole;
  Vacuum = new G4Material("Galactic", Z = 1., A,
				     density,kStateGas,temperature,pressure);
  

   // Perspex, plexiglass, lucite 
  d = 1.18*g/cm3;
  matplexiglass = new G4Material("PMMA",d,3);
  matplexiglass->AddElement(elH,0.08);
  matplexiglass->AddElement(elC,0.60);
  matplexiglass->AddElement(elO,0.32);
 
  // Brass
  d = 8.40*g/cm3;
  G4Material* Brass = new G4Material(name="Brass", d, ncomponents=2);         // BRASS
  Brass->AddElement(elZn, fractionmass=30*perCent);
  Brass->AddElement(elCu, fractionmass=70*perCent);


  // Kapton
  density = 1.43*g/cm3;
  G4Material* Kapton = new G4Material(name="Kapton",density, ncomponents=4);
  Kapton->AddElement(elH, natoms=10);
  Kapton->AddElement(elO, natoms=5);
  Kapton->AddElement(elC, natoms=22);
  Kapton->AddElement(elN, natoms=2);
  /*
  // Vacuum
  density     = universe_mean_density;    //from PhysicalConstants.h
  pressure    = 3.e-18*pascal;
  temperature = 2.73*kelvin;
  G4Material* Vacuum = new G4Material("Galactic", Z=1., A=1.01*g/mole,density,
                     kStateGas,temperature,pressure);
  */
  // Lead material
  A = 207.19*g/mole;
  Z = 82;
  d = 11.35*g/cm3;
  matPb = new G4Material("Lead",Z,A,d);

  // Iridium (Medical Physics, Vol 25, No 10, Oct 1998)
  d = 22.42*g/cm3;
  A = 191.96260*g/mole ;
  Z = 77;
  matir192 = new G4Material("Iridium",Z,A,d);

  //titanium
  A = 47.88*g/mole;
  d = 4.50*g/cm3;
  Titanium = new G4Material("titanium" ,Z = 22.,A,d);
 
  // Air material
  d = 1.290*mg/cm3;
  G4Material* matAir = new G4Material("Air",d,2);
  matAir->AddElement(elN,0.7);
  matAir->AddElement(elO,0.3);

  // Water
  d = 1.000*g/cm3;
  matH2O = new G4Material("Water",d,2);
  matH2O->AddElement(elH,2);
  matH2O->AddElement(elO,1);
  matH2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);


  //soft tissue(http://www.nist.gov)
  d = 1.0*g/cm3;
  soft = new G4Material("tissue",d,13);
  soft->AddElement(elH,0.104472);
  soft->AddElement(elC,0.23219);
  soft->AddElement(elN,0.02488);
  soft->AddElement(elO,0.630238);
  soft->AddElement(elNa,0.00113);
  soft->AddElement(elMg,0.00013);
  soft->AddElement(elP,0.00133);
  soft->AddElement(elS,0.00199);
  soft->AddElement(elCl,0.00134);
  soft->AddElement(elK,0.00199);
  soft->AddElement(elCa,0.00023);
  soft->AddElement(elFe,0.00005);
  soft->AddElement(elZn,0.00003); 
 
  // Stainless steel (Medical Physics, Vol 25, No 10, Oct 1998)
  d = 8.02*g/cm3 ;
  matsteel = new G4Material("Stainless steel",d,5);
  matsteel->AddElement(elMn, 0.02);
  matsteel->AddElement(elSi, 0.01);
  matsteel->AddElement(elCr, 0.19);
  matsteel->AddElement(elNi, 0.10);
  matsteel->AddElement(elFe, 0.68);
 
  //gold(chimica degli elementi N.N Greenwood,A.Earnshaw)
  A = 196.97*g/mole;
  d = 19.32*g/cm3;
  gold = new G4Material("gold",Z = 79.,A,d);

  //IodiumCore(chimica degli elementi N.N Greenwood,A.Earnshaw)
  A = 124.9*g/mole;
  d = 4.862*g/cm3;
  matI = new G4Material("Iodium",Z = 53.,A,d);

  

  
  //compact bone (http://www.NIST.gov)
  d = 1.85*g/cm3;
  bone = new G4Material("bone",d,8);
  bone->AddElement(elH,0.063984);
  bone->AddElement(elC,0.278);
  bone->AddElement(elN,0.027);
  bone->AddElement(elO,0.410016);
  bone->AddElement(elMg,0.002);
  bone->AddElement(elP,0.07);
  bone->AddElement(elS,0.002);
  bone->AddElement(elCa,0.147);

  //muscle(http://www.NIST.gov)
  muscle = new G4Material("muscle",d,9);
  muscle->AddElement(elH,0.101997);
  muscle->AddElement(elC,0.123);
  muscle->AddElement(elN,0.035);
  muscle->AddElement(elNa,0.0008);
  muscle->AddElement(elO,0.729);
  muscle->AddElement(elMg,0.0002);
  muscle->AddElement(elP,0.002);
  muscle->AddElement(elS,0.005);
  muscle->AddElement(elK,0.003);
}

G4Material* HadrontherapyMaterial::GetMat(G4String material)
{
  G4Material* pttoMaterial = G4Material::GetMaterial(material); 
  return pttoMaterial; 
}

