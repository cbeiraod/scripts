import FWCore.ParameterSet.Config as cms

process = cms.Process("Validator")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

inputFiles = cms.untracked.vstring()
inputFiles.extend([
  "file:/lustre/ncg.ingrid.pt/cmst3/store/user/cbeiraod/TStauStau/TStauStau_100_0_8E33/TStauStau_100_0_8E33v2_1_1_YIn.root",
])

process.source = cms.Source("PoolSource",
    fileNames = inputFiles,
#    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("Outfile.root"),
)


#process.validator = cms.EDAnalyzer('MyValidator')
process.load("UserCode.MyValidator.myvalidator_cfi")

#process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))


process.p = cms.Path(process.validator)
