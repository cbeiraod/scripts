import FWCore.ParameterSet.Config as cms

validator = cms.EDAnalyzer('MyValidator',
  verbose = cms.bool(False),
  modelTag = cms.string(""),

  genjets = cms.InputTag('ak5GenJets'),
  genparticles = cms.InputTag("genParticles"),
  pfmet = cms.InputTag("pfMet"),
  pfcandidates = cms.InputTag("particleFlow"),
  pfjets = cms.InputTag("ak5PFJets"),

  pftaus = cms.InputTag("hpsPFTauProducer"),
)
