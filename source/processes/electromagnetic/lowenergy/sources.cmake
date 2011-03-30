#------------------------------------------------------------------------------
# sources.cmake
# Module : G4emlowenergy
# Package: Geant4.src.G4processes.G4electromagnetic.G4emlowenergy
#
# Sources description for a library.
# Lists the sources and headers of the code explicitely.
# Lists include paths needed.
# Lists the internal granular and global dependencies of the library.
# Source specific properties should be added at the end.
#
# Generated on : 24/9/2010
#
# $Id: sources.cmake,v 1.4 2010-11-15 08:24:43 gcosmo Exp $
#
#------------------------------------------------------------------------------

# List external includes needed.
include_directories(${CLHEP_INCLUDE_DIRS})

# List internal includes needed.
include_directories(${CMAKE_SOURCE_DIR}/source/geometry/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPGeometry/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPNumerics/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPRandom/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/intercoms/include)
include_directories(${CMAKE_SOURCE_DIR}/source/materials/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/bosons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/barions/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/ions/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/mesons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/leptons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/cuts/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/standard/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/utils/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/track/include)

#
# Define the Geant4 Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME G4emlowenergy
    HEADERS
        G4AnalyticalEcpssrKCrossSection.hh
        G4AnalyticalEcpssrLiCrossSection.hh
        G4AtomicDeexcitation.hh
        G4AtomicTransitionManager.hh
        G4AugerData.hh
        G4AugerTransition.hh
        G4BoldyshevTripletModel.hh
        G4BremsstrahlungCrossSectionHandler.hh
        G4BremsstrahlungParameters.hh
        G4CompositeEMDataSet.hh
	G4CrossSectionDataSet.hh
        G4CrossSectionHandler.hh
        G4DopplerProfile.hh
        G4eBremsstrahlungSpectrum.hh
        G4eIonisationCrossSectionHandler.hh
        G4eIonisationParameters.hh
        G4eIonisationSpectrum.hh
        G4eLowEnergyLoss.hh
        G4eLowEnergyLoss.icc
        G4EMDataSet.hh
        G4empCrossSection.hh
        G4FluoData.hh
        G4FluoTransition.hh
        G4Generator2BN.hh
        G4Generator2BS.hh
        G4hBetheBlochModel.hh
        G4hICRU49He.hh
        G4hICRU49Nuclear.hh
        G4hICRU49p.hh
        G4hIonEffChargeSquare.hh
        G4hLowEnergyIonisation.hh
        G4hLowEnergyLoss.hh
        G4hLowEnergyLoss.icc
        G4hNuclearStoppingModel.hh
        G4hParametrisedLossModel.hh
        G4hQAOModel.hh
        G4hShellCrossSectionDoubleExpData.hh
        G4hShellCrossSectionDoubleExp.hh
        G4hShellCrossSectionExpData.hh
        G4hShellCrossSectionExp.hh
        G4hShellCrossSection.hh
        G4hSRIM2000p.hh
        G4hZiegler1977He.hh
        G4hZiegler1977Nuclear.hh
        G4hZiegler1977p.hh
        G4hZiegler1985Nuclear.hh
        G4hZiegler1985p.hh
        G4IonChuFluctuationModel.hh
        G4IonDEDXHandler.hh
        G4IonDEDXScalingICRU73.hh
        G4ionLowEnergyIonisation.hh
        G4IonParametrisedLossModel.hh
        G4IonParametrisedLossModel.icc
        G4IonYangFluctuationModel.hh
        G4LinInterpolation.hh
        G4LinLogInterpolation.hh
        G4LinLogLogInterpolation.hh
        G4LivermoreBremsstrahlungModel.hh
        G4LivermoreComptonModel.hh
        G4LivermoreGammaConversionModel.hh
        G4LivermoreGammaConversionModelRC.hh
        G4LivermoreIonisationModel.hh
        G4LivermoreNuclearGammaConversionModel.hh
        G4LivermorePhotoElectricModel.hh
        G4LivermorePolarizedComptonModel.hh
        G4LivermorePolarizedGammaConversionModel.hh
        G4LivermorePolarizedPhotoElectricModel.hh
        G4LivermorePolarizedRayleighModel.hh
        G4LivermoreRayleighModel.hh
        G4LogLogInterpolation.hh
        G4LowEnergyBremsstrahlung.hh
        G4LowEnergyCompton.hh
        G4LowEnergyGammaConversion.hh
        G4LowEnergyIonisation.hh
        G4LowEnergyPhotoElectric.hh
        G4LowEnergyPolarizedCompton.hh
        G4LowEnergyPolarizedRayleigh.hh
        G4LowEnergyRayleigh.hh
        G4OrlicLiCrossSection.hh
        G4PaulKCrossSection.hh
        G4Penelope08BremsstrahlungAngular.hh
        G4Penelope08BremsstrahlungFS.hh
        G4Penelope08BremsstrahlungModel.hh
        G4Penelope08ComptonModel.hh
        G4Penelope08GammaConversionModel.hh
        G4Penelope08IonisationModel.hh
        G4Penelope08PhotoElectricModel.hh
        G4Penelope08RayleighModel.hh
        G4PenelopeAnnihilationModel.hh
        G4PenelopeBremsstrahlungAngular.hh
        G4PenelopeBremsstrahlungContinuous.hh
        G4PenelopeBremsstrahlungModel.hh
        G4PenelopeComptonModel.hh
        G4PenelopeCrossSection.hh
        G4PenelopeGammaConversionModel.hh
        G4PenelopeIntegrator.hh
        G4PenelopeIntegrator.icc
        G4PenelopeInterpolator.hh
        G4PenelopeIonisationModel.hh
        G4PenelopeOscillator.hh
        G4PenelopeOscillatorManager.hh
        G4PenelopePhotoElectricModel.hh
        G4PenelopeRayleighModel.hh
        G4PenelopeSamplingData.hh
        G4PhotoElectricAngularGeneratorPolarized.hh
        G4PhotoElectricAngularGeneratorSauterGavrila.hh
        G4PhotoElectricAngularGeneratorSimple.hh
        G4QAOLowEnergyLoss.hh
        G4RangeNoTest.hh
        G4RangeTest.hh
        G4RayleighScattering.hh
        G4SemiLogInterpolation.hh
        G4ShellData.hh
        G4ShellEMDataSet.hh
        G4ShellVacancy.hh
        G4teoCrossSection.hh
        G4UAtomicDeexcitation.hh
        G4VCrossSectionHandler.hh
        G4VDataSetAlgorithm.hh
        G4VecpssrKModel.hh
        G4VecpssrLiModel.hh
        G4VeLowEnergyLoss.hh
        G4VEMDataSet.hh
        G4VEnergySpectrum.hh
        G4VhElectronicStoppingPower.hh
        G4VhEnergyLossModel.hh
        G4VhNuclearStoppingPower.hh
        G4VhShellCrossSection.hh
        G4VIonDEDXScalingAlgorithm.hh
        G4VLowEnergyDiscretePhotonProcess.hh
        G4VLowEnergyModel.hh
        G4VLowEnergyTestableDiscreteProcess.hh
        G4VPhotoElectricAngularDistribution.hh
        G4VRangeTest.hh
    SOURCES
        G4AnalyticalEcpssrKCrossSection.cc
        G4AnalyticalEcpssrLiCrossSection.cc
        G4AtomicDeexcitation.cc
        G4AtomicTransitionManager.cc
        G4AugerData.cc
        G4AugerTransition.cc
        G4BoldyshevTripletModel.cc
        G4BremsstrahlungCrossSectionHandler.cc
        G4BremsstrahlungParameters.cc
        G4CompositeEMDataSet.cc
	G4CrossSectionDataSet.cc
        G4CrossSectionHandler.cc
        G4DopplerProfile.cc
        G4eBremsstrahlungSpectrum.cc
        G4eIonisationCrossSectionHandler.cc
        G4eIonisationParameters.cc
        G4eIonisationSpectrum.cc
        G4eLowEnergyLoss.cc
        G4EMDataSet.cc
        G4empCrossSection.cc
        G4FluoData.cc
        G4FluoTransition.cc
        G4Generator2BN.cc
        G4Generator2BS.cc
        G4hBetheBlochModel.cc
        G4hICRU49He.cc
        G4hICRU49Nuclear.cc
        G4hICRU49p.cc
        G4hIonEffChargeSquare.cc
        G4hLowEnergyIonisation.cc
        G4hLowEnergyLoss.cc
        G4hNuclearStoppingModel.cc
        G4hParametrisedLossModel.cc
        G4hQAOModel.cc
        G4hShellCrossSection.cc
        G4hShellCrossSectionDoubleExp.cc
        G4hShellCrossSectionDoubleExpData.cc
        G4hShellCrossSectionExp.cc
        G4hShellCrossSectionExpData.cc
        G4hSRIM2000p.cc
        G4hZiegler1977He.cc
        G4hZiegler1977Nuclear.cc
        G4hZiegler1977p.cc
        G4hZiegler1985Nuclear.cc
        G4hZiegler1985p.cc
        G4IonChuFluctuationModel.cc
        G4IonDEDXHandler.cc
        G4IonDEDXScalingICRU73.cc
        G4IonParametrisedLossModel.cc
        G4IonYangFluctuationModel.cc
        G4LinInterpolation.cc
        G4LinLogInterpolation.cc
        G4LinLogLogInterpolation.cc
        G4LivermoreBremsstrahlungModel.cc
        G4LivermoreComptonModel.cc
        G4LivermoreGammaConversionModel.cc
        G4LivermoreGammaConversionModelRC.cc
        G4LivermoreIonisationModel.cc
        G4LivermoreNuclearGammaConversionModel.cc
        G4LivermorePhotoElectricModel.cc
        G4LivermorePolarizedComptonModel.cc
        G4LivermorePolarizedGammaConversionModel.cc
        G4LivermorePolarizedPhotoElectricModel.cc
        G4LivermorePolarizedRayleighModel.cc
        G4LivermoreRayleighModel.cc
        G4LogLogInterpolation.cc
        G4LowEnergyBremsstrahlung.cc
        G4LowEnergyCompton.cc
        G4LowEnergyGammaConversion.cc
        G4LowEnergyIonisation.cc
        G4LowEnergyPhotoElectric.cc
        G4LowEnergyPolarizedCompton.cc
        G4LowEnergyPolarizedRayleigh.cc
        G4LowEnergyRayleigh.cc
        G4OrlicLiCrossSection.cc
        G4PaulKCrossSection.cc
        G4Penelope08BremsstrahlungAngular.cc
        G4Penelope08BremsstrahlungFS.cc
        G4Penelope08BremsstrahlungModel.cc
        G4Penelope08ComptonModel.cc
        G4Penelope08GammaConversionModel.cc
        G4Penelope08IonisationModel.cc
        G4Penelope08PhotoElectricModel.cc
        G4Penelope08RayleighModel.cc
        G4PenelopeAnnihilationModel.cc
        G4PenelopeBremsstrahlungAngular.cc
        G4PenelopeBremsstrahlungContinuous.cc
        G4PenelopeBremsstrahlungModel.cc
        G4PenelopeComptonModel.cc
        G4PenelopeCrossSection.cc
        G4PenelopeGammaConversionModel.cc
        G4PenelopeInterpolator.cc
        G4PenelopeIonisationModel.cc
        G4PenelopeOscillator.cc
        G4PenelopeOscillatorManager.cc
        G4PenelopePhotoElectricModel.cc
        G4PenelopeRayleighModel.cc
        G4PenelopeSamplingData.cc
        G4PhotoElectricAngularGeneratorPolarized.cc
        G4PhotoElectricAngularGeneratorSauterGavrila.cc
        G4PhotoElectricAngularGeneratorSimple.cc
        G4QAOLowEnergyLoss.cc
        G4RangeTest.cc
        G4RayleighScattering.cc
        G4SemiLogInterpolation.cc
        G4ShellData.cc
        G4ShellEMDataSet.cc
        G4ShellVacancy.cc
        G4teoCrossSection.cc
        G4UAtomicDeexcitation.cc
        G4VCrossSectionHandler.cc
        G4VecpssrKModel.cc
        G4VecpssrLiModel.cc
        G4VeLowEnergyLoss.cc
        G4VEMDataSet.cc
        G4VhElectronicStoppingPower.cc
        G4VhNuclearStoppingPower.cc
        G4VhShellCrossSection.cc
        G4VIonDEDXScalingAlgorithm.cc
        G4VLowEnergyDiscretePhotonProcess.cc
        G4VLowEnergyModel.cc
        G4VPhotoElectricAngularDistribution.cc
    GRANULAR_DEPENDENCIES
        G4baryons
        G4bosons
        G4cuts
        G4emstandard
        G4emutils
        G4geometrymng
        G4globman
        G4hepnumerics
        G4intercoms
        G4ions
        G4leptons
        G4materials
        G4mesons
        G4partman
        G4procman
        G4track
    GLOBAL_DEPENDENCIES
        G4geometry
        G4global
        G4intercoms
        G4materials
        G4particles
        G4track
    LINK_LIBRARIES
)

# List any source specific properties here

