import FWCore.ParameterSet.Config as cms

validator = cms.EDAnalyzer('MyValidator',
  modelTag = cms.string(""),

  genjets = cms.InputTag('ak5GenJets'),
  genparticles = cms.InputTag("genParticles"),
)
