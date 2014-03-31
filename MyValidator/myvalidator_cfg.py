import FWCore.ParameterSet.Config as cms

process = cms.Process("Validator")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

inputFiles = cms.untracked.vstring()
inputFiles.extend([
])

process.source = cms.Source("PoolSource",
    fileNames = inputFiles,
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("Outfile.root"),
)


process.validator = cms.EDAnalyzer('MyValidator',
)


process.p = cms.Path(process.validator)
