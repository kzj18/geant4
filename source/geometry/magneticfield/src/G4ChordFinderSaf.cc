

#include "G4ChordFinderSaf.hh"
#include <iomanip>

// ..........................................................................

G4ChordFinderSaf::G4ChordFinderSaf(G4MagInt_Driver* pIntegrationDriver)
  : G4ChordFinder(pIntegrationDriver)
{
    // check the values and set the other parameters
    // fNoInitialRadBig=0;    fNoInitialRadSmall=0;  
    // fNoTrialsRadBig=0;     fNoTrialsRadSmall=0; 
   
}

// ..........................................................................

G4ChordFinderSaf::G4ChordFinderSaf( G4MagneticField*        theMagField,
                              G4double                stepMinimum, 
                              G4MagIntegratorStepper* pItsStepper )
  : G4ChordFinder( theMagField, stepMinimum, pItsStepper )
{
  //  Let  G4ChordFinder create the  Driver, the Stepper and EqRhs ...
  // ...
}

// ......................................................................

// ......................................................................

G4ChordFinderSaf::~G4ChordFinderSaf()
{
  if( SetVerbose(0) ) { PrintStatistics();  } 
   // Set verbosity 0, so that will be called in base class again
}

void
G4ChordFinderSaf::PrintStatistics()
{
  // Print Statistics
  G4cout << "G4ChordFinderSaf statistics report: " << G4endl;
  G4ChordFinder::PrintStatistics();

/*******************
  G4cout 
    << "  No radbig calls " << std::setw(10) << fNoInitialRadBig 
    << " trials " << std::setw(10) << fNoTrialsRadBig
    << "  - over " << std::setw(10) << fNoTrialsRadBig - fNoInitialRadBig
    << G4endl
    << "  No radsm  calls " << std::setw(10) << fNoInitialRadSmall 
    << " trials "  << std::setw(10) << fNoTrialsRadSmall
    << "  - over " << std::setw(10) << fNoTrialsRadSmall - fNoInitialRadSmall
    << G4endl;
  G4cout
    << "  *** Limiting stepTrial via if Delta_chord < R_curvature " 
    << "   for large to angle from Delta_chord / R_curv "  
    << "   and for small with multiple " << GetMultipleRadius()
    << G4endl; 
********************/
}


// G4SafetyDist::
// inline
G4double
CalculatePointSafety(G4ThreeVector safetyOrigin,
		     G4double      safetyRadius,
		     G4ThreeVector point)
{
  G4double      pointSafety= 0.0; 

  G4ThreeVector OriginShift = point - safetyOrigin ;
  G4double      MagSqShift  = OriginShift.mag2() ;
  if( MagSqShift < sqr(safetyRadius) ){ 
    pointSafety = safetyRadius - sqrt(MagSqShift) ;  
  }

  return pointSafety;
}

// inline
G4bool
CalculatePointInside(G4ThreeVector safetyOrigin,
		     G4double      safetyRadius,
		     G4ThreeVector point)
{
  G4ThreeVector OriginShift = point - safetyOrigin ;
  return ( OriginShift.mag2() < safetyRadius*safetyRadius ); 
}

G4double
G4ChordFinderSaf::FindNextChord( const  G4FieldTrack  yStart,
                                     G4double     stepMax,
                                     G4FieldTrack&   yEnd, // Endpoint
                                     G4double&   dyErrPos, // Error of endpoint
                                     G4double    epsStep,
                                     G4double*  pStepForAccuracy, 
				    const G4ThreeVector latestSafetyOrigin,
				    G4double       latestSafetyRadius 
					)
  // Returns Length of Step taken
{
  // G4int       stepRKnumber=0;
  G4FieldTrack yCurrent=  yStart;  
  G4double    stepTrial, stepForAccuracy;
  G4double    dydx[G4FieldTrack::ncompSVEC]; 

  //  1.)  Try to "leap" to end of interval
  //  2.)  Evaluate if resulting chord gives d_chord that is good enough.
  //     2a.)  If d_chord is not good enough, find one that is.
  
  G4bool    validEndPoint= false;
  G4double  dChordStep, lastStepLength; //  stepOfLastGoodChord;

  GetIntegrationDriver()-> GetDerivatives( yCurrent, dydx )  ;

  G4int     noTrials=0;
  const G4double safetyFactor= GetFirstFraction(); // was 0.999

  // Figure out the starting safety
  G4double      startSafety=  
    CalculatePointSafety( latestSafetyOrigin, 
			  latestSafetyRadius, 
			  yCurrent.GetPosition() );

  G4double
    likelyGood = std::max( startSafety , 
                        safetyFactor *  GetLastStepEstimateUnc() );

  stepTrial  = std::min( stepMax,  likelyGood ); 

  G4MagInt_Driver *pIntgrDriver= G4ChordFinder::GetIntegrationDriver(); 
  G4double newStepEst_Uncons= 0.0; 
  do
  { 
     G4double stepForChord;  
     yCurrent = yStart;    // Always start from initial point

   //            ************
     pIntgrDriver->QuickAdvance( yCurrent, dydx, stepTrial, 
                                 dChordStep, dyErrPos);
     //            ************

     G4ThreeVector EndPointCand= yCurrent.GetPosition(); 
     G4bool  endPointInSafetySphere= 
       CalculatePointInside(latestSafetyOrigin, latestSafetyRadius, EndPointCand);

     // We check whether the criterion is met here.
     validEndPoint = AcceptableMissDist(dChordStep)
                       || endPointInSafetySphere;
                      //  && (dyErrPos < eps) ;

     lastStepLength = stepTrial; 

     // This method estimates to step size for a good chord.
     stepForChord = NewStep(stepTrial, dChordStep, newStepEst_Uncons );

     if( ! validEndPoint ) {
        if( stepTrial<=0.0 )
	  stepTrial = stepForChord; 
        else if (stepForChord <= stepTrial) 
          // Reduce by a fraction, possibly up to 20% 
          stepTrial = std::min( stepForChord, 
                                GetFractionLast() * stepTrial); 
        else
          stepTrial *= 0.1;

        // if(dbg) G4cerr<<"Dchord too big. Try new hstep="<<stepTrial<<G4endl;
     }
     // #ifdef  TEST_CHORD_PRINT
     // TestChordPrint( noTrials, lastStepLength, dChordStep, stepTrial );
     // #endif

     noTrials++; 
  }
  while( ! validEndPoint );   // End of do-while  RKD 

  AccumulateStatistics( noTrials );

  // Should we update newStepEst_Uncons for a 'long step' via safety ??
  if( newStepEst_Uncons > 0.0  ){ 
    SetLastStepEstimateUnc( newStepEst_Uncons );
  }

  // stepOfLastGoodChord = stepTrial;

  if( pStepForAccuracy ){ 
     // Calculate the step size required for accuracy, if it is needed
     G4double dyErr_relative = dyErrPos/(epsStep*lastStepLength);
     if( dyErr_relative > 1.0 ) {
        stepForAccuracy =
           pIntgrDriver->ComputeNewStepSize( dyErr_relative,
                                             lastStepLength );
     }else{
        stepForAccuracy = 0.0;   // Convention to show step was ok 
     }
     *pStepForAccuracy = stepForAccuracy;
  }

#ifdef  TEST_CHORD_PRINT
  static int dbg=0;
  if( dbg ) 
    G4cout << "ChordF/FindNextChord:  NoTrials= " << noTrials 
           << " StepForGoodChord=" << std::setw(10) << stepTrial << G4endl;
#endif

  yEnd=  yCurrent;  
  return stepTrial; 
}
