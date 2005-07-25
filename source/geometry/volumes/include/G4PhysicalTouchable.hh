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
// $Id: G4PhysicalTouchable.hh,v 1.3 2005-07-25 10:02:43 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4PhysicalTouchable
//
// Class description:
//
// This is a concrete class that combines a simple physical volume,  
// with a touchable for the parent volume
//
// Behaviour expected: 
//    - pass all 'VPhysicalVolume' methods to pCurrentVol
//    - respond to GetParentTouchable method
//
// History:
// 14.02.05 J.Apostolakis - Created
// ----------------------------------------------------------------------
#ifndef G4PHYSICALTOUCHABLE_HH
#define G4PHYSICALTOUCHABLE_HH

#include "G4Types.hh"
#include "G4String.hh"

#include "geomdefs.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"

class G4LogicalVolume;
class G4VPVParameterisation;
class G4NavigationHistory; 

class G4PhysicalTouchable : public G4VPhysicalVolume
{
  public:  // with description

    G4PhysicalTouchable( G4VPhysicalVolume* pCurrentVol, 
                         const G4VTouchable* pParentTouchable); 
    G4PhysicalTouchable( G4VPhysicalVolume* pCurrentVol, 
                         const G4NavigationHistory& parentHist); 
      // Constructors, with existing touchable or history for use in one.

    virtual ~G4PhysicalTouchable();
      // Destructor, will be subclassed. Removes volume from volume-store.

    inline const G4VTouchable* GetParentTouchable() const; 
    inline void SetParentTouchable( const G4VTouchable* newParentT );  
      // Provide / set parent touchable
    inline G4VPhysicalVolume* GetCurrentVolume(); 
    inline const G4VPhysicalVolume* GetCurrentVolume() const; 
      // Access 

    inline void SetCurrentVolume( G4VPhysicalVolume* pCurrentVol ); 
      // Revise current volume pointer

    inline G4int GetMultiplicity() const;
    inline G4bool IsMany() const;

    inline G4int GetCopyNo() const;
    inline void  SetCopyNo(G4int CopyNo);
      // Get/Set the volumes copy number.
    inline G4bool IsReplicated() const;
      // Return true if replicated (single object instance represents
      // many real volumes), else false.
    inline G4bool IsParameterised() const;
      // Return true if parameterised (single object instance represents
      // many real parameterised volumes), else false.
    inline G4VPVParameterisation* GetParameterisation() const;
      // Return replicas parameterisation object 
    inline void GetReplicationData(EAxis& axis,
                                   G4int& nReplicas,
                                   G4double& width,
                                   G4double& offset,
                                   G4bool& consuming) const;
      // Return replication information. Nothing for no-replicated volumes.

  private:

    void CopyAttributes( G4VPhysicalVolume* pCurrentVol );
    G4PhysicalTouchable( const G4PhysicalTouchable & ) ; 
  
  private:

    G4VPhysicalVolume*   fpPhysVol; 
      // Current volume pointer

    const G4VTouchable*  fpTouchable;
    G4bool fCreatedParentTouch; 

}; 

#include "G4PhysicalTouchable.icc"

#endif
