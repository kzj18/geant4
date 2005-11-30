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
#ifndef Tst22PhysicsList_h
#define Tst22PhysicsList_h 1

//#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//class Tst22PhysicsList: public G4VModularPhysicsList
class Tst22PhysicsList: public G4VUserPhysicsList
{
public:
  Tst22PhysicsList();
  virtual ~Tst22PhysicsList();
  
//public:
  // SetCuts()
protected: 
  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();

  virtual void SetCuts();

protected:
  // these methods Construct physics processes and register them
  virtual void ConstructGeneral();
  virtual void ConstructEM();
  virtual void ConstructHad();
  virtual void ConstructLeptHad();
  //
  void  ConstructAllBosons();
  void  ConstructAllLeptons();
  void  ConstructAllMesons();
  void  ConstructAllBaryons();
  void  ConstructAllIons();
  void  ConstructAllShortLiveds();

};


#endif



