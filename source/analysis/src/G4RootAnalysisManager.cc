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
// $Id$

// Author: Ivana Hrivnacova, 15/06/2011  (ivana@ipno.in2p3.fr)

#include "G4RootAnalysisManager.hh"
#include "G4UnitsTable.hh"

#include <iostream>

//_____________________________________________________________________________
G4RootAnalysisManager::G4RootAnalysisManager()
 : G4VAnalysisManager(),
   fFile(0),
   fHistoDirectory(0),
   fNtupleDirectory(0),
   fH1Vector(),   
   fH1MapByName(),
   fNtupleName(),
   fNtupleTitle(),
   fNtuple(0),
   fNtupleIColumnMap(),
   fNtupleFColumnMap(),
   fNtupleDColumnMap()
{
}

//_____________________________________________________________________________
G4RootAnalysisManager::~G4RootAnalysisManager()
{  
/*
  // CHECK
  std::vector<tools::histo::h1d*>::iterator it;
  for ( it = fH1Vector.begin(); it != fH1Vector.end(); it++ ) {
    delete *it;
  }  
  delete fNtuple;
*/
  delete fFile;  
}

// 
// private methods
//

//_____________________________________________________________________________
tools::wroot::ntuple::column<int>*    
G4RootAnalysisManager::GetNtupleIColumn(G4int id) const
{
  std::map<G4int, tools::wroot::ntuple::column<int>* >::const_iterator it
    = fNtupleIColumnMap.find(id);
  if ( it == fNtupleIColumnMap.end() ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::GetNtupleIColumn():"
           << " column " << id << " does not exist. " << G4endl;
    return 0;
  }
  
  return it->second;
}  
    
//_____________________________________________________________________________
tools::wroot::ntuple::column<float>*  
G4RootAnalysisManager::GetNtupleFColumn(G4int id) const
{
  std::map<G4int, tools::wroot::ntuple::column<float>* >::const_iterator it
    = fNtupleFColumnMap.find(id);
  if ( it == fNtupleFColumnMap.end() ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::GetNtupleFColumn():"
           << " column " << id << " does not exist. " << G4endl;
    return 0;
  }
  
  return it->second;
}  


//_____________________________________________________________________________
tools::wroot::ntuple::column<double>* 
G4RootAnalysisManager::GetNtupleDColumn(G4int id) const
{
  std::map<G4int, tools::wroot::ntuple::column<double>* >::const_iterator it
    = fNtupleDColumnMap.find(id);
  if ( it == fNtupleDColumnMap.end() ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::GetNtupleDColumn():"
           << " column " << id << " does not exist. " << G4endl;
    return 0;
  }
  
  return it->second;
}  
 
// 
// public methods
//

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::OpenFile(const G4String& fileName)
{
  // Add file extension .root if no extension is given
  G4String name(fileName);
  if ( name.find(".") == std::string::npos ) name.append(".root");
  
  fFile = new tools::wroot::file(std::cout, name);
  return true;
}  
  
//_____________________________________________________________________________
G4bool G4RootAnalysisManager::Write() 
{
  std::map<G4String, tools::histo::h1d*>::iterator it;
  for ( it = fH1MapByName.begin(); it != fH1MapByName.end(); it++ ) {
    G4bool result
      = to(*fHistoDirectory,*(it->second),it->first);
    if ( ! result ) {
      G4cerr << "---> Warning from  G4RootAnalysisManager::Write():"
             << " saving histo " << it->first << " failed" << G4endl;
      return false;       
    } 
  }
  unsigned int n;
  return fFile->write(n);
}

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::CloseFile()
{
  fFile->close();  
  return true;
} 
   
//_____________________________________________________________________________
G4int G4RootAnalysisManager::CreateH1(const G4String& name,  const G4String& title,
                               G4int nbins, G4double xmin, G4double xmax)
{

  if ( ! fHistoDirectory ) {
    fHistoDirectory = fFile->dir().mkdir(fHistoDirectoryName);
    if ( ! fHistoDirectory ) {
      G4cerr << "---> Warning from G4RootAnalysisManager::CreateH1(): " 
             << "cannot create directory " << fHistoDirectoryName << std::endl;
      return -1;       
    }       
  }

  G4int index = fH1Vector.size();
  tools::histo::h1d* h1 = new tools::histo::h1d(title, nbins, xmin, xmax);
  fH1Vector.push_back(h1);
  fH1MapByName[name] = h1;
  return index + fFirstHistoId;
}                                         

//_____________________________________________________________________________
void G4RootAnalysisManager::CreateNtuple(const G4String& name, 
                                         const G4String& title)
{
  if ( fNtuple ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::CreateNtuple():"
           << " Ntuple already exists. (Only one ntuple is currently supported.)" 
           << G4endl;
    return;       
  }

  if ( ! fNtupleDirectory ) {
    fNtupleDirectory = fFile->dir().mkdir(fNtupleDirectoryName);
    if ( ! fNtupleDirectory ) {
      G4cerr << "---> Warning from G4RootAnalysisManager::CreateH1(): " 
             << "cannot create directory " << fNtupleDirectoryName << std::endl;
      return;       
    }       
  }

  fNtuple = new tools::wroot::ntuple(*fNtupleDirectory, name, title);
  fNtupleName = name;
  fNtupleTitle = title;
}                                         

//_____________________________________________________________________________
G4int G4RootAnalysisManager::CreateNtupleIColumn(const G4String& name)
{
  G4int index = fNtuple->columns().size();
  tools::wroot::ntuple::column<int>* column = fNtuple->create_column<int>(name);  
  fNtupleIColumnMap[index] = column;
  return index + fFirstNtupleId;       
}                                         

//_____________________________________________________________________________
G4int G4RootAnalysisManager::CreateNtupleFColumn(const G4String& name)
{
  G4int index = fNtuple->columns().size();
  tools::wroot::ntuple::column<float>* column = fNtuple->create_column<float>(name);  
  fNtupleFColumnMap[index] = column;
  return index + fFirstNtupleId;       
}                                         


//_____________________________________________________________________________
G4int G4RootAnalysisManager::CreateNtupleDColumn(const G4String& name)   
{
  G4int index = fNtuple->columns().size();
  tools::wroot::ntuple::column<double>* column = fNtuple->create_column<double>(name);  
  fNtupleDColumnMap[index] = column;
  return index + fFirstNtupleId;       
}                                         

//_____________________________________________________________________________
void G4RootAnalysisManager::FinishNtuple()
{ 
  // nothing to be done here
}
   
//_____________________________________________________________________________
G4bool G4RootAnalysisManager::FillH1(G4int id, G4double value, G4double weight)
{
  tools::histo::h1d* h1d = GetH1(id);
  if ( ! h1d ) return false;

  h1d->fill(value, weight);
  return true;
}

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::FillNtupleIColumn(G4int id, G4int value)
{
  tools::wroot::ntuple::column<int>* column = GetNtupleIColumn(id);
  if ( ! column ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::FillNtupleIColumn():"
           << " column " << id << " does not exist. " << G4endl;
    return false;
  }  
  
  column->fill(value);
  return true;       
}                                         
//_____________________________________________________________________________
G4bool G4RootAnalysisManager::FillNtupleFColumn(G4int id, G4float value)
{
  tools::wroot::ntuple::column<float>* column = GetNtupleFColumn(id);
  if ( ! column ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::FillNtupleFColumn():"
           << " column " << id << " does not exist. " << G4endl;
    return false;
  }  
  
  column->fill(value);
  return true;       
}                                         
//_____________________________________________________________________________
G4bool G4RootAnalysisManager::FillNtupleDColumn(G4int id, G4double value)
{
  tools::wroot::ntuple::column<double>* column = GetNtupleDColumn(id);
  if ( ! column ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::FillNtupleDColumn():"
           << " column " << id << " does not exist. " << G4endl;
    return false;
  }  
  
  column->fill(value);
  return true;       
}                                         

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::AddNtupleRow()
{ 
  if ( ! fNtuple ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::AddNtupleRow(): " 
           << " ntuple does not exist. " << G4endl;
    return false;
  }  
  
  G4bool result =fNtuple->add_row();
  if ( ! result ) {
    G4cerr << "---> Warning from G4RootAnalysisManager::AddNtupleRow(): " 
           << " adding row has failed. " << G4endl;
  }         
  return result;
}
 
//_____________________________________________________________________________
tools::histo::h1d*  G4RootAnalysisManager::GetH1(G4int id, G4bool warn) const 
{
  G4int index = id - fFirstHistoId;
  if ( index < 0 || index >= G4int(fH1Vector.size()) ) {
    if ( warn) {
      G4cerr << "---> Warning from G4RootAnalysisManager::GetH1():"
             << " histo " << id << " does not exist. " << G4endl;
    }
    return 0;         
  }
  return fH1Vector[index];
}

