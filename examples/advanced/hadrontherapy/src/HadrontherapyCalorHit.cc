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
// $Id: HadrontherapyCalorHit.cc,v 1.0
// --------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// --------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone, G. Russo
// Laboratori Nazionali del Sud - INFN, Catania, Italy
//
// --------------------------------------------------------------
#include "HadrontherapyCalorHit.hh"
G4Allocator<HadrontherapyCalorHit> HadrontherapyCalorHitAllocator;

// -------------------------------------------------------------
HadrontherapyCalorHit::HadrontherapyCalorHit()
{
  EdepAbs = 0.; TrackLengthAbs = 0.;
}

// --------------------------------------------------------------
HadrontherapyCalorHit::~HadrontherapyCalorHit()
{ }

// ----------------------------------------------------------------
HadrontherapyCalorHit::HadrontherapyCalorHit(const HadrontherapyCalorHit& right):G4VHit()
{
  EdepAbs = right.EdepAbs; 
}

// -----------------------------------------------------------------
const HadrontherapyCalorHit& HadrontherapyCalorHit::operator=(const HadrontherapyCalorHit& right)
{
  EdepAbs = right.EdepAbs;
  return *this;
}

// ---------------------------------------------------------------
void HadrontherapyCalorHit::Print()
{ }


