import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibNtuple.simpleNtuple_cff import *
from Calibration.EcalCalibNtuple.recoTags_cff import *

process = cms.Process("SimpleNtuple")



# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))



# simpleNtuple
ReReco=False
makeSimpleNtuple(process,GlobalTag="START42_V11::All",ReReco=ReReco)
#makeRecoTags(process)

process.simpleNtuple.saveMCPU = cms.untracked.bool(True)
process.simpleNtuple.saveMCZW = cms.untracked.bool(True)

process.simpleNtuple.eventType = cms.untracked.int32(0)
##---- 0 = signal    1 = background 



# source
process.source.fileNames = cms.untracked.vstring(
    #'file:/data_CMS/cms/abenagli/MC_GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6_PU_S1_START311_V1G1-v1_AODSIM.root'
    'file:/data2/WenuRelVal_428.root'
    )

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)

# out
if not ReReco:
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string("simpleNtuple.root")
        )

    #process.out = cms.OutputModule(
    #    "PoolOutputModule",
    #    fileName = cms.untracked.string('file:SimplePATple.root'),
    #    outputCommands = cms.untracked.vstring()
    #    )

if ReReco:
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string("simpleNtupleReReco.root")
        )

    #process.out = cms.OutputModule(
    #    "PoolOutputModule",
    #    fileName = cms.untracked.string('simplePATpleReReco.root'),
    #    outputCommands = cms.untracked.vstring('keep *_*_*_*')
    #    )
    #process.e = cms.EndPath(process.out)



# schedule

if not ReReco:
    process.schedule = cms.Schedule(
        process.simpleNtuple_step
        )

if ReReco:
    process.schedule = cms.Schedule(
        process.raw2digi_step,
        process.L1Reco_step,
        process.reconstruction_step,
        process.endjob_step,
        process.simpleNtuple_step
        #process.e
        )