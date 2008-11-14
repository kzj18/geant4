/////////////////////////////////////////////////////////////////////////////////
//      Module:		G4AdjointAlongStepWeightCorrection.hh
//	Author:       	L. Desorgher
//	Date:		10 May 2007
// 	Organisation: 	SpaceIT GmbH
// 	Customer:     	ESA/ESTEC
/////////////////////////////////////////////////////////////////////////////////
//
// CHANGE HISTORY
// --------------
//      ChangeHistory: 
//	 	10 May 2007 creation by L. Desorgher  		
//
//-------------------------------------------------------------
//	Documentation:
//		Continuous processes acting on adjoint particles to correct continuously their weight during the adjoint reverse tracking.
//


#ifndef G4AdjointAlongStepWeightCorrection_h
#define G4AdjointAlongStepWeightCorrection_h 1

#include "G4VContinuousProcess.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Track.hh"
#include "G4ParticleChange.hh"

class G4Step;
class G4ParticleDefinition;



class G4AdjointAlongStepWeightCorrection : public G4VContinuousProcess
{
public:

  G4AdjointAlongStepWeightCorrection(const G4String& name = "ContinuousWeightCorrection",
                         G4ProcessType type = fElectromagnetic);

  virtual ~G4AdjointAlongStepWeightCorrection();


protected:
  virtual G4double GetContinuousStepLimit(const G4Track& track,
                                                G4double previousStepSize,
                                                G4double currentMinimumStep,
                                                G4double& currentSafety);
					

  //------------------------------------------------------------------------
  // Generic methods common to all processes 
  //------------------------------------------------------------------------
public:

  void PreparePhysicsTable(const G4ParticleDefinition&);

  void BuildPhysicsTable(const G4ParticleDefinition&);

  G4VParticleChange* AlongStepDoIt(const G4Track&, const G4Step&);


private:

  void DefineMaterial(const G4MaterialCutsCouple* couple);
 


  G4AdjointAlongStepWeightCorrection(G4AdjointAlongStepWeightCorrection &);
  G4AdjointAlongStepWeightCorrection & operator=(const G4AdjointAlongStepWeightCorrection &right);



protected:

  G4ParticleChange* fParticleChange;
  
  
private:
 
  const G4Material*  currentMaterial;
  const G4MaterialCutsCouple* currentCouple;
  size_t   currentMaterialIndex; 
  G4double currentTcut;
  G4double preStepKinEnergy;
  
};

inline void G4AdjointAlongStepWeightCorrection::DefineMaterial(
            const G4MaterialCutsCouple* couple)
{
  if(couple != currentCouple) {
    currentCouple   = couple;
    currentMaterial = couple->GetMaterial();
    currentMaterialIndex = couple->GetIndex();
    //G4cout<<"Define Material"<<std::endl;
    //if(!meanFreePath) ResetNumberOfInteractionLengthLeft();
  }
}


///////////////////////////////////////////////////////
//
inline G4double G4AdjointAlongStepWeightCorrection::GetContinuousStepLimit(const G4Track& track,
                G4double , G4double , G4double& )
{ 
  G4double x = DBL_MAX;
  DefineMaterial(track.GetMaterialCutsCouple());
  preStepKinEnergy = track.GetKineticEnergy();
  return x;
}
#endif
