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

  printTree = cms.untracked.bool(True),

  processName = cms.string("HLT"),
  triggerName = cms.string("@"),
  triggerResults = cms.InputTag("TriggerResults","","HLT"),
  triggerEvent   = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
#  HLTPaths = cms.vstring("@"),    # For all paths
#  HLTPaths = cms.vstring("HLT_Mu*"),
  HLTPaths = cms.vstring(
    "HLT_MET120_HBHENoiseCleaned_v*",
    "HLT_MET200_v*",
    "HLT_PFMET150_v*",
    "HLT_Ele27_WP80_CentralPFJet80_v*",
    "HLT_Ele27_WP80_PFMET_MT50_v*",
    "HLT_Ele27_WP80_v*",
    "HLT_IsoMu20_eta2p1_CentralPFJet80_v*",
    "HLT_IsoMu24_eta2p1_v*",
    "HLT_IsoMu24_v*",
    "HLT_IsoMu30_v*",
    "HLT_Mu40_v*",
    "HLT_Mu40_eta2p1_v*",
    "HLT_LooseIsoPFTau35_Trk20_Prong1_v*",
    "HLT_LooseIsoPFTau35_Trk20_Prong1_MET70_v*",
    "HLT_Ele13_eta2p1_WP90Rho_LooseIsoPFTau20_v*",
    "HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v*",
    "HLT_IsoMu8_eta2p1_LooseIsoPFTau20_v*",
    "HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v*",
    "HLT_Ele20_CaloIdVT_CaloIsoRhoT_Trk_IdT_TrkIsoT_LooseIsoPFTau20_v*",
    "HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v*",
  ),
)
