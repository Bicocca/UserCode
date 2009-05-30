# Auto generated configuration file
# using: 
# Revision: 1.99.2.3 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: SingleElectronE200_cfi.py -s GEN,FASTSIM --pileup=NoPileUp --conditions FrontierConditions_GlobalTag,IDEAL_V11::All --eventcontent=FEVTDEBUG -n 10
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('FastSimulation/Configuration/RandomServiceInitialization_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Generator_cff')
process.load('FastSimulation/Configuration/FamosSequences_cff')
process.load('FastSimulation/Configuration/HLT_cff')
process.load('Configuration.StandardSequences.L1TriggerDefaultMenu_cff')
#process.load('FastSimulation.PileUpProducer.PileUpSimulator10TeV_cfi')
#process.load('IOMC.EventVertexGenerators.VtxSmearedParameters_cfi')
#process.load('FastSimulation.Configuration.mixNoPU_cfi')
process.load('FastSimulation/Configuration/CommonInputs_cff')
process.load('FastSimulation/Configuration/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.99.2.3 $'),
    annotation = cms.untracked.string('SingleElectronE200_cfi.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("FlatRandomEGunSource",
 PGunParameters = cms.untracked.PSet(
    PartID = cms.untracked.vint32(11),
    MaxEta = cms.untracked.double(-0.2094),
    MinEta = cms.untracked.double(-0.2269),
    MinPhi = cms.untracked.double(0.12217),
    MaxPhi = cms.untracked.double(0.1396),
    MinE = cms.untracked.double(200.0),
    MaxE = cms.untracked.double(200.0)
    ),
    Verbosity = cms.untracked.int32(0),
    AddAntiParticle = cms.untracked.bool(False)
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
                                  outputCommands = cms.untracked.vstring("keep *"),
                                  fileName = cms.untracked.string(
                                  '/gwtera1/users/martelli/data/proveTT/Fast_withTP/prova_EBneg_100.root'

                                                                  ),
                                  dataset = cms.untracked.PSet(
                                                             dataTier = cms.untracked.string(''),
                                                             filterName = cms.untracked.string('')
                                                             ),
                                  SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('generation_step')
                                    )
                                  )

# Additional output definition

# If you want to turn on/off pile-up
process.famosPileUp.PileUpSimulator.averageNumber = 0
# You may not want to simulate everything for your study
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
# process.famosSimHits.SimulateMuons = False

# Other statements
process.famosPileUp.PileUpSimulator.averageNumber = 0
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.famosSimHits.ActivateDecays.comEnergy = 10000
#process.famosPileUp.PileUpSimulator = process.PileUpSimulatorBlock.PileUpSimulator
process.simulation = cms.Sequence(process.simulationWithFamos)
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)




# set correct vertex smearing
#process.Early10TeVCollisionVtxSmearingParameters.type = cms.string("BetaFunc")
#process.famosSimHits.VertexGenerator = process.Early10TeVCollisionVtxSmearingParameters
#process.famosPileUp.VertexGenerator = process.Early10TeVCollisionVtxSmearingParameters
# Apply Tracker misalignment
#process.famosSimHits.ApplyAlignment = True
#process.misalignedTrackerGeometry.applyAlignment = True

process.GlobalTag.globaltag = 'IDEAL_V11::All'

# Path and EndPath definitions
process.generation_step = cms.Path(cms.SequencePlaceholder("randomEngineStateProducer")+process.GeneInfo+process.genJetMET)
process.reconstruction = cms.Path(process.reconstructionWithFamos)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,
                                process.HLTriggerFirstPath,
                                process.HLT_L1Jet15,
                                process.HLT_Jet30,
                                process.HLT_Jet50,
                                process.HLT_Jet80,
                                process.HLT_Jet110,
                                process.HLT_Jet180,
                                process.HLT_Jet250,
                                process.HLT_FwdJet20,
                                process.HLT_DoubleJet150,
                                process.HLT_DoubleJet125_Aco,
                                process.HLT_DoubleFwdJet50,
                                process.HLT_DiJetAve15,
                                process.HLT_DiJetAve30,
                                process.HLT_DiJetAve50,
                                process.HLT_DiJetAve70,
                                process.HLT_DiJetAve130,
                                process.HLT_DiJetAve220,
                                process.HLT_TripleJet85,
                                process.HLT_QuadJet30,
                                process.HLT_QuadJet60,
                                process.HLT_SumET120,
                                process.HLT_L1MET20,
                                process.HLT_MET25,
                                process.HLT_MET35,
                                process.HLT_MET50,
                                process.HLT_MET65,
                                process.HLT_MET75,
                                process.HLT_MET65_HT350,
                                process.HLT_Jet180_MET60,
                                process.HLT_Jet60_MET70_Aco,
                                process.HLT_Jet100_MET60_Aco,
                                process.HLT_DoubleJet125_MET60,
                                process.HLT_DoubleFwdJet40_MET60,
                                process.HLT_DoubleJet60_MET60_Aco,
                                process.HLT_DoubleJet50_MET70_Aco,
                                process.HLT_DoubleJet40_MET70_Aco,
                                process.HLT_TripleJet60_MET60,
                                process.HLT_QuadJet35_MET60,
                                process.HLT_IsoEle15_L1I,
                                process.HLT_IsoEle18_L1R,
                                process.HLT_IsoEle15_LW_L1I,
                                process.HLT_LooseIsoEle15_LW_L1R,
                                process.HLT_Ele10_SW_L1R,
                                process.HLT_Ele15_SW_L1R,
                                process.HLT_Ele15_LW_L1R,
                                process.HLT_EM80,
                                process.HLT_EM200,
                                process.HLT_DoubleIsoEle10_L1I,
                                process.HLT_DoubleIsoEle12_L1R,
                                process.HLT_DoubleIsoEle10_LW_L1I,
                                process.HLT_DoubleIsoEle12_LW_L1R,
                                process.HLT_DoubleEle5_SW_L1R,
                                process.HLT_DoubleEle10_LW_OnlyPixelM_L1R,
                                process.HLT_DoubleEle10_Z,
                                process.HLT_DoubleEle6_Exclusive,
                                process.HLT_IsoPhoton30_L1I,
                                process.HLT_IsoPhoton10_L1R,
                                process.HLT_IsoPhoton15_L1R,
                                process.HLT_IsoPhoton20_L1R,
                                process.HLT_IsoPhoton25_L1R,
                                process.HLT_IsoPhoton40_L1R,
                                process.HLT_Photon15_L1R,
                                process.HLT_Photon25_L1R,
                                process.HLT_DoubleIsoPhoton20_L1I,
                                process.HLT_DoubleIsoPhoton20_L1R,
                                process.HLT_DoublePhoton10_Exclusive,
                                process.HLT_L1Mu,process.HLT_L1MuOpen,
                                process.HLT_L2Mu9,process.HLT_IsoMu9,
                                process.HLT_IsoMu11,process.HLT_IsoMu13,
                                process.HLT_IsoMu15,process.HLT_Mu3,
                                process.HLT_Mu5,process.HLT_Mu7,
                                process.HLT_Mu9,process.HLT_Mu11,
                                process.HLT_Mu13,process.HLT_Mu15, process.HLT_Mu15_Vtx2mm, process.HLT_DoubleIsoMu3,
                                process.HLT_DoubleMu3, process.HLT_DoubleMu3_Vtx2mm, process.HLT_DoubleMu3_JPsi,
                                process.HLT_DoubleMu3_Upsilon,process.HLT_DoubleMu7_Z,
                                process.HLT_DoubleMu3_SameSign,process.HLT_DoubleMu3_Psi2S,process.HLT_BTagIP_Jet180,
                                process.HLT_BTagIP_Jet120_Relaxed,process.HLT_BTagIP_DoubleJet120,
                                process.HLT_BTagIP_DoubleJet60_Relaxed,process.HLT_BTagIP_TripleJet70,
                                process.HLT_BTagIP_TripleJet40_Relaxed,process.HLT_BTagIP_QuadJet40,
                                process.HLT_BTagIP_QuadJet30_Relaxed,process.HLT_BTagIP_HT470,
                                process.HLT_BTagIP_HT320_Relaxed,process.HLT_BTagMu_DoubleJet120,
                                process.HLT_BTagMu_DoubleJet60_Relaxed,process.HLT_BTagMu_TripleJet70,
                                process.HLT_BTagMu_TripleJet40_Relaxed,process.HLT_BTagMu_QuadJet40,
                                process.HLT_BTagMu_QuadJet30_Relaxed,process.HLT_BTagMu_HT370,
                                process.HLT_BTagMu_HT250_Relaxed,process.HLT_DoubleMu3_BJPsi,
                                process.HLT_DoubleMu4_BJPsi,process.HLT_TripleMu3_TauTo3Mu,
                                process.HLT_IsoTau_MET65_Trk20,process.HLT_IsoTau_MET35_Trk15_L1MET,
                                process.HLT_LooseIsoTau_MET30,process.HLT_LooseIsoTau_MET30_L1MET,
                                process.HLT_DoubleIsoTau_Trk3,process.HLT_DoubleLooseIsoTau,
                                process.HLT_IsoEle8_IsoMu7,process.HLT_IsoEle10_Mu10_L1R,
                                process.HLT_IsoEle12_IsoTau_Trk3,process.HLT_IsoEle10_BTagIP_Jet35,
                                process.HLT_IsoEle12_Jet40,process.HLT_IsoEle12_DoubleJet80,
                                process.HLT_IsoEle5_TripleJet30,process.HLT_IsoEle12_TripleJet60,
                                process.HLT_IsoEle12_QuadJet35,process.HLT_IsoMu14_IsoTau_Trk3,
                                process.HLT_IsoMu7_BTagIP_Jet35,process.HLT_IsoMu7_BTagMu_Jet20,
                                process.HLT_IsoMu7_Jet40,process.HLT_NoL2IsoMu8_Jet40,process.HLT_Mu14_Jet50,
                                process.HLT_Mu5_TripleJet30, process.HLT_BTagMu_Jet20_Calib, process.HLT_ZeroBias,
                                process.HLT_MinBias, process.HLT_MinBiasHcal, process.HLT_MinBiasEcal,
                                process.HLT_MinBiasPixel, process.HLT_MinBiasPixel_Trk5, process.HLT_BackwardBSC,
                                process.HLT_ForwardBSC, process.HLT_CSCBeamHalo, process.HLT_CSCBeamHaloOverlapRing1,
                                process.HLT_CSCBeamHaloOverlapRing2, process.HLT_CSCBeamHaloRing2or3,
                                process.HLT_TrackerCosmics, process.AlCa_IsoTrack, process.AlCa_EcalPhiSym,
                                process.AlCa_EcalPi0, process.HLTriggerFinalPath, process.reconstruction, process.out_step)
