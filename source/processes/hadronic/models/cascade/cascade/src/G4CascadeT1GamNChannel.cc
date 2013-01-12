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
// $Id: $
// GEANT4 tag: $Name: not supported by cvs2svn $
//

#include "G4CascadeGamPChannel.hh"
#include "G4CascadeGamNChannel.hh"
#include "G4InuclParticleNames.hh"
using namespace G4InuclParticleNames;

namespace {
  // gamma p : Outgoing particle types of a given multiplicity
  static G4int gamp2bfs[6][2] =
  {{gam,pro}, {pro,pi0}, {neu,pip}, {lam,kpl}, {s0, kpl}, {sp,k0}};

  static G4int gamp3bfs[6][3] =
  {{pro,pip,pim}, {pro,pi0,pi0}, {neu,pip,pi0},
   {pro,kpl,kmi},  {pro,k0,k0b},  {neu,kpl,k0b}};

  static G4int gamp4bfs[4][4] =
  {{pro,pip,pim,pi0}, {pro,pi0,pi0,pi0}, {neu,pip,pip,pim}, {neu,pip,pi0,pi0}};

  static G4int gamp5bfs[5][5] =
  {{pro,pip,pip,pim,pim}, {pro,pip,pim,pi0,pi0}, {pro,pi0,pi0,pi0,pi0},
   {neu,pip,pip,pim,pi0}, {neu,pip,pi0,pi0,pi0}};

  static G4int gamp6bfs[6][6] =
  {{pro,pip,pip,pim,pim,pi0}, {pro,pip,pim,pi0,pi0,pi0}, {pro,pi0,pi0,pi0,pi0,pi0},
   {neu,pip,pip,pip,pim,pim}, {neu,pip,pip,pim,pi0,pi0}, {neu,pip,pi0,pi0,pi0,pi0}};

  static G4int gamp7bfs[7][7] =
  {{pro,pip,pip,pip,pim,pim,pim}, {pro,pip,pip,pim,pim,pi0,pi0},
   {pro,pip,pim,pi0,pi0,pi0,pi0}, {pro,pi0,pi0,pi0,pi0,pi0,pi0},
   {neu,pip,pip,pim,pi0,pi0,pi0}, {neu,pip,pip,pip,pim,pim,pi0},
   {neu,pip,pi0,pi0,pi0,pi0,pi0}};

  static G4int gamp8bfs[7][8] =
  {{pro,pip,pip,pim,pim,pi0,pi0,pi0}, {pro,pip,pip,pip,pim,pim,pim,pi0},
   {pro,pip,pim,pi0,pi0,pi0,pi0,pi0}, {neu,pip,pip,pip,pim,pim,pi0,pi0},
   {neu,pip,pip,pim,pi0,pi0,pi0,pi0}, {neu,pip,pip,pip,pip,pim,pim,pim},
   {neu,pip,pi0,pi0,pi0,pi0,pi0,pi0}};

  static G4int gamp9bfs[7][9] =
  {{pro,pip,pip,pip,pim,pim,pim,pi0,pi0}, {pro,pip,pip,pim,pim,pi0,pi0,pi0,pi0},
   {pro,pip,pip,pip,pip,pim,pim,pim,pim}, {pro,pip,pim,pi0,pi0,pi0,pi0,pi0,pi0},
   {neu,pip,pip,pip,pim,pim,pi0,pi0,pi0}, {neu,pip,pip,pip,pip,pim,pim,pim,pi0},
   {neu,pip,pip,pim,pi0,pi0,pi0,pi0,pi0}};
}

namespace {
  // gamma n : Outgoing particle types of a given multiplicity
  static G4int gamn2bfs[6][2] =
  {{gam,neu}, {neu,pi0}, {pro,pim}, {lam,k0}, {s0, k0}, {sm,kpl}};

  static G4int gamn3bfs[6][3] =
  {{neu,pip,pim}, {neu,pi0,pi0}, {pro,pim,pi0},
   {neu,k0,k0b},  {neu,kpl,kmi},   {pro,k0,kmi}};

  static G4int gamn4bfs[4][4] =
  {{neu,pip,pim,pi0}, {neu,pi0,pi0,pi0}, {pro,pip,pim,pim}, {pro,pim,pi0,pi0}};

  static G4int gamn5bfs[5][5] =
  {{neu,pip,pip,pim,pim}, {neu,pip,pim,pi0,pi0}, {neu,pi0,pi0,pi0,pi0},
   {pro,pip,pim,pim,pi0}, {pro,pim,pi0,pi0,pi0}};

  static G4int gamn6bfs[6][6] =
  {{neu,pip,pip,pim,pim,pi0}, {neu,pip,pim,pi0,pi0,pi0}, {neu,pi0,pi0,pi0,pi0,pi0},
   {pro,pip,pip,pim,pim,pim}, {pro,pip,pim,pim,pi0,pi0}, {pro,pim,pi0,pi0,pi0,pi0}};

  static G4int gamn7bfs[7][7] =
  {{neu,pip,pip,pip,pim,pim,pim}, {neu,pip,pip,pim,pim,pi0,pi0},
   {neu,pip,pim,pi0,pi0,pi0,pi0}, {neu,pi0,pi0,pi0,pi0,pi0,pi0},
   {pro,pip,pim,pim,pi0,pi0,pi0}, {pro,pip,pip,pim,pim,pim,pi0},
   {pro,pim,pi0,pi0,pi0,pi0,pi0}};

  static G4int gamn8bfs[7][8] =
  {{neu,pip,pip,pim,pim,pi0,pi0,pi0}, {neu,pip,pip,pip,pim,pim,pim,pi0},
   {neu,pip,pim,pi0,pi0,pi0,pi0,pi0}, {pro,pip,pip,pim,pim,pim,pi0,pi0},
   {pro,pip,pim,pim,pi0,pi0,pi0,pi0}, {pro,pip,pip,pip,pim,pim,pim,pim},
   {pro,pim,pi0,pi0,pi0,pi0,pi0,pi0}};

  static G4int gamn9bfs[7][9] =
  {{neu,pip,pip,pip,pim,pim,pim,pi0,pi0}, {neu,pip,pip,pim,pim,pi0,pi0,pi0,pi0},
   {neu,pip,pip,pip,pip,pim,pim,pim,pim}, {neu,pip,pim,pi0,pi0,pi0,pi0,pi0,pi0},
   {pro,pip,pip,pim,pim,pim,pi0,pi0,pi0}, {pro,pip,pip,pip,pim,pim,pim,pim,pi0},
   {pro,pip,pim,pim,pi0,pi0,pi0,pi0,pi0}};
}


namespace {
  // Total gamma p cross section as a function of kinetic energy
  static G4double gampTotXSec[30] =
   {0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002,
    0.0002, 0.0002, 0.0723, 0.2327, 0.54,   0.24,   0.19,   0.28,   0.21,
    0.18,   0.146,  0.138,  0.13,   0.1277, 0.124,  0.122,  0.12,   0.1185,
    0.117,  0.115,  0.115};

  static G4double gampCrossSections[48][30] = {
  //
  // multiplicity 2 (6 channels)
  //
  // gamma p elastic ( = gamma n elastic )
   {0.0002,  0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002,
    0.0002,  0.0002, 0.0003, 0.0007, 0.0027, 0.0014, 0.0010, 0.0012, 0.0005,
    0.0003,  0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0001, 0.0001, 0.0001,
    0.0001,  0.0001, 0.0001},
 
  // p pi0 ( = n pi0 )
   {0.0,   0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,   0.0,
    0.0,   0.01,  0.08,   0.3,    0.1143, 0.0226, 0.0122, 0.0106, 0.009, 0.006,
    0.004, 0.003, 0.0017, 0.0009, 0.0005, 0.0002, 0.0,    0.0,    0.0,   0.0}, 

  // n pi+ ( = p pi- )
   {0.0,   0.0,   0.0,    0.0,    0.0,    0.0,    0.0,   0.0,    0.0,   0.0,
    0.0,   0.062, 0.152,  0.2373, 0.1143, 0.093,  0.105, 0.0228, 0.011, 0.006,
    0.003, 0.003, 0.0017, 0.0009, 0.0005, 0.0001, 0.0,   0.0,    0.0,   0.0},

  // L K+ ( = L K0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,   0.0,  0.0,  0.0,    0.0,   0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,   0.0,  0.0,  0.0014, 0.002, 0.0007,
    0.0004, 0.0003, 0.0002, 0.0001, 0.0,   0.0,  0.0,  0.0,    0.0,   0.0}, 

  // S0 K+ ( = S0 K0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,  0.0,  0.0,  0.0, 0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,  0.0,  0.0,  0.0, 0.0012, 0.0006,
    0.0003, 0.0002, 0.0001, 0.0001, 0.0,  0.0,  0.0,  0.0, 0.0,    0.0},
 
  // S+ K0  ( = S- K+ )
   {0.0,    0.0,    0.0,    0.0,    0.0,   0.0,  0.0,  0.0, 0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,   0.0,  0.0,  0.0, 0.0015, 0.0006,
    0.0003, 0.0002, 0.0001, 0.0001, 0.0,   0.0,  0.0,  0.0, 0.0,    0.0},
  //
  // multiplicity 3 (6 channels)
  //
  // p pi+ pi- ( = n pi+ pi- )
   {0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,    0.0,   0.0,   0.0,
    0.0,   0.0,   0.0,   0.0,   0.004, 0.05,  0.077, 0.077,  0.061, 0.040,
    0.033, 0.026, 0.022, 0.018, 0.016, 0.015, 0.014, 0.0135, 0.013, 0.012},

  // p pi0 pi0 ( = n pi0 pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,   0.0,
    0.0,  0.0,    0.0,    0.0,    0.002, 0.008,  0.0317, 0.0318, 0.026, 0.016,
    0.01, 0.0089, 0.0076, 0.0071, 0.006, 0.0065, 0.0066, 0.007,  0.006, 0.006},

  // n pi+ pi0 ( = p pi- pi0 ) 
   {0.0,   0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,   0.0,   0.0,
    0.0,   0.0,   0.0,    0.0,    0.004,  0.0154, 0.0488, 0.057, 0.044, 0.029,
    0.017, 0.013, 0.0081, 0.0043, 0.0017, 0.0006, 0.0002, 0.0,   0.0,   0.0},

  // p K+ K- ( = n K0 K0b )
   {0.0,    0.0,   0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,   0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0004, 0.001, 0.001, 0.0008, 0.0007, 0.0006, 0.0005, 0.0003, 0.0002, 0.0001},   

  // p K0 K0b  (= n K+ K- )
   {0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0004, 0.001, 0.0009, 0.0006, 0.0007, 0.0006, 0.0005, 0.0003, 0.0002, 0.0001},  

  // n K+ K0b  (= p K0 K- )
   {0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0004, 0.001, 0.0009, 0.0006, 0.0007, 0.0006, 0.0005, 0.0003, 0.0002, 0.0001},
  //
  // multiplicity 4 (4 channels)
  //
  // p pi+ pi- pi0 ( = n pi+ pi- pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0023, 0.0049, 0.014,  0.024,
    0.025,  0.0225, 0.0196, 0.0162, 0.0125, 0.0088, 0.0059, 0.003,  0.0013, 0.0004},

  // p pi0 pi0 pi0 ( = n pi0 pi0 pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0004, 0.0008, 0.0016, 0.0025,
    0.004,  0.0024, 0.0023, 0.0021, 0.0016, 0.0014, 0.0009, 0.0005, 0.0002, 0.0001},

  // n pi+ pi+ pi- ( = p pi+ pi- pi- )
   {0.0,   0.0,   0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,   0.0,   0.0,    0.0,    0.0,   0.0,    0.0007, 0.0016, 0.004,  0.0088,
    0.011, 0.010, 0.0081, 0.0064, 0.0046, 0.003, 0.0018, 0.0008, 0.0003, 0.0001},

  // n pi+ pi0 pi0 (= p pi- pi0 pi0 )
   {0.0,    0.0,   0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,   0.0,    0.0,   0.0,    0.0,    0.0007, 0.0016, 0.003,  0.006,
    0.0106, 0.007, 0.0056, 0.005, 0.0035, 0.0025, 0.0017, 0.0008, 0.0003, 0.0001},
  //
  // multiplicity 5 (5 channels)
  //
  // p pi+ pi+ pi- pi- ( = n pi+ pi+ pi- pi- )
   {0.0,    0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0004, 0.0012,
    0.0032, 0.0049, 0.0055, 0.0052, 0.005, 0.0048, 0.0046, 0.0042, 0.0038, 0.0033},

  // p pi+ pi- pi0 pi0 ( = n pi+ pi- pi0 pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0005, 0.0016,
    0.0038, 0.0064, 0.0076, 0.0081, 0.0075, 0.008, 0.0086, 0.0084, 0.0076, 0.0066},   

  // p pi0 pi0 pi0 pi0 ( = n pi0 pi0 pi0 pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0001,
    0.0005, 0.0005, 0.0006, 0.0007, 0.0007, 0.0007, 0.0008, 0.0007, 0.0006, 0.0005},   

  // n pi+ pi+ pi- pi0 ( = p pi+ pi- pi- pi0 )
   {0.0,   0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,   0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0005, 0.0016,
    0.006, 0.0064, 0.0076, 0.0081, 0.009, 0.0093, 0.0086, 0.0084, 0.0076, 0.0066},    

  // n pi+ pi0 pi0 pi0 ( = p pi- pi0 pi0 pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0006,
    0.0013, 0.0022, 0.0026, 0.0027, 0.0025, 0.0027, 0.0028, 0.0028, 0.0026, 0.0022},
  //
  // multiplicity 6 (6 channels)
  //
  // p pi+ pi+ pi- pi- pi0 ( = n pi+ pi+ pi- pi- pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0002,
    0.0014, 0.0038, 0.0062, 0.0083, 0.0090, 0.0086, 0.0082, 0.0075, 0.0068, 0.0059},

  // p pi+ pi- pi0 pi0 pi0 ( = n pi+ pi- pi0 pi0 pi0 )
   {0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0009, 0.0017, 0.0029, 0.005, 0.0045, 0.0049, 0.0051, 0.0050, 0.0046, 0.0040},

  // p pi0 pi0 pi0 pi0 pi0 ( = n pi0 pi0 pi0 pi0 pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0001, 0.0002, 0.0002, 0.0002, 0.0003, 0.0003, 0.0002, 0.0002, 0.0002},

  // n pi+ pi+ pi+ pi- pi- ( = p pi+ pi+ pi- pi- pi- )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0001,
    0.0003, 0.0009, 0.0026, 0.0032, 0.0035, 0.0033, 0.0031, 0.0028, 0.0025, 0.0022},

  // n pi+ pi+ pi- pi0 pi0 ( = p pi+ pi- pi- pi0 pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0002,
    0.0005, 0.0017, 0.0054, 0.0073, 0.0079, 0.0085, 0.0088, 0.0084, 0.0075, 0.0066},

  // n pi+ pi0 pi0 pi0 pi0 ( = p pi- pi0 pi0 pi0 pi0 )
   {0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0001, 0.0003, 0.0009, 0.0012, 0.0013, 0.0013, 0.0014, 0.0014, 0.0012, 0.0011},
  //
  // multiplicity 7 (7 channels)
  //
  // p pi+ pi+ pi+ pi- pi- pi- ( = n pi+ pi+ pi+ pi- pi- pi- )
   {0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0001, 0.0004, 0.0006, 0.0009, 0.0009, 0.0009, 0.0009, 0.0008, 0.0008},

  // p pi+ pi+ pi- pi- pi0 pi0 ( = n pi+ pi+ pi- pi- pi0 pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,   0.0,   0.0,    0.0,   0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,   0.0,   0.0,    0.0,   0.0,    0.0,
    0.0,  0.0003, 0.0012, 0.0021, 0.004, 0.004, 0.0047, 0.005, 0.0045, 0.004},

  // p pi+ pi- pi0 pi0 pi0 pi0 ( = n pi+ pi- pi0 pi0 pi0 pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0001, 0.0004, 0.0007, 0.0013, 0.0014, 0.0016, 0.0017, 0.0015, 0.0013},

  // p pi0 pi0 pi0 pi0 pi0 pi0 ( = n pi0 pi0 pi0 pi0 pi0 pi0 )
   {0.0, 0.0, 0.0, 0.0, 0.0, 0.0,    0.0,    0.0,    0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0,    0.0,    0.0,    0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0001, 0.0001, 0.0001, 0.0, 0.0},

  // n pi+ pi+ pi- pi0 pi0 pi0 ( = p pi+ pi- pi- pi0 pi0 pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0003, 0.0008, 0.0014, 0.002, 0.0023, 0.0025, 0.0027, 0.0025, 0.0025},

  // n pi+ pi+ pi+ pi- pi- pi0 ( = p pi+ pi+ pi- pi- pi- pi0 )
   {0.0,  0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0003, 0.0008, 0.0014, 0.002, 0.0023, 0.0025, 0.0027, 0.0025, 0.0025},

  // n pi+ pi0 pi0 pi0 pi0 pi0 ( = p pi- pi0 pi0 pi0 pi0 pi0 )
   {0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0001, 0.0002, 0.0002, 0.0003, 0.0003, 0.0003, 0.0002, 0.0002},
  //
  // multiplicity 8 (7 channels)
  //
  // p pi+ pi+ pi- pi- pi0 pi0 pi0 ( = n pi+ pi+ pi- pi- pi0 pi0 pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0001, 0.0003, 0.0006, 0.0015, 0.0026, 0.0033, 0.0034, 0.0034, 0.0033},

  // p pi+ pi+ pi+ pi- pi- pi- pi0 ( = n pi+ pi+ pi+ pi- pi- pi- pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0001, 0.0002, 0.0005, 0.0011, 0.0017, 0.0023, 0.0022, 0.0022, 0.0022},

  // p pi+ pi- pi0 pi0 pi0 pi0 pi0 ( = n pi+ pi- pi0 pi0 pi0 pi0 pi0 )
   {0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0001, 0.0002, 0.0004, 0.0005, 0.0007, 0.0007, 0.0007, 0.0007},

  // p pi0 pi0 pi0 pi0 pi0 pi0 pi0 ( = n pi0 pi0 pi0 pi0 pi0 pi0 pi0 )
  // negligible 

  // n pi+ pi+ pi+ pi- pi- pi0 pi0 ( = p pi+ pi+ pi- pi- pi- pi0 pi0 )
   {0.0,  0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0,    0.0,    0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,
    0.0,  0.0001, 0.0003, 0.0006, 0.002, 0.0026, 0.0033, 0.0034, 0.0034, 0.0033},

  // n pi+ pi+ pi- pi0 pi0 pi0 pi0 ( = p pi+ pi- pi- pi0 pi0 pi0 pi0 )
   {0.0, 0.0, 0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0,    0.0,   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0001, 0.0003, 0.001, 0.0013, 0.0017, 0.0017, 0.0017, 0.0016},

  // n pi+ pi+ pi+ pi+ pi- pi- pi- ( = p pi+ pi+ pi+ pi- pi- pi- pi- )
   {0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0,  0.0001, 0.0002, 0.0004, 0.0006, 0.0005, 0.0005, 0.0005},

  // n pi+ pi0 pi0 pi0 pi0 pi0 pi0 ( = p pi- pi0 pi0 pi0 pi0 pi0 pi0 )
   {0.0, 0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0, 0.0,  0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001},
  //
  // multiplicity 9 (7 channels)
  //
  // p pi+ pi+ pi+ pi- pi- pi- pi0 pi0 ( = n pi+ pi+ pi+ pi- pi- pi- pi0 pi0 )
   {0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0002, 0.0005, 0.0013, 0.0019, 0.0023, 0.0026, 0.0026, 0.0024},

  // p pi+ pi+ pi- pi- pi0 pi0 pi0 pi0 ( = n pi+ pi+ pi- pi- pi0 pi0 pi0 pi0 )
   {0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0002, 0.0003, 0.0011, 0.0014, 0.0018, 0.0020, 0.0020, 0.0018},

  // p pi+ pi+ pi+ pi+ pi- pi- pi- pi- ( = n pi+ pi+ pi+ pi+ pi- pi- pi- pi- )
   {0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0,  0.0001, 0.0002, 0.0003, 0.0003, 0.0003, 0.0003, 0.0003},

  // p pi+ pi- pi0 pi0 pi0 pi0 pi0 pi0 ( = n pi+ pi- pi0 pi0 pi0 pi0 pi0 pi0 )
   {0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0, 0.0,  0.0001, 0.0001, 0.0002, 0.0003, 0.0002, 0.0002, 0.0002},

  // p pi0 pi0 pi0 pi0 pi0 pi0 pi0 pi0 ( = n pi0 pi0 pi0 pi0 pi0 pi0 pi0 pi0 )
  // negligible

  // n pi+ pi+ pi+ pi- pi- pi0 pi0 pi0 ( = p pi+ pi+ pi- pi- pi- pi0 pi0 pi0 )
   {0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0002, 0.0005, 0.0013, 0.0018, 0.0023, 0.0026, 0.0026, 0.0024},

  // n pi+ pi+ pi+ pi+ pi- pi- pi- pi0 ( = p pi+ pi+ pi+ pi- pi- pi- pi- pi0 )
   {0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0001, 0.0002, 0.0007, 0.0009, 0.0011, 0.0013, 0.0013, 0.0012},

  // n pi+ pi+ pi- pi0 pi0 pi0 pi0 pi0 ( = p pi+ pi- pi- pi0 pi0 pi0 pi0 pi0 )
   {0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    0.0, 0.0,  0.0001, 0.0002, 0.0004, 0.0006, 0.0008, 0.0009, 0.0009, 0.0008}};

  // n pi+ pi0 pi0 pi0 pi0 pi0 pi0 pi0 ( = p pi- pi0 pi0 pi0 pi0 pi0 pi0 pi0 )
  // negligible
}

// Initialize both |T Tz> = |1/2 +-1/2> channels using gamma-p cross section
// tables

typedef G4CascadeGamPChannelData::data_t G4CascadeGamPChannelData_t;

G4CascadeGamPChannelData_t *G4CascadeGamPChannelData::data = 0;

G4CascadeGamPChannelData::data_t *G4CascadeGamPChannelData::initializer()
{
  if (G4CascadeGamPChannelData::data == 0)
    G4CascadeGamPChannelData::data = new G4CascadeGamPChannelData::data_t(gamp2bfs, gamp3bfs, gamp4bfs, gamp5bfs, gamp6bfs, gamp7bfs, gamp8bfs, gamp9bfs, gampCrossSections, gampTotXSec, gam*pro, "GammaProton");
  return G4CascadeGamPChannelData::data;
}

typedef G4CascadeGamNChannelData::data_t G4CascadeGamNChannelData_t;

G4CascadeGamNChannelData_t *G4CascadeGamNChannelData::data = 0;

G4CascadeGamNChannelData::data_t *G4CascadeGamNChannelData::initializer()
{
  if (G4CascadeGamNChannelData::data == 0)
    G4CascadeGamNChannelData::data =new G4CascadeGamNChannelData::data_t(gamn2bfs, gamn3bfs, gamn4bfs, gamn5bfs, gamn6bfs, gamn7bfs, gamn8bfs, gamn9bfs, gampCrossSections, gampTotXSec, gam*neu, "GammaNeutron");

  return G4CascadeGamNChannelData::data;
}
