import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
#process.MessageLogger.cerr.threshold = ''
#process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(
#PG 428p4 production
'dcap://pnfs/roma1.infn.it/data/cms/store/user/govoni/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/SQWaT_PAT_42X_Fall11_v3_2/5dfbd3335c0693a371a47af125bb82e4/pat_42x_fall11_withANDfilter_91_0_Eop.root',
    )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )    
process.source.inputCommands = cms.untracked.vstring("keep *","drop *_MEtoEDMConverter_*_*")

process.EleWithRho = cms.EDProducer("ElectronWithRhoProducer",
  leptonTag = cms.InputTag ("selectedPatElectronsPFlow") ,
  rhoTag = cms.InputTag ("kt6PFJetsForIsolation:rho:PAT") 
  )

process.MuWithRho = cms.EDProducer("MuonWithRhoProducer",
  leptonTag = cms.InputTag ("selectedPatMuonsPFlow") ,
  rhoTag = cms.InputTag ("kt6PFJetsForIsolation:rho:PAT")
  )

process.out = cms.OutputModule("PoolOutputModule", 
       fileName = cms.untracked.string("testLeptonWithRhoProducer.root"),
       outputCommands = cms.untracked.vstring(
          'drop *', 
          'keep *_EleWithRho_*_*', 
          'keep *_MuWithRho_*_*'
        )
    )

process.p = cms.Path (process.EleWithRho + process.MuWithRho)
process.outpath = cms.EndPath(process.out)


