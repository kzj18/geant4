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
//
// 20100804  M. Kelsey -- Add name string to ctor
// 20110719  M. Kelsey -- Add initial state code to ctor
// 20110725  M. Kelsey -- Instantiate cross-section object for self-registration
// 20110916  M. Kelsey -- Drop self-registration due to platform inconsistencies

#include "G4CascadePiMinusPChannel.hh"
#include "G4CascadePiPlusNChannel.hh"
#include "G4InuclParticleNames.hh"
using namespace G4InuclParticleNames;

namespace {
  // pi- p : Outgoing particle types of a given multiplicity
  static G4int pimP2bfs[5][2] =
  {{pim,pro}, {pi0,neu}, {k0,lam}, {k0,s0}, {kpl,sm}};

  static G4int pimP3bfs[13][3] =
  {{pim,pro,pi0}, {pim,neu,pip}, {pi0,neu,pi0}, {pi0,lam,k0}, 
    {pim,lam,kpl},  {pip,sm,k0},   {pi0,sm,kpl},   {pim,sp,k0},
    {pim,s0,kpl},   {pi0,s0,k0},   {kmi,pro,k0},   {kmi,neu,kpl},
    {k0b,neu,k0}};

  static G4int pimP4bfs[22][4] =
  {{pim,pro,pip,pim}, {pim,pro,pi0,pi0}, {pim,neu,pip,pi0},
    {pi0,neu,pi0,pi0}, {pim,lam,k0,pip},  {pi0,lam,k0,pi0},
    {pim,lam,kpl,pi0},  {pim,s0,k0,pip},   {pi0,s0,k0,pi0},
    {pim,s0,kpl,pi0},   {pim,sp,kpl,pim},   {pim,sp,k0,pi0},
    {pim,sm,kpl,pip},   {pi0,sm,kpl,pi0},   {pip,sm,k0,pi0},
    {pim,pro,kpl,kmi},   {pim,pro,k0,k0b},  {pi0,pro,k0,kmi},
    {pip,neu,k0,kmi},   {pi0,neu,k0,k0b},  {pi0,neu,kpl,kmi},
    {pim,neu,kpl,k0b}};

  static G4int pimP5bfs[31][5] =
  {{pim,pro,pip,pim,pi0}, {pim,pro,pi0,pi0,pi0}, {pim,neu,pip,pip,pim},
    {pim,neu,pip,pi0,pi0}, {pi0,neu,pi0,pi0,pi0}, {pim,lam,k0,pip,pi0},
    {pim,lam,kpl,pi0,pi0},  {pim,lam,kpl,pip,pim},  {pi0,lam,k0,pi0,pi0},
    {pim,s0,kpl,pip,pim},   {pim,s0,kpl,pi0,pi0},   {pim,s0,k0,pip,pi0},
    {pi0,s0,k0,pi0,pi0},   {pim,sp,k0,pip,pim},   {pim,sp,k0,pi0,pi0},
    {pim,sp,kpl,pim,pi0},   {pim,sm,k0,pip,pip},   {pip,sm,k0,pi0,pi0},
    {pim,sm,kpl,pip,pi0},   {pi0,sm,kpl,pi0,pi0},   {pim,pro,pi0,kpl,kmi},
    {pim,pro,pi0,k0,k0b},  {pim,pro,pip,k0,kmi},   {pi0,pro,pi0,k0,kmi},
    {pim,pro,pim,kpl,k0b},  {pim,neu,pip,kpl,kmi},   {pim,neu,pip,k0,k0b},
    {pip,neu,pi0,k0,kmi},   {pim,neu,pi0,kpl,k0b},  {pi0,neu,pi0,k0,k0b},
    {pi0,neu,pi0,kpl,kmi}}; 

  static G4int pimP6bfs[6][6] =
  {{pim,pro,pip,pip,pim,pim}, {pim,pro,pip,pim,pi0,pi0},
    {pim,pro,pi0,pi0,pi0,pi0}, {pim,neu,pip,pip,pim,pi0},
    {pim,neu,pip,pi0,pi0,pi0}, {pi0,neu,pi0,pi0,pi0,pi0}};

  static G4int pimP7bfs[7][7] =
  {{pim,pro,pip,pip,pim,pim,pi0}, {pim,pro,pip,pim,pi0,pi0,pi0},
    {pim,pro,pi0,pi0,pi0,pi0,pi0}, {pim,neu,pip,pip,pip,pim,pim},
    {pim,neu,pip,pip,pim,pi0,pi0}, {pim,neu,pip,pi0,pi0,pi0,pi0},
    {pi0,neu,pi0,pi0,pi0,pi0,pi0}};

  static G4int pimP8bfs[8][8] =
  {{pim,pro,pip,pip,pip,pim,pim,pim}, {pim,pro,pip,pip,pim,pim,pi0,pi0},
    {pim,pro,pip,pim,pi0,pi0,pi0,pi0}, {pim,pro,pi0,pi0,pi0,pi0,pi0,pi0},
    {pi0,neu,pi0,pi0,pi0,pi0,pi0,pi0}, {pim,neu,pip,pi0,pi0,pi0,pi0,pi0},
    {pim,neu,pip,pip,pim,pi0,pi0,pi0}, {pim,neu,pip,pip,pip,pim,pim,pi0}};

  static G4int pimP9bfs[9][9] =
  {{pim,pro,pip,pip,pip,pim,pim,pim,pi0}, {pim,pro,pip,pip,pim,pim,pi0,pi0,pi0},
    {pim,pro,pip,pim,pi0,pi0,pi0,pi0,pi0}, {pim,pro,pi0,pi0,pi0,pi0,pi0,pi0,pi0},
    {pi0,neu,pi0,pi0,pi0,pi0,pi0,pi0,pi0}, {pim,neu,pip,pi0,pi0,pi0,pi0,pi0,pi0},
    {pim,neu,pip,pip,pim,pi0,pi0,pi0,pi0}, {pim,neu,pip,pip,pip,pim,pim,pi0,pi0},
    {pim,neu,pip,pip,pip,pip,pim,pim,pim}};
}

namespace {
  // pi+ n : Outgoing particle types of a given multiplicity
  static G4int pipN2bfs[5][2] =
   {{pip,neu}, {pi0,pro}, {kpl,lam}, {kpl,s0}, {k0,sp}};

  static G4int pipN3bfs[13][3] =
   {{pip,neu,pi0}, {pip,pro,pim}, {pi0,pro,pi0}, {pi0,lam,kpl},
    {pip,lam,k0},  {pim,sp,kpl},   {pi0,sp,k0},   {pip,sm,kpl},
    {pip,s0,k0},   {pi0,s0,kpl},   {k0b,neu,kpl},  {k0b,pro,k0},
    {kmi,pro,kpl}};

  static G4int pipN4bfs[22][4] =
   {{pip,neu,pip,pim},  {pip,neu,pi0,pi0}, {pip,pro,pim,pi0},
    {pi0,pro,pi0,pi0},  {pip,lam,kpl,pim},  {pi0,lam,kpl,pi0},
    {pip,lam,k0,pi0},   {pip,s0,kpl,pim},   {pi0,s0,kpl,pi0},
    {pip,s0,k0,pi0},    {pip,sm,k0,pip},   {pip,sm,kpl,pi0},
    {pip,sp,k0,pim},    {pi0,sp,k0,pi0},   {pim,sp,kpl,pi0},
    {pip,neu,k0,k0b},   {pip,neu,kpl,kmi},   {pi0,neu,kpl,k0b},
    {pim,pro,kpl,k0b},   {pi0,pro,kpl,kmi},   {pi0,pro,k0,k0b},
    {pip,pro,k0,kmi}};

  static G4int pipN5bfs[31][5] =
   {{pip,neu,pip,pim,pi0}, {pip,neu,pi0,pi0,pi0}, {pip,pro,pip,pim,pim},
    {pip,pro,pim,pi0,pi0}, {pi0,pro,pi0,pi0,pi0}, {pip,lam,kpl,pim,pi0},
    {pip,lam,k0,pi0,pi0},  {pip,lam,k0,pip,pim},  {pi0,lam,kpl,pi0,pi0},
    {pip,s0,k0,pip,pim},   {pip,s0,k0,pi0,pi0},   {pip,s0,kpl,pim,pi0},
    {pi0,s0,kpl,pi0,pi0},   {pip,sm,kpl,pip,pim},   {pip,sm,kpl,pi0,pi0},
    {pip,sm,k0,pip,pi0},   {pip,sp,kpl,pim,pim},   {pim,sp,kpl,pi0,pi0},
    {pip,sp,k0,pim,pi0},   {pi0,sp,k0,pi0,pi0},   {pip,neu,pi0,k0,k0b},
    {pip,neu,pi0,kpl,kmi},   {pip,neu,pim,kpl,k0b},  {pi0,neu,pi0,kpl,k0b},
    {pip,neu,pip,k0,kmi},   {pip,pro,pim,k0,k0b},  {pip,pro,pim,kpl,kmi},
    {pim,pro,pi0,kpl,k0b},  {pip,pro,pi0,k0,kmi},   {pi0,pro,pi0,kpl,kmi},
    {pi0,pro,pi0,k0,k0b}};

  static G4int pipN6bfs[6][6] =
   {{pip,neu,pip,pip,pim,pim}, {pip,neu,pip,pim,pi0,pi0},
    {pip,neu,pi0,pi0,pi0,pi0}, {pip,pro,pip,pim,pim,pi0},
    {pip,pro,pim,pi0,pi0,pi0}, {pi0,pro,pi0,pi0,pi0,pi0}};

  static G4int pipN7bfs[7][7] =
   {{pip,neu,pip,pip,pim,pim,pi0}, {pip,neu,pip,pim,pi0,pi0,pi0},
    {pip,neu,pi0,pi0,pi0,pi0,pi0}, {pip,pro,pip,pip,pim,pim,pim},
    {pip,pro,pip,pim,pim,pi0,pi0}, {pip,pro,pim,pi0,pi0,pi0,pi0},
    {pi0,pro,pi0,pi0,pi0,pi0,pi0}};

  static G4int pipN8bfs[8][8] =
   {{pip,neu,pip,pip,pip,pim,pim,pim}, {pip,neu,pip,pip,pim,pim,pi0,pi0},
    {pip,neu,pip,pim,pi0,pi0,pi0,pi0}, {pip,neu,pi0,pi0,pi0,pi0,pi0,pi0},
    {pi0,pro,pi0,pi0,pi0,pi0,pi0,pi0}, {pip,pro,pim,pi0,pi0,pi0,pi0,pi0},
    {pip,pro,pip,pim,pim,pi0,pi0,pi0}, {pip,pro,pip,pip,pim,pim,pim,pi0}};

  static G4int pipN9bfs[9][9] =
   {{pip,neu,pip,pip,pip,pim,pim,pim,pi0}, {pip,neu,pip,pip,pim,pim,pi0,pi0,pi0},
    {pip,neu,pip,pim,pi0,pi0,pi0,pi0,pi0}, {pip,neu,pi0,pi0,pi0,pi0,pi0,pi0,pi0},
    {pi0,pro,pi0,pi0,pi0,pi0,pi0,pi0,pi0}, {pip,pro,pim,pi0,pi0,pi0,pi0,pi0,pi0},
    {pip,pro,pip,pim,pim,pi0,pi0,pi0,pi0}, {pip,pro,pip,pip,pim,pim,pim,pi0,pi0},
    {pip,pro,pip,pip,pip,pim,pim,pim,pim}};
}

namespace {
  // Total pi- p cross section as a function of kinetic energy
  // New cs values after 9B tuning (27 July 09)
  static G4double pimPtotXSec[30] = 
   { 6.13,  6.4,   6.67,  6.94,  7.22,  7.5,   8.3,  12.0,  14.4,  24.0,
    46.0,  72.04, 43.02, 27.19, 27.32, 43.8,  37.08, 51.37, 34.21, 34.79,
    32.08, 31.19, 30.32, 28.5,  27.0,  25.9,  25.5,  25.2,  25.0,  24.8};

  // pi- p cross sections as functions of kinetic energy and multiplicity
  G4double pimPCrossSections[101][30] = {
  //
  // multiplicity 2 (5 channels)
  //
  //  pi- p (pi+ n)
   { 1.43, 1.5,  1.57, 1.64, 1.72,  1.8,   2.0,   3.0,  3.4,  7.0,
    14.0, 24.0, 14.7, 10.5, 10.84, 21.0,  13.97, 25.1, 10.46, 9.88,
     8.0,  7.1,  6.0,  5.7,  5.0,   4.6,   4.3,   4.0,  3.8,  3.7},

  //  n pi0  (p pi0)
   { 4.7,  4.9,  5.1,  5.3,  5.5,  5.7,  6.3,  9.0, 11.0, 17.0,
    32.0, 48.0, 28.0, 14.5, 11.04, 8.99, 4.79, 5.02, 2.08, 0.94,
     0.5,  0.25, 0.15, 0.09, 0.06, 0.05, 0.04, 0.02, 0.02, 0.01},

  //  L K0  (L K+)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.65, 0.29, 0.17,
     0.13, 0.08, 0.05, 0.03, 0.02, 0.01, 0.01, 0.01, 0.01, 0.0},

  //  S0 K0  (S0 K+)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.28, 0.19, 0.12,
     0.09, 0.06, 0.04, 0.03, 0.02, 0.01, 0.01, 0.0,  0.0,  0.0},

  //  S- K+  (S+ K0)
   { 0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0,  0.20, 0.25, 0.09,
     0.04, 0.01, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
  //
  // multiplicity 3 (13 channels)
  //
  //  p pi- pi0  (n pi+ pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.05, 0.18, 0.86, 3.89, 5.19, 6.63, 5.11, 4.03,
     3.2,  2.5,  2.0,  1.4,  0.97, 0.68, 0.55, 0.36, 0.3,  0.22},

  //  n pi+ pi-  (p pi+ pi-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.03, 0.19, 1.41, 3.12, 6.2,  8.14, 8.23, 7.84, 5.91,
     4.58, 3.58, 2.71, 1.73, 1.06, 0.85, 0.62, 0.38, 0.31, 0.22},

  //  n pi0 pi0  (p pi0 pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.01, 0.08, 0.6,  1.34, 2.7,  2.99, 2.51, 1.35, 0.87,
     0.61, 0.24, 0.15, 0.10, 0.06, 0.04, 0.0,  0.0,  0.0,  0.0},

  //  L K0 pi0  (L K+ pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.05, 0.19,
     0.14, 0.09, 0.07, 0.05, 0.03, 0.02, 0.02, 0.01, 0.01, 0.0},
 
  //  L K+ pi-  (L K0 pi+)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.04, 0.12,
     0.13, 0.11, 0.08, 0.06, 0.04, 0.02, 0.02, 0.01, 0.01, 0.0},

  //  S- K0 pi+  (S+ K+ pi-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.1,
     0.13, 0.07, 0.03, 0.02, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0},

  //  S- K+ pi0  (S+ K0 pi0)
  {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.05,
     0.05, 0.03, 0.02, 0.01, 0.01, 0.0, 0.0, 0.0,  0.0,  0.0},

  //  S+ K0 pi-  (S- K+ pi+)
  { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.0,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.04,
    0.06, 0.04, 0.02, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0, 0.0},

  //  S0 K+ pi-  (S0 K0 pi+)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.04,
     0.07, 0.04, 0.03, 0.02, 0.02, 0.01, 0.0, 0.0, 0.0, 0.0},

  //  S0 K0 pi0  (S0 K+ pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.02, 0.09,
     0.07, 0.05, 0.03, 0.02, 0.02, 0.01, 0.01, 0.0, 0.0,  0.0},

  //  p K0 K-  (n K+ K0bar)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.03,
     0.08, 0.07, 0.05, 0.04, 0.03, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  n K+ K-  (p K0 K0bar)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.04,
     0.11, 0.28, 0.12, 0.07, 0.04, 0.02, 0.01, 0.0,  0.0,  0.0},

  //  n K0 K0bar  (p K+ K-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.06,
     0.10, 0.15, 0.18, 0.10, 0.05, 0.02, 0.01, 0.0,  0.0,  0.0},
  //
  // multiplicity 4 (22 channels)
  //
  //  p pi+ pi- pi-  (n pi+ pi+ pi-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.01, 0.04, 0.18, 0.43, 0.82, 1.69,
     1.8,  1.99, 1.8,  1.71, 1.44, 1.26, 1.17, 0.99, 1.04, 0.9},

  //  p pi- pi0 pi0  (n pi+ pi0 pi0)
   { 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0, 0.01, 0.03, 0.09, 0.21, 1.5,  2.5,
     1.2,  1.2,  1.0,  0.9, 0.8,  0.7,  0.65, 0.6,  0.55, 0.5},

  //  n pi+ pi- pi0  (p pi+ pi- pi0)
   { 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0, 0.0,  0.0,  0.0,  0.4,  0.7,  1.11, 2.6,  3.9,
     4.07, 3.8, 2.76, 1.38, 1.16, 0.97, 0.85, 0.7,  0.55, 0.4},

  //  n pi0 pi0 pi0  (p pi0 pi0 pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.1,  0.55, 1.0, 0.87, 0.73, 0.71,
     0.7,  0.56, 0.35, 0.29, 0.21, 0.14, 0.1, 0.06, 0.03, 0.0},

  //  L K0 pi+ pi-  (L K+ pi+ pi-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.08, 0.12, 0.09, 0.08, 0.07, 0.05, 0.05, 0.04, 0.03, 0.03},

  //  L K0 pi0 pi0  (L K+ pi0 pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.08, 0.12, 0.09, 0.08, 0.07, 0.05, 0.05, 0.04, 0.03, 0.03},

  //  L K+ pi- pi0  (L K0 pi+ pi0)
   { 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.06, 0.1, 0.09, 0.07, 0.06, 0.05, 0.04, 0.03, 0.02, 0.02},

  //  S0 K0 pi+ pi-  (S0 K+ pi+ pi-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.04, 0.06, 0.05, 0.04, 0.04, 0.03, 0.03, 0.02, 0.01, 0.01},

  //  S0 K0 pi0 pi0  (S0 K+ pi0 pi0)
  {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.04, 0.06, 0.05, 0.04, 0.04, 0.03, 0.03, 0.02, 0.01, 0.01},

  //  S0 K+ pi- pi0  (S0 K0 pi+ pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.03, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01},

  //  S+ K+ pi- pi-  (S- K0 pi+ pi+)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.03, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01},

  //  S+ K0 pi- pi0  (S- K+ pi+ pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.03, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01},

  //  S- K+ pi+ pi-  (S+ K0 pi+ pi-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.03, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01},

  //  S- K+ pi0 pi0  (S+ K0 pi0 pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.03, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01},

  //  S- K0 pi+ pi0  (S+ K+ pi- pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
     0.03, 0.05, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.01, 0.01},

  //  p pi- K+ K-  (n pi+ K0 K0bar)
   { 0.0, 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.06, 0.1, 0.09, 0.08, 0.07, 0.07, 0.06, 0.06, 0.05},

  //  p pi- K0 K0bar  (n pi+ K+ K-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.03, 0.05, 0.07, 0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05},

  //  p pi0 K0 K-  (n pi0 K+ K0bar)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.03, 0.05, 0.07, 0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05},

  //  n pi+ K0 K-  (p pi- K+ K0bar)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.03, 0.05, 0.07, 0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05},

  //  n pi0 K0 K0bar  (p pi0 K+ K-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.03, 0.05, 0.07, 0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05},

  //  n pi0 K+ K-  (p pi0 K0 K0bar)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.03, 0.05, 0.07, 0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05},

  //  n pi- K+ K0bar  (p pi+ K0 K-)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.03, 0.05, 0.07, 0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05},
  //
  // multiplicity 5 (31 channels)
  //
  //  p pi+ pi- pi- pi0  (n pi+ pi+ pi- pi0)
   { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.02, 0.10, 0.93,
     1.5,  1.9,  2.2,  2.0,  1.7,  1.4,  1.2,  0.90, 0.76, 0.62},

  //  p pi- pi0 pi0 pi0  (n pi+ pi0 pi0 pi0)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.02, 0.10, 0.73,
      1.4,  1.9,  2.2,  2.0,  1.7,  1.4,  1.2,  0.90, 0.76, 0.62},

  //  n pi+ pi+ pi- pi-  (p pi+ pi+ pi- pi-)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.01, 0.03, 0.10, 0.31,
      0.66, 0.93, 1.2,  1.2,  1.2, 0.94, 0.74, 0.53, 0.40, 0.30},

  //  n pi+ pi- pi0 pi0  (p pi+ pi- pi0 pi0)
   {  0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0,  0.01, 0.22, 0.4,
      0.8,  0.95, 1.0, 0.9, 0.75, 0.65, 0.55, 0.40, 0.35, 0.30},

  //  n pi0 pi0 pi0 pi0  (p pi0 pi0 pi0 pi0)
   {  0.0,  0.0, 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0, 0.0, 0.0, 0.0, 0.0,  0.0,  0.01, 0.22, 0.39,
      0.66, 0.9, 1.0, 0.8, 0.7, 0.55, 0.5,  0.35, 0.30, 0.25},

  //  L K0 pi+ pi- pi0  (L K+ pi+ pi- pi0)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.01, 0.05, 0.07, 0.08, 0.08, 0.07, 0.06, 0.06, 0.05, 0.05},

  //  L K+ pi- pi0 pi0  (L K0 pi+ pi0 pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.02, 0.03, 0.04, 0.04, 0.03, 0.03, 0.03, 0.02, 0.02},

  //  L K+ pi+ pi- pi-  (L K0 pi+ pi+ pi-)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.03, 0.04, 0.04, 0.03, 0.03, 0.03, 0.02, 0.02},

  //  L K0 pi0 pi0 pi0  (L K+ pi0 pi0 pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.02, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  S0 K+ pi+ pi- pi-  (S0 K0 pi+ pi+ pi-)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0},

  //  S0 K+ pi- pi0 pi0  (S0 K0 pi+ pi0 pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0},

  //  S0 K0 pi+ pi- pi0  (S0 K+ pi+ pi- pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.02, 0.03, 0.04, 0.04, 0.04, 0.03, 0.03, 0.02, 0.02},

  //  S0 K0 pi0 pi0 pi0  (S0 K+ pi0 pi0 pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.01, 0.02, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  S+ K0 pi+ pi- pi-  (S- K+ pi+ pi+ pi-)
   {  0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0},

  //  S+ K0 pi- pi0 pi0  (S- K+ pi+ pi0 pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0},

  //  S+ K+ pi- pi- pi0  (S- K0 pi+ pi+ pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,
      0.0, 0.0, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0},

  //  S- K0 pi+ pi+ pi-  (S+ K+ pi+ pi- pi-)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,
      0.0, 0.01, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0},

  //  S- K0 pi+ pi0 pi0  (S+ K+ pi- pi0 pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.01, 0.01, 0.01, 0.01, 0.0, 0.0, 0.0, 0.0},

  //  S- K+ pi+ pi- pi0  (S+ K0 pi+ pi- pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.03, 0.02, 0.02, 0.01, 0.01, 0.01, 0.01},

  //  S- K+ pi0 pi0 pi0  (S+ K0 pi0 pi0 pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0},

  //  p pi- pi0 K+ K-  (n pi+ pi0 K0 K0bar)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.07, 0.07, 0.06, 0.06, 0.05, 0.05, 0.04, 0.04},

  //  p pi- pi0 K0 K0bar  (n pi+ pi0 K+ K-)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.02, 0.02, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  p pi+ pi- K0 K-  (n pi+ pi- K+ K0bar)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.02, 0.02, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  p pi0 pi0 K0 K-  (n pi0 pi0 K+ K0bar)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.02, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01},

  //  p pi- pi- K+ K0bar  (n pi+ pi+ K0 K-)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.04, 0.05, 0.04, 0.04, 0.03, 0.03, 0.03, 0.02},

  //  n pi+ pi- K+ K-  (p pi+ pi- K0 K0bar)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.07, 0.06, 0.05, 0.04, 0.04, 0.03, 0.03, 0.03},

  //  n pi+ pi- K0 K0bar  (p pi+ pi- K+ K-)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.02, 0.01},

  //  n pi+ pi0 K0 K-  (p pi- pi0 K+ K0bar)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.07, 0.06, 0.05, 0.04, 0.04, 0.04, 0.03, 0.03},

  //  n pi- pi0 K+ K0bar  (p pi+ pi0 K0 K-)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.07, 0.06, 0.05, 0.04, 0.04, 0.04, 0.03, 0.03},

  //  n pi0 pi0 K0 K0bar  (p pi0 pi0 K+ K-)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.02, 0.01},

  //  n pi0 pi0 K+ K-  (p pi0 pi0 K0 K0bar)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.02, 0.01},
  //
  // multiplicity 6 (6 channels)
  //
  //  p pi+ pi+ pi- pi- pi-  (n pi+ pi+ pi+ pi- pi-)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.01, 0.03, 0.04,
      0.06, 0.12, 0.16, 0.22, 0.31, 0.34, 0.3, 0.24, 0.19, 0.16},

  //  p pi+ pi- pi- pi0 pi0  (n pi+ pi+ pi- pi0 pi0)
   {  0.0,  0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,
      0.0,  0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.01, 0.02, 0.05,
      0.07, 0.1, 0.12, 0.15, 0.18, 0.2, 0.19, 0.2,  0.17, 0.16},

  //  p pi- pi0 pi0 pi0 pi0  (n pi+ pi0 pi0 pi0 pi0)
   {  0.0,  0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,
      0.0,  0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.01, 0.02, 0.05,
      0.07, 0.1, 0.12, 0.15, 0.18, 0.2, 0.19, 0.2,  0.17, 0.16},

  //  n pi+ pi+ pi- pi- pi0  (p pi+ pi+ pi- pi- pi0)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.01, 0.02, 0.05,
      0.07, 0.1,  0.12, 0.15, 0.18, 0.2, 0.19, 0.2,  0.17, 0.16},

  //  n pi+ pi- pi0 pi0 pi0  (p pi+ pi- pi0 pi0 pi0)
   {  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.01, 0.03,
      0.05, 0.06, 0.08, 0.1, 0.12, 0.13, 0.14, 0.13, 0.11, 0.11},

  //  n pi0 pi0 pi0 pi0 pi0  (p pi0 pi0 pi0 pi0 pi0)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0, 0.0, 0.01, 0.03,
      0.04, 0.05, 0.06, 0.08, 0.09, 0.1, 0.1, 0.1, 0.08, 0.07},
  //
  // multiplicity 7 (7 channels)
  //
  //  p pi+ pi+ pi- pi- pi- pi0
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.03,
      0.05, 0.12, 0.19, 0.37, 0.67, 0.70, 0.65, 0.60, 0.50, 0.45},

  //  p pi+ pi- pi- pi0 pi0 pi0
   {  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.01, 0.02,
      0.03, 0.07, 0.11, 0.22, 0.4, 0.4,  0.35, 0.30, 0.25, 0.2},

  //  p pi- pi0 pi0 pi0 pi0 pi0
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  0.01,
      0.02, 0.03, 0.05, 0.11, 0.20, 0.2, 0.15, 0.12, 0.10, 0.08},

  //  n pi+ pi+ pi+ pi- pi- pi-   (measured R 501)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
      0.01, 0.03, 0.06, 0.15, 0.29, 0.29, 0.24, 0.18, 0.16, 0.13},

  //  n pi+ pi+ pi- pi- pi0 pi0   (measured R 496)
   {  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0, 0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0,  0.0, 0.01, 0.02,
      0.03, 0.07, 0.11, 0.22, 0.4, 0.45, 0.4,  0.4, 0.3,  0.3},

  //  n pi+ pi- pi0 pi0 pi0 pi0
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01,
      0.02, 0.04, 0.07, 0.14, 0.26, 0.26, 0.22, 0.20, 0.18, 0.15},

  //  n pi0 pi0 pi0 pi0 pi0 pi0
   {  0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0, 0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0, 0.0,  0.0, 0.0, 0.0,  0.0,
      0.01, 0.02, 0.03, 0.05, 0.1, 0.11, 0.1, 0.1, 0.08, 0.7},
  //
  // multiplicity 8 (8 channels)
  //
  //  p pi+ pi+ pi+ pi- pi- pi- pi-  (n pi+ pi+ pi+ pi+ pi- pi- pi-) (measured R 420)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.06, 0.09, 0.11, 0.11, 0.10, 0.10, 0.09},

  //  p pi+ pi+ pi- pi- pi- pi0 pi0  (n pi+ pi+ pi+ pi- pi- pi0 pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.03, 0.09, 0.13, 0.13, 0.11, 0.10, 0.09, 0.08},

  //  p pi+ pi- pi- pi0 pi0 pi0 pi0  (n pi+ pi+ pi- pi0 pi0 pi0 pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.06, 0.09, 0.11, 0.11, 0.10, 0.10, 0.09},

  //  p pi- pi0 pi0 pi0 pi0 pi0 pi0  (n pi+ pi0 pi0 pi0 pi0 pi0 pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.01, 0.02, 0.03, 0.04, 0.04, 0.03, 0.03, 0.03},

  //  n pi0 pi0 pi0 pi0 pi0 pi0 pi0  (p pi0 pi0 pi0 pi0 pi0 pi0 pi0)
   {  0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0, 0.01, 0.02, 0.03, 0.03, 0.03, 0.03, 0.02},

  //  n pi+ pi- pi0 pi0 pi0 pi0 pi0  (p pi+ pi- pi0 pi0 pi0 pi0 pi0)
   {  0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0, 0.01, 0.03, 0.04, 0.05, 0.05, 0.05, 0.05, 0.04},

  //  n pi+ pi+ pi- pi- pi0 pi0 pi0  (p pi+ pi+ pi- pi- pi0 pi0 pi0)
   {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.02, 0.06, 0.09, 0.11, 0.11, 0.10, 0.10, 0.09 },

  //  n pi+ pi+ pi+ pi- pi- pi- pi0  (p pi+ pi+ pi+ pi- pi- pi- pi0)
   {  0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0, 0.01, 0.03, 0.09, 0.12, 0.13, 0.11, 0.09, 0.08, 0.07},
  //
  // multiplicity 9 (9 channels)
  //
  //  p pi+ pi+ pi+ pi- pi- pi- pi- pi0   (measured R 418)
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.04, 0.08, 0.14, 0.18, 0.18, 0.18, 0.17},

  //  p pi+ pi+ pi- pi- pi- pi0 pi0 pi0
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.02, 0.05, 0.08, 0.11, 0.11, 0.11, 0.1},

  //  p pi+ pi- pi- pi0 pi0 pi0 pi0 pi0
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.02, 0.03, 0.06, 0.07, 0.07, 0.07, 0.07},

  //  p pi- pi0 pi0 pi0 pi0 pi0 pi0 pi0
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.01, 0.02, 0.03, 0.04, 0.04, 0.04, 0.04},

  //  n pi0 pi0 pi0 pi0 pi0 pi0 pi0 pi0 
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.01, 0.02, 0.03, 0.04, 0.04, 0.04, 0.04},

  //  n pi+ pi- pi0 pi0 pi0 pi0 pi0 pi0
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.01, 0.02, 0.03, 0.04, 0.04, 0.04, 0.04},

  //  n pi+ pi+ pi- pi- pi0 pi0 pi0 pi0
   { 0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0, 0.02, 0.03, 0.06, 0.07, 0.07, 0.07, 0.07},

  //  n pi+ pi+ pi+ pi- pi- pi- pi0 pi0
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.02, 0.05, 0.08, 0.11, 0.11, 0.11, 0.1},

  //  n pi+ pi+ pi+ pi+ pi- pi- pi- pi-   (measured R 503)
   { 0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
     0.0, 0.0, 0.01, 0.04, 0.08, 0.14, 0.18, 0.18, 0.18, 0.17} };
 }

// Initialize both |T Tz> = |3/2 1/2> channels, using pipP cross-section table

typedef G4CascadePiMinusPChannelData::data_t G4CascadePiMinusPChannelData_t;

G4CascadePiMinusPChannelData_t *G4CascadePiMinusPChannelData::data = 0;

G4CascadePiMinusPChannelData::data_t *G4CascadePiMinusPChannelData::initializer()
{
  if (G4CascadePiMinusPChannelData::data == 0)
    G4CascadePiMinusPChannelData::data = new G4CascadePiMinusPChannelData::data_t(pimP2bfs, pimP3bfs, pimP4bfs, pimP5bfs, pimP6bfs, pimP7bfs, pimP8bfs, pimP9bfs, pimPCrossSections, pimPtotXSec, pim*pro, "PiMinusP");

  return G4CascadePiMinusPChannelData::data;
}

typedef G4CascadePiPlusNChannelData::data_t G4CascadePiPlusNChannelData_t;

G4CascadePiPlusNChannelData_t *G4CascadePiPlusNChannelData::data = 0;

G4CascadePiPlusNChannelData::data_t *G4CascadePiPlusNChannelData::initializer()
{
  if (G4CascadePiPlusNChannelData::data == 0)
    G4CascadePiPlusNChannelData::data = new G4CascadePiPlusNChannelData::data_t(pipN2bfs, pipN3bfs, pipN4bfs, pipN5bfs, pipN6bfs, pipN7bfs, pipN8bfs, pipN9bfs, pimPCrossSections, pimPtotXSec, pip*neu, "PiPlusN");

  return G4CascadePiPlusNChannelData::data;
}
