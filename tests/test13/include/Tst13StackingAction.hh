#ifndef Tst13StackingAction_h
#define Tst13StackingAction_h

#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"

class Tst13StackingAction : public G4UserStackingAction
{
      virtual G4ClassificationOfNewTrack
        ClassifyNewTrack(const G4Track* aTrack)
	{
	  G4ClassificationOfNewTrack result(fUrgent);
	  if(aTrack->GetDefinition()->GetPDGCharge() == 0 && 
	    aTrack->GetDefinition()->GetPDGMass()<200*MeV)
	  {
	    result = fKill;
	  }
	  if(aTrack->GetKineticEnergy()<1*GeV)
	  {
	    result = fKill;
	  }
	  return result;
	}
};

#endif
