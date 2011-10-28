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
//
//
// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation by Alfonso Mantero 4 Mar 2009
//
// **********************************************************************

#include "G4MolecularDecayTable.hh"
#include "G4MolecularDecayChannel.hh"

using namespace std;

// ######################################################################
// ###                      MolecularDecayTable                       ###
// ######################################################################

// constructors and destructor

G4MolecularDecayTable::G4MolecularDecayTable()
{;}

G4MolecularDecayTable::~G4MolecularDecayTable()
{
    channelsMap::iterator it_map = fDecayChannelsMap.begin();
    // fDecayChannelsMap.erase(fDecayChannelsMap.begin(),fDecayChannelsMap.end());

     for(;it_map != fDecayChannelsMap.end() ; it_map++)
     {
         vector<const G4MolecularDecayChannel*>& decayChannels = it_map->second;
         if(!decayChannels.empty())
         {
             for(int i = 0 ; i < (int) decayChannels.size() ; i++)
             {
                 if(decayChannels [i])
                 {
                     delete decayChannels[i];
                     decayChannels[i] = 0;
                 }
             }
             decayChannels.clear();
         }
     }
     fDecayChannelsMap.clear();
}

G4MolecularDecayTable::G4MolecularDecayTable(const G4MolecularDecayTable& right)
{
    *this = right;
}

G4MolecularDecayTable& G4MolecularDecayTable::operator=(const G4MolecularDecayTable& aMolecularDecayTable)
{
    fExcitedStatesMap = aMolecularDecayTable.fExcitedStatesMap;
    fDecayChannelsMap = channelsMap(aMolecularDecayTable.GetDecayChannelsMap());
    return *this;
}

const vector<const G4MolecularDecayChannel*>* const G4MolecularDecayTable::GetDecayChannels(const G4ElectronOccupancy* conf) const
{
    statesMap::const_iterator it_exstates  = fExcitedStatesMap.find(*conf);
    if(it_exstates == fExcitedStatesMap.end()) return 0;
    channelsMap::const_iterator it_decchannel =  fDecayChannelsMap.find(it_exstates->second) ;
    if(it_decchannel == fDecayChannelsMap.end()) return 0;
    return &(it_decchannel->second);
}

const vector<const G4MolecularDecayChannel*>* const G4MolecularDecayTable::GetDecayChannels(const G4String& exState) const
{
    channelsMap::const_iterator it_decchannel = fDecayChannelsMap.find(exState);
    if(it_decchannel == fDecayChannelsMap.end()) return 0;
    return &(it_decchannel->second);
}

const G4String& G4MolecularDecayTable::GetExcitedState(const G4ElectronOccupancy* conf) const
{
    statesMap::const_iterator it_exstates  = fExcitedStatesMap.find(*conf);
    if(it_exstates == fExcitedStatesMap.end())
    {
        // TODO exception
    }
    return it_exstates->second;
}

const G4ElectronOccupancy& G4MolecularDecayTable::GetElectronOccupancy(const G4String& exState) const
{
    statesMap::const_iterator statesIter;
    const G4ElectronOccupancy* conf(0);
    for (statesIter=fExcitedStatesMap.begin(); statesIter!=fExcitedStatesMap.end(); statesIter++ )
    {
        if(exState == statesIter->second) conf = &(statesIter->first);
    }

    if(statesIter == fExcitedStatesMap.end())
    {
        // TODO exception
    }
    return *conf;
}

void G4MolecularDecayTable::AddExcitedState(const G4String& label)
{
    channelsMap::iterator channelsIter = fDecayChannelsMap.find(label);
    if(channelsIter != fDecayChannelsMap.end())
    {
        G4String errMsg = "Excited State" + label + " already Exsisting in decayChannelsMap";
        G4Exception("G4MolecularDecayTable::AddExcitedState","",FatalErrorInArgument, errMsg);
    }
    fDecayChannelsMap[label] ;
}

void G4MolecularDecayTable::AddeConfToExcitedState(const G4String& label, const G4ElectronOccupancy& conf)
{
    statesMap::iterator statesIter = fExcitedStatesMap.find(conf);

    if (statesIter == fExcitedStatesMap.end())
    {
        fExcitedStatesMap[conf] = label;
    }
    else
    {
        G4Exception("G4MolecularDecayTable::AddExcitedState","",FatalErrorInArgument,"Electronic Configuration already Exsisting in excitedStatesMap");
    }
}

void G4MolecularDecayTable::AddDecayChannel(const G4String& label, const G4MolecularDecayChannel* channel)
{
    fDecayChannelsMap[label].push_back(channel);
}

void G4MolecularDecayTable::CheckDataConsistency()
{
    channelsMap::iterator channelsIter;

    //Let's check probabilities

    for (channelsIter=fDecayChannelsMap.begin(); channelsIter!=fDecayChannelsMap.end(); channelsIter++ )
    {

        vector<const G4MolecularDecayChannel*>& decayVect = channelsIter->second;
        G4double sum=0;

        G4double max = decayVect.size();

        for (size_t i=0; i<max; i++)
        {
            const G4MolecularDecayChannel* decay = decayVect[i];
            const G4double prob = decay->GetProbability();
            sum += prob;
        }

        if (sum != 1)
        {
            G4String errMsg = "Deexcitation Channels probabilities in " + channelsIter->first + "excited state don't sum up to 1";

            G4Exception("G4MolecularDecayTable::CheckDataConsistency","",FatalErrorInArgument, errMsg);
        }
    }

}
