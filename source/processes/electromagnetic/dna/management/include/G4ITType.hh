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
// Author: Mathieu Karamitros (kara@cenbg.in2p3.fr)
//
// WARNING : This class is released as a prototype.
// It might strongly evolve or even disapear in the next releases.
//
// History:
// -----------
// 10 Oct 2011 M.Karamitros created
//
// -------------------------------------------------------------------

#ifndef G4ITTYPE_HH
#define G4ITTYPE_HH 1
#include "globals.hh"
#include <vector>
#include <cstddef>

struct G4ITType
{
private :
    friend G4ITType operator +(const G4ITType& left,const int& right);
    friend G4ITType operator -(const G4ITType& left,const int& right);
    int i;

public :

    static size_t size();

    G4ITType(const int d_) : i(d_) {;}
    G4ITType(){i=0;}
    G4ITType(const G4ITType & d_) : i(d_.i){;}
    inline G4ITType & operator=(const G4ITType & rhs) { this->i = rhs.i; return *this;}
    G4ITType & operator=(const int & rhs) { this->i = rhs; return *this;}
    inline operator int & () { return i; }
    inline operator const int & () const { return i; }
    inline bool operator==(const G4ITType & rhs) const { return i == rhs.i; }
    inline bool operator==(const int & rhs) const { return i == rhs; }
    inline bool operator<(const G4ITType & rhs) const { return i < rhs.i; }
};

inline G4ITType operator +(const G4ITType& left,const int& right) {
    G4ITType output( left.i + right );
    return output;
}

inline G4ITType operator -(const G4ITType& left,const int& right) {
    G4ITType output( left.i - right );
    return output;
}

class G4ITTypeManager : public std::vector<G4ITType>
{
private:
    static G4ITTypeManager* fInstance ;

public :
    G4ITType NewType() ;
    static G4ITTypeManager* Instance();
    G4ITTypeManager();
    virtual ~G4ITTypeManager();
};

#define ITDef(T)\
public:\
static G4ITType fType;\
static const G4ITType ITType()\
{\
    return fType;\
}\
const G4ITType GetITType() const\
{\
    return fType;\
}\
virtual G4bool equal(const G4IT &right) const \
{\
    const T& right_mol = (const T&)right ;\
    return (this->operator==(right_mol));\
}\
virtual G4bool diff(const G4IT &right) const\
{\
    const T& right_mol = (const T&)right ;\
    return (this->operator<(right_mol));\
}

#define ITImp(T) \
G4ITType T::fType = G4ITTypeManager::Instance()->NewType();

#endif // G4ITTYPE_HH