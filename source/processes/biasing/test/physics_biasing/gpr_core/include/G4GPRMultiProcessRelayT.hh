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
// $Id: G4GPRMultiProcessRelayT.hh,v 1.2 2007-09-06 22:10:09 tinslay Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// J. Tinslay, August 2007. 
//
#ifndef G4GPRMULTIPROCESSRELAYT_HH
#define G4GPRMULTIPROCESSRELAYT_HH

#include "G4GPRWrapItUp.hh"

template <typename L>
class G4GPRMultiProcessRelayT {

public:
  
  typedef L List;
  typedef typename G4GPRProcessWrappers::Wrappers<List>::MultiProcessRelayWrapper Wrapper;

  template <typename Pointer, typename MemberFunc>
  G4GPRMultiProcessRelayT(const G4String& name, const Pointer& pointer,
			  MemberFunc memberFunc, std::vector<unsigned> processIndices, G4int placement)
    :fName(name)
    ,fActive(true) 
    ,fPlacement(placement)
    ,fProcessIndices(processIndices)
  {
    fWrapped = Wrapper(name, pointer, memberFunc);
  }

  // Construct with either a regular pointer or function pointer.
  // Need to figure out which it is so can do correct wrapping
  template <typename Pointer>
  G4GPRMultiProcessRelayT(const G4String& name, const Pointer& pointer, std::vector<unsigned> processIndices, G4int placement)
    :fName(name)
    ,fActive(true)
    ,fPlacement(placement)
    ,fProcessIndices(processIndices)
  {
    G4GPRWrapItUp<Wrapper, Pointer> doWrapping;  
    fWrapped = doWrapping.Wrap(name, pointer);
  }

  G4bool IsActive() {return fActive;}

  void ChangeState() 
  {
    fActive = !fActive;
  }

  const std::vector<unsigned>& GetProcessIndices() 
  {
    return fProcessIndices;
  }

  G4int Placement() {return fPlacement;}

  Wrapper GetWrapper() {return fWrapped;}

  G4String GetName() {return fName;}

private:

  G4String fName;
  G4bool fActive;
  G4int fPlacement;
  Wrapper fWrapped;
  std::vector<unsigned> fProcessIndices;
};

#endif
