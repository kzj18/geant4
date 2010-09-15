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
//
// $Id: G4PSEnergyDeposit3D.cc,v 1.5 2010-09-15 21:16:41 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4PSEnergyDeposit3D
#include "G4PSEnergyDeposit3D.hh"

///////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for 3D scoring energy deposit.
//
// Created: 2007-08-14  Tsukasa ASO
///////////////////////////////////////////////////////////////////////////////

G4PSEnergyDeposit3D::G4PSEnergyDeposit3D(G4String name,
					 G4int ni, G4int nj, G4int nk,
					 G4int depi, G4int depj, G4int depk)
    :G4PSEnergyDeposit(name),
     fDepthi(depi),fDepthj(depj),fDepthk(depk)
{
  fNi=ni;
  fNj=nj;
  fNk=nk;
}

G4PSEnergyDeposit3D::G4PSEnergyDeposit3D(G4String name, const G4String& unit,
					 G4int ni, G4int nj, G4int nk,
					 G4int depi, G4int depj, G4int depk)
    :G4PSEnergyDeposit(name),
     fDepthi(depi),fDepthj(depj),fDepthk(depk)
{
  fNi=ni;
  fNj=nj;
  fNk=nk;
  SetUnit(unit);
}

G4PSEnergyDeposit3D::~G4PSEnergyDeposit3D()
{;}

G4int G4PSEnergyDeposit3D::GetIndex(G4Step* aStep)
{
  const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int i = touchable->GetReplicaNumber(fDepthi);
  G4int j = touchable->GetReplicaNumber(fDepthj);
  G4int k = touchable->GetReplicaNumber(fDepthk);
  
if(i<0)
{
  G4Exception("G4PSEnergyDeposit3D","G4PSEnergyDeposit3D::GetIndex",JustWarning,
              "GetReplicaNumber is negative");
  G4cerr << "touchable->GetReplicaNumber(fDepthi) returns " << i << G4endl;
}
if(j<0)
{
  G4Exception("G4PSEnergyDeposit3D","G4PSEnergyDeposit3D::GetIndex",JustWarning,
              "GetReplicaNumber is negative");
  G4cerr << "touchable->GetReplicaNumber(fDepthj) returns " << j << G4endl;
}
if(k<0)
{
  G4Exception("G4PSEnergyDeposit3D","G4PSEnergyDeposit3D::GetIndex",JustWarning,
              "GetReplicaNumber is negative");
  G4cerr << "touchable->GetReplicaNumber(fDepthk) returns " << k << G4endl;
}

  return i*fNj*fNk+j*fNk+k;
}
