import FWCore.ParameterSet.Config as cms

validator = cms.EDAnalyzer('MyValidator',
  verbose = cms.untracked.bool(False),
  modelTag = cms.string(""),

  genjets = cms.InputTag('ak5GenJets'),
  genparticles = cms.InputTag("genParticles"),
  pfmet = cms.InputTag("pfMet"),
  pfcandidates = cms.InputTag("particleFlow"),
  pfjets = cms.InputTag("ak5PFJets"),

  pftaus = cms.InputTag("hpsPFTauProducer"),

  leptonCut = cms.double(10),
  tauCut = cms.double(40),
  jetCut = cms.double(40),
  jetEtaCut = cms.double(2.1),

  trigger = cms.InputTag("hltTriggerSummaryAOD"),
)
