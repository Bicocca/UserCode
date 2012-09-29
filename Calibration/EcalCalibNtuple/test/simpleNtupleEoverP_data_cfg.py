import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtupleEoverP_cff import *
from Calibration.EcalCalibNtuple.recoTags_cff import *

process = cms.Process("SimpleNtupleEoverP")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

# simpleNtuple
makeSimpleNtuple(process,GlobalTag="GR_P_V41::All",runOverSandbox=False,runOverData=True)

#makeRecoTags(process)  #commented for 05Jul ReReco
#makeSqliteTags(process)



# source
process.source.fileNames = cms.untracked.vstring(
    '/store/data/Run2012C/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v3/000/202/087/00000/D847428A-3CF8-E111-B644-002618FDA265.root',
    '/store/data/Run2012C/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v3/000/202/087/00000/D63AF27F-3CF8-E111-921A-003048678FFE.root'
    
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("simpleNtuple.root")
    )


process.schedule = cms.Schedule(
    process.simpleNtuple_step
    )
