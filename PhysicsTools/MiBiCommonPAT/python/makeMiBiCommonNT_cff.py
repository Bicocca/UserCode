import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *

from PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.photonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *



def makeMiBiCommonNT(process, GlobalTag, HLT='HLT', MC=False, MCType='Other'):

    # Setup the process
    process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 10
    process.load('Configuration.StandardSequences.GeometryExtended_cff')
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag

    
    # Source
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring()
    )
    
    # Out
    process.out = cms.OutputModule(
      "PoolOutputModule",
      fileName = cms.untracked.string('file:./MiBiCommonPAT.root'),
      outputCommands = cms.untracked.vstring()
    )


    process.load("PhysicsTools.NtupleUtils.AllPassFilter_cfi")

    #--------------------------
    # Counter1: All read events
    process.AllEvents = process.AllPassFilter.clone()
    


    #--------------------------
    # Counter2: Scraping filter
    process.scrapingFilter = cms.EDFilter("FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
    )
    
    process.NonScrapedEvents = process.AllPassFilter.clone()
    
    #-----------------------------------
    # Counter3: Filter on primary vertex
    process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
        minimumNDOF = cms.uint32(4) ,
        maxAbsZ = cms.double(24),
        maxd0 = cms.double(2)
    )
    
    process.GoodVtxEvents = process.AllPassFilter.clone()
    
    #------------------
    #Load PAT sequences
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    process.load("PhysicsTools.PatAlgos.tools.pfTools")
    postfix = "PFlow"
    usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=MC, postfix=postfix)  
    
    if not MC:
     removeMCMatching(process, ['All'])
    
    process.patJets.addTagInfos = cms.bool(False)    #bugfix related to btagging
    
    
    
    # ---------------
    # add collections
    addTcMET(process, 'TC')
    addPfMET(process, 'PF')
    
    addJetCollection(
        process,
        cms.InputTag('ak5CaloJets'),
        'AK5',
        'Calo',
        doJTA        = True,
        doBTagging   = True,
        jetCorrLabel = ('AK5', 'Calo'),
        doType1MET   = True,
        doL1Cleaning = True,
        doL1Counters = False,
        genJetCollection=cms.InputTag("ak5GenJets"),
        doJetID      = True
        )
    
    addJetCollection(
        process,
        cms.InputTag('ak5PFJets'),
        'AK5',
        'PF',
        doJTA        = True,
        doBTagging   = True,
        jetCorrLabel = ('AK5', 'PF'),
        doType1MET   = False,
        doL1Cleaning = True,
        doL1Counters = False,
        genJetCollection=cms.InputTag("ak5GenJets"),
        doJetID      = True
        )
    
    

    #---------
    #PG PF2PAT sequence from /CMSSW/PhysicsTools/PatAlgos/test/patTuple_PATandPF2PAT_cfg.py
    # An empty postfix means that only PF2PAT is run,
    # otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
    # collections have standard names + postfix (e.g. patElectronPFlow)  
    
    
    
    # -------------------
    # pat selection layer
    process.selectedPatElectrons.cut      = cms.string("pt > 20. & abs(eta) < 2.5")
    process.selectedPatElectronsPFlow.cut = cms.string("pt > 20. & abs(eta) < 2.5")    

    process.selectedPatMuons.cut      = cms.string("pt > 20. & abs(eta) < 2.5")
    process.selectedPatMuonsPFlow.cut = cms.string("pt > 20. & abs(eta) < 2.5")

    process.selectedPatJets.cut        = cms.string("pt > 15. & abs(eta) < 5")
    process.selectedPatJetsPFlow.cut   = cms.string("pt > 15. & abs(eta) < 5")    
    process.selectedPatJetsAK5Calo.cut = cms.string("pt > 15. & abs(eta) < 5")
    process.selectedPatJetsAK5PF.cut   = cms.string("pt > 15. & abs(eta) < 5")

    process.selectedPatPhotons.cut      = cms.string("pt > 10. & abs(eta) < 5")
    process.selectedPatPhotonsPFlow.cut = cms.string("pt > 10. & abs(eta) < 5")    
    
    # the HCAL Noise Filter
    #process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
    
    # the MiBiPAT path
    process.MiBiCommonPAT = cms.Sequence(
        process.AllEvents * # -> Counter
        process.scrapingFilter *
        process.NonScrapedEvents * # -> Counter
        process.primaryVertexFilter *
        process.GoodVtxEvents * # -> Counter
        getattr(process,"patPF2PATSequence"+postfix) *
        process.patDefaultSequence
    )
    

    
    #----------------------
    # Lepton/Photon Filters
    process.load('PhysicsTools.PatAlgos.selectionLayer1.leptonCountFilter_cfi')
    process.LeptonsFilter = countPatLeptons.clone(
      electronSource = cms.InputTag("selectedPatElectrons"),
      muonSource     = cms.InputTag("selectedPatMuons"),
      minNumber      = cms.uint32(1)
     )

    
    process.LeptonsFilterPFlow = countPatLeptons.clone(
      electronSource = cms.InputTag("selectedPatElectronsPFlow"),
      muonSource     = cms.InputTag("selectedPatMuonsPFlow"),
      minNumber      = cms.uint32(1)
     )
    
    
    
    process.load('PhysicsTools.PatAlgos.selectionLayer1.photonCountFilter_cfi')
    process.PhotonsFilter = countPatPhotons.clone(
      src       = cms.InputTag("selectedPatPhotons"),
      minNumber = cms.uint32(2)
     )

    
    
    #------------
    # Jet Filters
    process.load('PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi')
    process.JetFilterAK5Calo = countPatJets.clone(
      src = cms.InputTag("selectedPatJetsAK5Calo"),
      minNumber      = cms.uint32(2)
    )
    
    
    process.JetFilterAK5PF = countPatJets.clone(
      src = cms.InputTag("selectedPatJetsAK5PF"),
      minNumber      = cms.uint32(2)
    )
    
     
    process.JetFilterPFlow = countPatJets.clone(
      src = cms.InputTag("selectedPatJetsPFlow"),
      minNumber      = cms.uint32(2)
    )
    
    
    
    #-----------
    # Sequences & Other counters
    process.LeptonsFilterEvents = process.AllPassFilter.clone()
    process.LeptonsFilterPFlowEvents = process.AllPassFilter.clone()
    process.JetFilterAK5CaloEvents = process.AllPassFilter.clone()
    process.JetFilterAK5PFEvents = process.AllPassFilter.clone()
    process.JetFilterPFlowEvents = process.AllPassFilter.clone()
    process.PhotonsFilterEvents = process.AllPassFilter.clone()
    
    process.OneLeptonTwoJetsAK5CaloSeq = cms.Sequence(
        process.LeptonsFilter*
        process.LeptonsFilterEvents*
        process.JetFilterAK5Calo*
        process.JetFilterAK5CaloEvents
       )

    
    process.OneLeptonTwoJetsAK5PFSeq = cms.Sequence(
        process.LeptonsFilter*
        #process.LeptonsFilterEvents*
        process.JetFilterAK5PF*
        process.JetFilterAK5PFEvents
        )
    
    
    process.OneLeptonTwoJetsPFlowSeq = cms.Sequence(
        process.LeptonsFilterPFlow*
        process.LeptonsFilterPFlowEvents*
        process.JetFilterPFlow*
        process.JetFilterPFlowEvents
        )

    process.TwoPhotonsSeq = cms.Sequence(
        process.PhotonsFilter*
        process.PhotonsFilterEvents
        )
        
        
    # the MiBiNTUPLE
    process.load("PhysicsTools.MiBiCommonPAT.SimpleNtuple_cfi")
    process.MiBiCommonNT = process.SimpleNtuple.clone()
    process.MiBiCommonNT.HLTTag = cms.InputTag("TriggerResults","",HLT)
    process.MiBiCommonNT.saveGenJet            = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCPtHat           = cms.untracked.bool (MC)
    process.MiBiCommonNT.saveMCTTBar           = cms.untracked.bool (False)    
    process.MiBiCommonNT.saveMCHiggs           = cms.untracked.bool (False)
    process.MiBiCommonNT.saveMCHiggsWW         = cms.untracked.bool (False)
    process.MiBiCommonNT.saveMCHiggsGammaGamma = cms.untracked.bool (False)
    
    if MCType == 'TTBar':
        process.MiBiCommonNT.saveMCTTBar = cms.untracked.bool (True)
    if MCType == 'Higgs':
        process.MiBiCommonNT.saveMCHiggs = cms.untracked.bool (True)
    if MCType == 'HiggsWW':
        process.MiBiCommonNT.saveMCHiggs   = cms.untracked.bool (True)
        process.MiBiCommonNT.saveMCHiggsWW = cms.untracked.bool (True)
    if MCType == 'HiggsGammaGamma':
        process.MiBiCommonNT.saveMCHiggs           = cms.untracked.bool (True)
        process.MiBiCommonNT.saveMCHiggsGammaGamma = cms.untracked.bool (True)                
    
    process.MiBiPathAK5PF = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsAK5PFSeq*process.MiBiCommonNT)
    process.MiBiPathAK5Calo = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsAK5CaloSeq*process.MiBiCommonNT)
    process.MiBiPathPFlow = cms.Path(process.MiBiCommonPAT*process.OneLeptonTwoJetsPFlowSeq*process.MiBiCommonNT)
    process.MiBiPathPhotons = cms.Path(process.MiBiCommonPAT*process.TwoPhotonsSeq*process.MiBiCommonNT)

