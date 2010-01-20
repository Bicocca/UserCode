import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer(
    "SimpleNtple",
     MuTag           = cms.InputTag("muons"),
     Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducer:Mu3DipSignificanceMap"),
     MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:MuTipSignificanceMap"),
     MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:MuLipSignificanceMap"),
     EleTag          = cms.InputTag("gsfElectrons"),
     Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducer:Ele3DipSignificanceMap"),
     EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:EleTipSignificanceMap"),
     EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:EleLipSignificanceMap"),
     TracksTag       = cms.InputTag("generalTracks"),
     JetTag          = cms.InputTag("sisCone5CaloJets"),
     flag_JetBTag    = cms.untracked.bool(False),
     JetBTag         = cms.untracked.InputTag("newJetBProbabilityBJetTags"),
     correctedJetTag = cms.InputTag("dummy"),
     MetTag          = cms.InputTag("met"),         
     MCtruthTag      = cms.InputTag("genParticles"),
     genJetTag       = cms.InputTag("sisCone5GenJets"),
     genMetTag       = cms.InputTag("genMetCalo"),
     
     eleIDCut_LooseInputTag  = cms.InputTag("eidLoose"),
     eleIDCut_RLooseInputTag = cms.InputTag("eidRobustLoose"),
     eleIDCut_TightInputTag  = cms.InputTag("eidTight"), 
     eleIDCut_RTightInputTag = cms.InputTag("eidRobustTight"),
 
     saveMu          = cms.untracked.bool (True) ,
     saveTrack       = cms.untracked.bool (True) , 
     saveEle         = cms.untracked.bool (True) ,
     saveJet         = cms.untracked.bool (True) ,
     saveMet         = cms.untracked.bool (True) ,
     saveGenJet      = cms.untracked.bool (True) ,
     saveGenMet      = cms.untracked.bool (True) ,                        
     saveMC          = cms.untracked.bool (True) ,
     
        
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0), 

)
