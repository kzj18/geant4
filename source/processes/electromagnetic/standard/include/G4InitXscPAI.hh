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
// $Id: G4InitXscPAI.hh,v 1.1 2004-04-09 13:36:27 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// G4InitXscPAI.hh -- header file
//
// History:
//
// 02.04.04, V. Grichine: 1st version based on G4PAIxSection class

#ifndef G4INITXSCPAI_HH
#define G4INITXSCPAI_HH

#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"

class G4MaterialCutsCouple;
class G4SandiaTable;
class G4OrderedTable;

class G4InitXscPAI
{
public:
	  // Constructors
  G4InitXscPAI( G4MaterialCutsCouple* matCC);
	  
	  
	  
  ~G4InitXscPAI() ;
	  
  // Methods
  // General control functions

  void KillCloseIntervals();

  void Normalisation();


  // Physical methods
          

  G4double RutherfordIntegral( G4int intervalNumber,
	                               G4double limitLow,
				       G4double limitHigh     ) ;

  G4double IntegralTerm(G4double omega);

  G4double ImPartDielectricConst( G4int intervalNumber,
	                                  G4double energy        ) ;

  G4double RePartDielectricConst(G4double energy) ;

  G4double DifPAIxSection( G4double omega ) ;

  G4double PAIdNdxCerenkov( G4double omega ) ;

  G4double PAIdNdxPlasmon( G4double omega ) ;

  G4double IntegralPAIxSection(G4double omega, G4double bg2, G4double Tmax) ;

  void     IntegralCerenkov() ;
  void     IntegralPlasmon() ;


  G4double GetStepEnergyLoss( G4double step ) ;
  G4double GetStepCerenkovLoss( G4double step ) ;
  G4double GetStepPlasmonLoss( G4double step ) ;
	 
  // Inline access functions

	  
  G4int GetIntervalNumber() const { return fIntervalNumber ; }

  G4double GetNormalizationCof() const { return fNormalizationCof ; }
          

protected :

private :

// Local class constants
 
  static const G4double fDelta ; // energy shift from interval border = 0.001


  G4int    fIntervalNumber;    //  The number of energy intervals
  G4double fNormalizationCof;   // Normalization cof for PhotoAbsorptionXsection
  G4int    fCurrentInterval;
  G4int    fIntervalTmax;
  G4double fBetaGammaSq ;        // (beta*gamma)^2
  G4double fTmax;
  G4double fDensity ;            // Current density
  G4double fElectronDensity ;    // Current electron (number) density
  G4int    fSplineNumber ;       // Current size of spline

// Arrays of Sandia coefficients

  G4OrderedTable* fMatSandiaMatrix;
  G4SandiaTable*  fSandia;

} ;    

#endif   

//
//
/////////////////   end of G4InitXscPAI header file    //////////////////////
