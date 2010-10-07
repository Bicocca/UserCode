import FWCore.ParameterSet.Config as cms

process = cms.Process("Validation")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

# initialize magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
    'file:/tmp/dimatteo/62D0033C-FCC8-DF11-96C3-00304867BEE4.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)


# filter on PhysDeclared bit
process.skimming = cms.EDFilter("PhysDecl",
applyfilter = cms.untracked.bool(True),
debugOn = cms.untracked.bool(False),
HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
)

# filter on bit 40 || 41 nad !(bit36 || bit37 || bit38 || bit39)
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# filter on MinBias HLT
process.load('HLTrigger/HLTfilters/hltHighLevel_cfi')
process.hltHighLevel = cms.EDFilter("HLTHighLevel",
 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
 HLTPaths = cms.vstring('HLT_MinBiasBSC'),
 #HLTPaths = cms.vstring('HLT_MinBias*'),
 eventSetupPathsKey = cms.string(''),
 andOr = cms.bool(True),
 throw = cms.bool(True)
) 

#Good Vertex Filter (see GOODCOLL skim)
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"), 
  filter = cms.bool(True)   
)

# FilterOutScraping
process.noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)


process.load("Validation.EcalValidation.ecalvalidation_cfi")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('EcalValidation.root')
)

process.p = cms.Path(
    process.skimming*
    process.hltLevel1GTSeed*
    process.hltHighLevel*
    process.noscraping*
    process.primaryVertexFilter*
    process.ecalvalidation
    )

