import FWCore.ParameterSet.Config as cms

simpleNtuple = cms.EDAnalyzer(
    "SimpleNtupleCalib",
    MCPileupTag = cms.InputTag ("addPileupInfo"),
    L1Tag               = cms.InputTag("gtDigis"),
    TriggerEventTag     = cms.InputTag("hltTriggerSummaryAOD"),
    TriggerResultsTag   = cms.InputTag("TriggerResults::HLT"),
    PVTag               = cms.InputTag("offlinePrimaryVertices"),
    recHitCollection_EB = cms.InputTag("reducedEcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("reducedEcalRecHitsEE"),
    #recHitCollection_EB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    #recHitCollection_EE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),    
    EleTag              = cms.InputTag("selectedPatElectrons"),
    PhotonTag           = cms.InputTag("selectedPatPhotons"),
    MuTag               = cms.InputTag("selectedPatMuons"),
    JetTag              = cms.InputTag("selectedPatJets"),
    CALOMetTag          = cms.InputTag("patMETs"),
    TCMetTag            = cms.InputTag("patMETsTC"),
    PFMetTag            = cms.InputTag("patMETsPF"),
    MCtruthTag          = cms.InputTag("genParticles"),
    eleId_names         = cms.vstring('eidLoose','eidTight','eidRobustLoose','eidRobustTight'),
    useTriggerEvent = cms.untracked.bool(True),
    dataFlag      = cms.untracked.bool(True),
    saveL1        = cms.untracked.bool(True),
    saveHLT       = cms.untracked.bool(True),
    saveRho       = cms.untracked.bool(True),
    saveEle       = cms.untracked.bool(True),
    savePho       = cms.untracked.bool(True),
    saveSC        = cms.untracked.bool(False),
    saveEleShape  = cms.untracked.bool(True),
    saveMu        = cms.untracked.bool(True),
    saveJet       = cms.untracked.bool(True),
    saveCALOMet   = cms.untracked.bool(True),
    saveTCMet     = cms.untracked.bool(True),
    savePFMet     = cms.untracked.bool(True),
    saveMCPU      = cms.untracked.bool(True),
    verbosity_    = cms.untracked.bool(False),
    posCalcParameters = cms.PSet( T0_barl      = cms.double(7.4),
                                  T0_endc      = cms.double(6.3),        
                                  T0_endcPresh = cms.double(3.6),
                                  LogWeighted  = cms.bool(True),
                                  W0           = cms.double(4.2),
                                  X0           = cms.double(0.89)
                                  )

    )
