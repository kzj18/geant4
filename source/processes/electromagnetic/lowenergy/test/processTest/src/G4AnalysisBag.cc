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
// $Id: G4AnalysisBag.cc,v 1.2 ????
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Authors: MGP
//
// History:
// -----------
// 
//
// -------------------------------------------------------------------

#include "G4AnalysisBag.hh"
#include "G4Ntuple.hh"

// New Histogramming (from AIDA and Anaphe):
#include "Interfaces/IHistoManager.h"
#include "Interfaces/IHistogram1D.h"
#include "Interfaces/IHistogram2D.h"

// For NtupleTag from Anaphe
#include "NtupleTag/LizardNTupleFactory.h"
using namespace Lizard;

G4AnalysisBag::G4AnalysisBag() : histoManager(0), factory(0)
{ }

G4AnalysisBag::~G4AnalysisBag()
{ 
  delete factory;
  factory = 0;
  delete histoManager;
  histoManager = 0;

  G4int n = ntuples.size();
  for (G4int i=0; i<n; ++i)
    {
      delete ntuples[i];
      ntuples[i] = 0;
    } 
}
 
G4AnalysisBag* G4AnalysisBag::instance = 0;

G4AnalysisBag* G4AnalysisBag::getInstance()
{
  if (instance == 0)
    {
      instance = new G4AnalysisBag;
     
    }
  return instance;
}

void G4AnalysisBag::init(const G4String& file)
{
  histoManager = createIHistoManager();
  G4String name = file + ".hbook";
  histoManager->selectStore(name);

  // Create a nTuple factory:
  factory = createNTupleFactory();
}

void G4AnalysisBag::addNtuple(G4Ntuple* ntuple,const G4String& name)
{
  ntuples.push_back(ntuple);
}

const G4Ntuple* G4AnalysisBag::getNtuple(const G4String& name) const
{
  return 0;
}

void G4AnalysisBag::store()
{
}
