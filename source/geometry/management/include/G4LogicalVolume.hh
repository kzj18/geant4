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
// $Id: G4LogicalVolume.hh,v 1.10 2002-12-16 09:24:02 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// class G4LogicalVolume
//
// Class description:
//
// Represents a leaf node or unpositioned subtree in the geometry hierarchy.
// Logical volumes are named, and may have daughters ascribed to them.
// They are responsible for retrieval of the physical and tracking attributes
// of the physical volume that it represents: solid, material, magnetic field,
// and optionally, user limits, sensitive detectors.
//
// Get and Set functionality is provided for all attributes, but note that
// most set functions should not be used  when the geometry is `closed'.
// As a  further development, `Guard' checks can be added to ensure
// only legal operations at tracking time.
//
// On construction, solid, material and name must be specified.
//
// Daughters are ascribed and managed by means of a simple
// GetNoDaughters,Get&SetDaughter(n),AddDaughter interface.
//
// Smart voxels as used for tracking optimisation. They're also an attribute.
//
// Logical volumes self register to the logical volume Store on construction,
// and deregister on destruction.
//
// NOTE: This class is currently *NOT* subclassed, since not meant to
//       act as a base class. Therefore, the destructor is NOT virtual.
//
// Data members:
//
//    G4std::vector<G4VPhysicalVolume*> fDaughters
//    - Vector of daughters. Given initial size of 0.
//    G4FieldManager* fFieldManager
//    - Pointer (possibly 0) to (magnetic or other) field manager object.
//    G4Material* fMaterial
//    - Pointer to material at this node.
//    G4String fName
//    - Name of logical volume.
//    G4VSensitiveDetector *fSensitiveDetector
//    - Pointer (possibly 0) to `Hit' object.
//    G4VSolid* fSolid
//    - Pointer to solid.
//    G4UserLimits* fUserLimits
//    - Pointer (possibly 0) to user Step limit object for this node.
//    G4SmartVoxelHeader* fVoxel
//    - Pointer (possibly 0) to optimisation info objects.
//    G4bool fOptimise
//    - Flag to identify if optimisation should be applied or not.
//    G4bool fRootRegion
//    - Flag to identify if the logical volume is a root region.
//    G4double fSmartless
//    - Quality for optimisation, average number of voxels to be spent
//      per content.
//    const G4VisAttributes* fVisAttributes
//    - Pointer (possibly 0) to visualization attributes.
//    G4FastSimulationManager* fFastSimulationManager
//    - Pointer (possibly 0) to G4FastSimulationManager object.
//    G4Region* fRegion
//    - Pointer to the cuts region (if any)
//    G4MaterialCutsCouple* fCutsCouple
//    - Pointer (possibly 0) to associated production cuts.
//    G4bool fIsEnvelope
//    - Flags if the Logical Volume is an envelope for a FastSimulationManager.
//    G4double fBiasWeight
//    - Weight used in the event biasing technique.

// History:
// 24.09.02 G.Cosmo: Added flags and accessors for region cuts handling
// 17.05.02 G.Cosmo: Added IsToOptimise() method and related flag
// 18.04.01 G.Cosmo: Migrated to STL vector
// 12.02.99 S.Giani: Added user defined optimisation quality
// 09.11.98 J.Apostolakis:  Changed G4MagneticField to G4FieldManager
// 09.11.98 M.Verderi, J.Apostolakis: Added BiasWeight member and accessors
// 10.20.97 P.M.DeFreitas: Added pointer to a FastSimulation
//          J.Apostolakis: & flag to indicate if it is an Envelope for it
// 19.11.96 J.Allison: Replaced G4Visible with explicit const G4VisAttributes*
// 19.08.96 P.Kent: Split -> hh/icc/cc files; G4VSensitiveDetector change
// 11.07.95 P.Kent: Initial version.

#ifndef G4LOGICALVOLUME_HH
#define G4LOGICALVOLUME_HH

#include "globals.hh"
#include "G4Region.hh"
#include "G4VPhysicalVolume.hh"  // Need operator == for vector fdaughters
#include "g4std/vector"
#include <assert.h>

// Forward declarations
//
class G4FieldManager;
class G4Material;
class G4VSensitiveDetector;
class G4VSolid;
class G4UserLimits;
class G4SmartVoxelHeader;
class G4VisAttributes;
class G4FastSimulationManager;
class G4MaterialCutsCouple;

class G4LogicalVolume
{
  typedef G4std::vector<G4VPhysicalVolume*> G4PhysicalVolumeList;

  public:  // with description
  
    G4LogicalVolume(G4VSolid* pSolid,
                    G4Material* pMaterial,
              const G4String& name,
                    G4FieldManager* pFieldMgr=0,
                    G4VSensitiveDetector* pSDetector=0,
                    G4UserLimits* pULimits=0,
                    G4bool optimise=true);
      // Constructor. The solid and material pointer must be non null.
      // The parameters for field, detector and user limits are optional.
      // The volume also enters itself into the logical volume Store.
      // Optimisation of the geometry (voxelisation) for the volume
      // hierarchy is applied by default. For parameterised volumes in
      // the hierarchy, optimisation is -always- applied.

    ~G4LogicalVolume();
      // Destructor. Removes the logical volume from the logical volume Store.
      // NOT virtual, since not meant to act as base class.

    inline G4String GetName() const;
    inline void SetName(const G4String& pName);
      // Returns and sets the name of the logical volume.

    inline G4int GetNoDaughters() const;
      // Returns the number of daughters (0 to n).
    inline G4VPhysicalVolume* GetDaughter(const G4int i) const;
      // Return the ith daughter. Note numbering starts from 0,
      // and no bounds checking is performed.
    inline void AddDaughter(G4VPhysicalVolume* p);
      // Add the volume p as a daughter of the current logical volume.
    inline G4bool IsDaughter(const G4VPhysicalVolume* p) const;
      // Returns true is the volume p is a daughter of the current
      // logical volume.
    inline void RemoveDaughter(const G4VPhysicalVolume* p);
      // Remove the volume p from the List of daughter of the current
      // logical volume.

    inline G4VSolid* GetSolid() const;
    inline void SetSolid(G4VSolid *pSolid);
      // Gets and sets the current solid.

    inline G4Material* GetMaterial() const;
    inline void SetMaterial(G4Material *pMaterial);
      // Gets and sets the current material.

    inline G4FieldManager* GetFieldManager() const;
      // Gets current FieldManager.
    void SetFieldManager(G4FieldManager *pFieldMgr, G4bool forceToAllDaughters); 
      // Sets FieldManager and propagates it:
      //  i) only to daughters with G4FieldManager = 0
      //     if forceToAllDaughters=false
      // ii) to all daughters
      //     if forceToAllDaughters=true

    inline G4VSensitiveDetector* GetSensitiveDetector() const;
      // Gets current SensitiveDetector.
    inline void SetSensitiveDetector(G4VSensitiveDetector *pSDetector);
      // Sets SensitiveDetector (can be 0).

    inline G4UserLimits* GetUserLimits() const;
    inline void SetUserLimits(G4UserLimits *pULimits);
      // Gets and sets current UserLimits.

    inline G4SmartVoxelHeader* GetVoxelHeader() const;
    inline void SetVoxelHeader(G4SmartVoxelHeader *pVoxel);
      // Gets and sets current VoxelHeader.
    
    inline G4double GetSmartless() const;
    inline void SetSmartless(G4double s);
      // Gets and sets user defined optimisation quality.

    inline G4bool IsToOptimise() const;
      // Replies if geometry optimisation (voxelisation) is to be
      // applied for this volume hierarchy.
    inline void SetOptimisation(G4bool optim);
      // Specifies if to apply or not geometry optimisation to this
      // volume hierarchy. Note that for parameterised volumes in the
      // hierarchy, optimisation is always applied. 

    inline G4bool IsRootRegion() const;
      // Replies if the logical volume represents a root region or not.
    inline void SetRegionRootFlag(G4bool rreg);
      // Sets/unsets the volume as a root region for cuts.
    inline G4bool IsRegion() const;
      // Replies if the logical volume is part of a cuts region or not.
    inline void SetRegion(G4Region* reg);
      // Sets/unsets the volume as cuts region.
    inline const G4Region* GetRegion() const;
      // Return the region to which the volume belongs, if any.
    inline void PropagateRegion();
      // Propagates region pointer to daughters.

    inline const G4MaterialCutsCouple* GetMaterialCutsCouple() const;
    inline void SetMaterialCutsCouple(G4MaterialCutsCouple* cuts);
      // Accessors for production cuts.

    G4bool operator == (const G4LogicalVolume& lv) const;
      // Equality defined by address only.
      // Returns true if objects are at same address, else false.

    inline const G4VisAttributes* GetVisAttributes () const;
    inline void  SetVisAttributes (const G4VisAttributes* pVA);
    inline void  SetVisAttributes (const G4VisAttributes& VA);
      // Gets and sets visualization attributes.

    inline void BecomeEnvelopeForFastSimulation(G4FastSimulationManager* );
      // Makes this an Envelope for given FastSimulationManager. 
      // Ensures that all its daughter volumes get it too - unless they 
      // have one already.
    void  ClearEnvelopeForFastSimulation(G4LogicalVolume* motherLV= 0);
      // Erase volume's Envelope status and propagate the FastSimulationManager 
      // of its mother volume to itself and its daughters.
    inline G4FastSimulationManager* GetFastSimulationManager () const;
      // Gets current FastSimulationManager pointer.

    inline void SetBiasWeight (G4double w);
    inline G4double GetBiasWeight() const;
      // Sets and gets bias weight.

  private:

    void  SetFastSimulationManager (G4FastSimulationManager* pPA, 
                                    G4bool IsEnvelope);
      // Sets the fast simulation manager. Private method called by the
      // public SetIsEnvelope method with IsEnvelope = true. It is 
      // then called recursivaly to the daughters to propagate the 
      // FastSimulationManager pointer with IsEnvelope = false.

    G4LogicalVolume* FindMotherLogicalVolumeForEnvelope(); 

    G4LogicalVolume(const G4LogicalVolume&);
    G4LogicalVolume& operator=(const G4LogicalVolume&);
      // Private copy-constructor and assignment operator.

  private:

    // Data members:   

    G4PhysicalVolumeList fDaughters;
      // Vector of daughters. Given initial size of 0.
    G4FieldManager* fFieldManager;
      // Pointer (possibly 0) to (magnetic or other) field manager object.
    G4Material* fMaterial;
      // Pointer to material at this node.
    G4String fName;
      // Name of logical volume.
    G4VSensitiveDetector* fSensitiveDetector;
      // Pointer (possibly 0) to `Hit' object.
    G4VSolid* fSolid;
      // Pointer to solid.
    G4UserLimits* fUserLimits;
      // Pointer (possibly 0) to user Step limit object for this node.
    G4SmartVoxelHeader* fVoxel;
      // Pointer (possibly 0) to optimisation info objects.
    G4bool fOptimise;
      // Flag to identify if optimisation should be applied or not.
    G4bool fRootRegion;
      // Flag to identify if the logical volume is a root region.
    G4double fSmartless;
      // Quality for optimisation, average number of voxels to be spent
      // per content.
    const G4VisAttributes* fVisAttributes;
      // Pointer (possibly 0) to visualization attributes.
    G4FastSimulationManager* fFastSimulationManager;
      // Pointer (possibly 0) to G4FastSimulationManager object.
    G4Region* fRegion;
      // Pointer to the cuts region (if any)
    G4MaterialCutsCouple* fCutsCouple;
      // Pointer (possibly 0) to associated production cuts.
    G4bool fIsEnvelope;
      // Flags if the Logical Volume is an envelope for a
      // FastSimulationManager.
    G4double fBiasWeight;
      // Weight used in the event biasing technique.
};

#include "G4LogicalVolume.icc"

#endif
