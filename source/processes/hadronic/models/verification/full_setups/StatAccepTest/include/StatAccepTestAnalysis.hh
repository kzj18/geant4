#ifndef StatAccepTestAnalysis_h 
#define StatAccepTestAnalysis_h 1

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"

namespace AIDA {
  class IAnalysisFactory;
  class IHistogramFactory;
  class ITree;
  class IHistogram1D;
  class IHistogram2D;
  class ITuple;
  class IPlotter;
}

class G4Step;
class G4Track;
class G4ParticleDefinition;


class StatAccepTestAnalysis {

public:

  static StatAccepTestAnalysis* getInstance();

  ~StatAccepTestAnalysis();
  
public:
  
  void init( const G4int numberOfReplicasIn, 
	     const G4int numberOfReadoutLayersIn,
	     const G4int numberOfRadiusBinsIn,
	     const G4double radiusBinIn );
  void init();
  void finish();
  void close();
  // The first method is called by StatAccepTestDetectorConstruction
  // only when the geometry is constructed the first time or changed.  
  // The second and third methods are called by StatAccepTestRunAction, 
  // at the beginning and at the end of a Run, respectively.
  // The fourth method, close(), it is called at the end of
  // job, i.e. in the destructor of StatAccepTestRunAction.

  void fillNtuple( float incidentParticleId, 
		   float incidenteParticleEnergy, 
                   float totalEnergyDepositedInActiveLayers,
                   float totalEnergyDepositedInCalorimeter );
  // This method is supposed to be called at most once for each event.  

  void fillShowerProfile( G4int replica, G4double radius, G4double edep );
  // This method is called by StatAccepTestSensitiveCalorimeter at each step
  // in the active layer of the calorimeter.

  void infoStep( const G4Step* aStep );
  // This method is called by StatAccepTestSteppingAction at each step.

  void infoTrack( const G4Track* aTrack );
  // This method is called by StatAccepTestTrackingAction when each track
  // is created.

  void fillSpectrum( const G4ParticleDefinition* particleDef, 
		     const G4int layerNumber, const G4double kinEnergy );
  // These methods fill histograms with the kinetic energy of some 
  // particles (electron or positron; pion+ or pion- or pion0; 
  // proton; neutron; gamma) when entering an active layer. 
  // Notice that the kinetic energy is defined negative when the particle 
  // is going backward with respect to the beam direction.

private:
  
  StatAccepTestAnalysis();

  void classifyParticle( const bool isTrack, const G4ParticleDefinition* particleDef );
  
  static StatAccepTestAnalysis* instance;
  
  AIDA::IAnalysisFactory* analysisFactory;
  AIDA::ITree* tree;
  AIDA::ITuple* tuple;
  AIDA::IHistogramFactory* histoFactory;

  G4int numberOfEvents;

  G4int numberOfReplicas;
  G4int numberOfReadoutLayers;
  G4int numberOfActiveLayersPerReadoutLayer;
  G4int numberOfRadiusBins;  // Number of bins in the transverse profile
  G4double radiusBin;        // Size of the first bin in the transverse profile.
                             // The other bins have a bin size that is 
                             // as follows: second bin has a size that is twice
                             // the first bin size; the third one has a size
                             // that is three times the first bin size, and so on.

  std::vector< G4double > longitudinalProfile;
  std::vector< G4double > transverseProfile; 

  // To print, at the end of the Run, the average energy deposits, <E>,
  // and its error, in the whole calorimeter, in each readout layer, 
  // and in each radius bin, we need to collect the sum of the energy 
  // deposits and the sum of the square of energy deposits.
  // We also consider the maximum total energy deposit in an event,
  // to check for energy non conservation.
  G4double beamEnergy;
  G4double sumEdepAct, sumEdepAct2;
  G4double sumEdepTot, sumEdepTot2, maxEdepTot;
  G4int countEnergyNonConservation;
  std::vector< G4double > sumL;
  std::vector< G4double > sumL2;
  std::vector< G4double > sumR;
  std::vector< G4double > sumR2;

  // Due to the non-gaussian visible energy distribution, the width 
  // of it cannot be estimated as the sigma of the gaussian fit; on
  // the other hand, the rms is not appropriate because it weights 
  // too much the tails. A better estimate would be to consider the
  // "normalized deviation". To calculate it, we need to keep all
  // the values of EdepAct in a vector.
  std::vector< G4double > vecEvis;

  // Summary histograms for the longitudinal and transverse shower profiles. 
  AIDA::IHistogram1D* longitudinalProfileHisto;
  AIDA::IHistogram1D* transverseProfileHisto;

  // Step Energy vs. Step Length information.
  AIDA::IHistogram2D* h2stepEvsL_active;
  AIDA::IHistogram2D* h2stepEvsL_electron_active;
  AIDA::IHistogram2D* h2stepEvsL_muon_active;
  AIDA::IHistogram2D* h2stepEvsL_pionCharged_active;
  AIDA::IHistogram2D* h2stepEvsL_proton_active;
  AIDA::IHistogram2D* h2stepEvsL_gamma_active;
  AIDA::IHistogram2D* h2stepEvsL_neutron_active;
  AIDA::IHistogram2D* h2stepEvsL_absorber;
  AIDA::IHistogram2D* h2stepEvsL_electron_absorber;
  AIDA::IHistogram2D* h2stepEvsL_muon_absorber;
  AIDA::IHistogram2D* h2stepEvsL_pionCharged_absorber;
  AIDA::IHistogram2D* h2stepEvsL_proton_absorber;
  AIDA::IHistogram2D* h2stepEvsL_gamma_absorber;
  AIDA::IHistogram2D* h2stepEvsL_neutron_absorber;

  // Kinetic spectra of some particles when entering an active layer.
  AIDA::IHistogram1D* emSpectrum1[10];
  AIDA::IHistogram1D* emSpectrum2[10];
  AIDA::IHistogram1D* emSpectrum3[10];
  AIDA::IHistogram1D* emSpectrum4[10];
  AIDA::IHistogram1D* emSpectrum5[10];
  AIDA::IHistogram1D* pionSpectrum1[10];
  AIDA::IHistogram1D* pionSpectrum2[10];
  AIDA::IHistogram1D* pionSpectrum3[10];
  AIDA::IHistogram1D* pionSpectrum4[10];
  AIDA::IHistogram1D* pionSpectrum5[10];
  AIDA::IHistogram1D* protonSpectrum1[10];
  AIDA::IHistogram1D* protonSpectrum2[10];
  AIDA::IHistogram1D* protonSpectrum3[10];
  AIDA::IHistogram1D* protonSpectrum4[10];
  AIDA::IHistogram1D* protonSpectrum5[10];
  AIDA::IHistogram1D* neutronSpectrum1[10];
  AIDA::IHistogram1D* neutronSpectrum2[10];
  AIDA::IHistogram1D* neutronSpectrum3[10];
  AIDA::IHistogram1D* neutronSpectrum4[10];
  AIDA::IHistogram1D* neutronSpectrum5[10];
  AIDA::IHistogram1D* gammaSpectrum1[10];
  AIDA::IHistogram1D* gammaSpectrum2[10];
  AIDA::IHistogram1D* gammaSpectrum3[10];
  AIDA::IHistogram1D* gammaSpectrum4[10];
  AIDA::IHistogram1D* gammaSpectrum5[10];

  // Keep the count of the number of steps and tracks.
  G4int numStep;
  G4int numStepPositive, numStepNeutral, numStepNegative;
  G4int numStepPDGCodeZero, numStepPDGCodeUnrecognized;
  G4int numStepEM;             // e- , e+ , gamma
  G4int numStepEWK;            // mu- , mu+ , tau+, tau-, neutrinos
  G4int numStepHAD;            // mesons + baryons
  G4int numStepMeson, numStepBaryon;     
  G4int numStepMesonLight, numStepBaryonLight;            // u/d-hadrons    
  G4int numStepMesonStrange, numStepBaryonStrange;        // s-hadrons
  G4int numStepMesonHeavy, numStepBaryonHeavy;            // c-hadrons, and b-hadrons
  G4int numStepElectron, numStepGamma, numStepPositron;
  G4int numStepMuMinus, numStepMuPlus;
  G4int numStepTauMinus, numStepTauPlus;
  G4int numStepNeutrino;
  G4int numStepPiPlus, numStepPi0, numStepPiMinus;
  G4int numStepKPlus;
  G4int numStepKNeutral;       // K0/K0bar or K0_S/KO_L
  G4int numStepKMinus;
  G4int numStepProton, numStepAntiProton;
  G4int numStepNeutron, numStepAntiNeutron;

  G4int numTrack;
  G4int numTrackPositive, numTrackNeutral, numTrackNegative;
  G4int numTrackPDGCodeZero, numTrackPDGCodeUnrecognized;
  G4int numTrackEM;             // e- , e+ , gamma
  G4int numTrackEWK;            // mu- , mu+ , tau+, tau-, neutrinos
  G4int numTrackHAD;            // mesons + baryons
  G4int numTrackMeson, numTrackBaryon;     
  G4int numTrackMesonLight, numTrackBaryonLight;            // u/d-hadrons
  G4int numTrackMesonStrange, numTrackBaryonStrange;        // s-hadrons
  G4int numTrackMesonHeavy, numTrackBaryonHeavy;            // c-hadrons, and b-hadrons
  G4int numTrackElectron, numTrackGamma, numTrackPositron;
  G4int numTrackMuMinus, numTrackMuPlus;
  G4int numTrackTauMinus, numTrackTauPlus;
  G4int numTrackNeutrino;
  G4int numTrackPiPlus, numTrackPi0, numTrackPiMinus;
  G4int numTrackKPlus;
  G4int numTrackKNeutral;       // K0/K0bar or K0_S/KO_L
  G4int numTrackKMinus;
  G4int numTrackProton, numTrackAntiProton;
  G4int numTrackNeutron, numTrackAntiNeutron;

  // Count the track length of some particles.
  G4double electronTrackLength;      // e- or e+
  G4double muonTrackLength;          // mu- or mu+
  G4double pionChargedTrackLength;   // pi- or pi+
  G4double protonTrackLength;        // proton (not anti_proton)
  G4double gammaTrackLength;         // gamma
  G4double pion0TrackLength;         // pi0       
  G4double neutronTrackLength;       // neutron (not anti_neutron)

  // Monitor the kinetic energy of particles exiting the calorimeter.
  G4double kinEnergyExiting;          // total
  G4double kinEnergyExitingGammas;    // due only to gammas
  G4double kinEnergyExitingNeutrons;  // due only to neutrons
  G4double kinEnergyExitingNeutrinos; // due only to neutrinos
  G4double kinEnergyExitingMuons;     // due only to muons
  G4int    numExiting;                // number of exiting particles
  G4int    numExitingGammas;          // number of exiting gammas
  G4int    numExitingNeutrons;        // number of exiting neutrons
  G4int    numExitingNeutrinos;       // number of exiting neutrons
  G4int    numExitingMuons;           // number of exiting neutrons

};

#endif

