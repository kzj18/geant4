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
// Code developed by:
// S. Agostinelli, F. Foppiano, S. Garelli , M. Tropeano, S.Guatelli
//
//    ********************************
//    *                              *  
//    *    BrachyPhantomSD.cc       *
//    *                              *
//    ********************************
//
// $Id: BrachyPhantomSD.cc,v 1.3 2002-11-18 15:18:38 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "BrachyPhantomSD.hh"
#include "BrachyPhantomHit.hh"

#include "BrachyDetectorConstruction.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"

//....

BrachyPhantomSD::BrachyPhantomSD(G4String name, G4int NumVoxelX, G4int NumVoxelZ)
	:G4VSensitiveDetector(name),m_NumVoxelX(NumVoxelX),m_NumVoxelZ(NumVoxelZ)
{
 G4String HCname;
 collectionName.insert(HCname="PhantomHitsCollection");
 m_pVoxelID = new G4int[NumVoxelX*NumVoxelZ*NumVoxelZ];
 m_pPhantomHitsCollection = NULL;
}

//....

BrachyPhantomSD::~BrachyPhantomSD()
{
 delete[] m_pVoxelID;
}

//....

void BrachyPhantomSD::Initialize(G4HCofThisEvent*HCE)
{
  G4int m_NumVoxelY=300;
 m_pPhantomHitsCollection = new BrachyPhantomHitsCollection(SensitiveDetectorName,collectionName[0]);

 for(G4int k=0;k<m_NumVoxelZ;k++)
 	for(G4int i=0;i<m_NumVoxelX;i++)
            for(G4int j=0;j<m_NumVoxelY;j++)
 		m_pVoxelID[i+k*m_NumVoxelX+j*m_NumVoxelY*m_NumVoxelY] = -1;

}

//....

G4bool BrachyPhantomSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
 if(!ROhist)
	return false;

 if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() != "PhantomPhys")
	return false;

 G4double edep = aStep->GetTotalEnergyDeposit();
 if(edep==0.)
	return false;

 G4VPhysicalVolume* physVol = ROhist->GetVolume();
 G4VPhysicalVolume* mothVol = ROhist->GetVolume(1);
  
 // Read Voxel indexes: i is the x index, k is the z index
 G4int k = ROhist->GetReplicaNumber(1);
 G4int i = ROhist->GetReplicaNumber(2);
 G4int j= ROhist->GetReplicaNumber();
 if(m_pVoxelID[i+k*m_NumVoxelX+j*m_NumVoxelX*m_NumVoxelX]==-1)
 	{
	BrachyPhantomHit* PhantomHit = new BrachyPhantomHit(physVol->GetLogicalVolume(),i,j,k);
      
	G4RotationMatrix rotM;
        if(physVol->GetObjectRotation())
		rotM = *(physVol->GetObjectRotation());
      
	PhantomHit->SetEdep(edep);
        PhantomHit->SetPos(physVol->GetTranslation());
        PhantomHit->SetRot(rotM);

        G4int VoxelID = m_pPhantomHitsCollection->insert(PhantomHit);
        m_pVoxelID[i+k*m_NumVoxelX+j*m_NumVoxelX*m_NumVoxelX] = VoxelID - 1;
	}
 else
	(*m_pPhantomHitsCollection)[m_pVoxelID[i+k*m_NumVoxelX+j*m_NumVoxelX*m_NumVoxelX]]->AddEdep(edep);

 return true;
}

//....

void BrachyPhantomSD::EndOfEvent(G4HCofThisEvent*HCE)
{
  static G4int HCID = -1;
  if(HCID<0)
  	{ 
	HCID = GetCollectionID(0); 
	}
  HCE->AddHitsCollection(HCID,m_pPhantomHitsCollection);
}

//....

void BrachyPhantomSD::clear()
{
} 

//....

void BrachyPhantomSD::DrawAll()
{
} 

//....

void BrachyPhantomSD::PrintAll()
{
}



