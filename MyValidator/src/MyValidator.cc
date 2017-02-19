// -*- C++ -*-
//
// Package:    MyValidator
// Class:      MyValidator
// 
/**\class MyValidator MyValidator.cc UserCode/MyValidator/src/MyValidator.cc

 Description: EDAnalyzer to process after MC gen, for an overview of several parameters

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Cristovao B da Cruz e Silva 
//         Created:  Seg Mar 31 14:39:36 WEST 2014
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <map>
//#include <regex>
#include <boost/regex.hpp>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/TauReco/interface/PFTau.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"


//TriggerEvent
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

//TriggerResults
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "FWCore/Utilities/interface/RegexMatch.h"


//
// class declaration
//

class MyValidator : public edm::EDAnalyzer {
   public:
      explicit MyValidator(const edm::ParameterSet&);
      ~MyValidator();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


      typedef math::PtEtaPhiMLorentzVector PolarLorentzVector;

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      void printParticleAndDaughters(const reco::GenParticle* particle, std::string prefix, bool last=false);

      // ----------member data ---------------------------
      bool verbose;

      std::string param_modelTag_;

      edm::InputTag inputTag_GenParticles_;
      edm::InputTag inputTag_GenJets_;
      edm::InputTag inputTag_PFMET_;
      edm::InputTag inputTag_PFCandidates_;
      edm::InputTag inputTag_PFJets_;
      edm::InputTag inputTag_PFTaus_;
      edm::InputTag inputTag_Trigger_;
      edm::InputTag inputTag_TriggerResults_;
      edm::InputTag inputTag_TriggerEvent_;

      std::string parameter_ProcessName_;
      std::string parameter_TriggerName_;

      std::vector<std::string>  HLTPatterns_;
      std::vector<std::string>  HLTPathsByName_;
      std::vector<unsigned int> HLTPathsByIndex_;
      std::vector<std::string>  HLTPaths_;

      double parameter_LeptonCut_;
      double parameter_TauCut_;
      double parameter_JetCut_;
      double parameter_JetEtaCut_;

      bool parameter_printTree_;
      bool parameter_FullTree_;


      TH1F* stauM;
      TH1F* neutralinoM;

      TH1F* hGenMET;
      TH1F* hGenNElectrons;
      TH1F* hGenElectron1Pt;
      TH1F* hGenNMuons;
      TH1F* hGenMuon1Pt;
      TH1F* hGenHT;
      TH1F* hGenNJets;
      TH1F* hGenJet1Pt;
      TH1F* hPFMET;
      TH1F* hNElectrons;
      TH1F* hElectron1Pt;
      TH1F* hNMuons;
      TH1F* hMuon1Pt;
      TH1F* hHT;
      TH1F* hNJets;
      TH1F* hJet1Pt;
      TH1F* hNTaus;
      TH1F* hTau1Pt;

      TH1F* hGenElectron1PtFilt;
      TH1F* hElectron1PtFilt;
      TH1F* hGenMuon1PtFilt;
      TH1F* hMuon1PtFilt;
      TH1F* hGenElectron1PtFilt2;
      TH1F* hElectron1PtFilt2;
      TH1F* hGenMuon1PtFilt2;
      TH1F* hMuon1PtFilt2;

      TH1F* hNElectronsCut;
      TH1F* hElectron1PtCut;
      TH1F* hNMuonsCut;
      TH1F* hMuon1PtCut;
      TH1F* hHTCut;
      TH1F* hNJetsCut;
      TH1F* hJet1PtCut;
      TH1F* hNTausCut;
      TH1F* hTau1PtCut;

      TH1F* hInvMass;
      TH1F* hInvMassCut;


      edm::Handle<edm::TriggerResults>   triggerResultsHandle_;
      edm::Handle<trigger::TriggerEvent> triggerEventHandle_;
      HLTConfigProvider hltConfig_;

      bool doAllHLT;

      unsigned long long nEvents, nEvents2;
      std::map<std::string, unsigned long long> filteredEvents;

      unsigned long long nee, nmumu, nemu, neh, nmuh, nhh;
      unsigned long long ne, nmu;
      unsigned long long nee2, nmumu2, nemu2, neh2, nmuh2, nhh2;
      unsigned long long ne2, nmu2;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MyValidator::MyValidator(const edm::ParameterSet& iConfig)
{
   verbose = iConfig.getUntrackedParameter<bool>("verbose");

   param_modelTag_ = iConfig.getParameter<std::string>("modelTag");

   inputTag_GenParticles_ = iConfig.getParameter<edm::InputTag>("genparticles");
   inputTag_GenJets_      = iConfig.getParameter<edm::InputTag>("genjets");
   inputTag_PFMET_        = iConfig.getParameter<edm::InputTag>("pfmet");
   inputTag_PFCandidates_ = iConfig.getParameter<edm::InputTag>("pfcandidates");
   inputTag_PFJets_       = iConfig.getParameter<edm::InputTag>("pfjets");
   inputTag_PFTaus_       = iConfig.getParameter<edm::InputTag>("pftaus");

   inputTag_Trigger_        = iConfig.getParameter<edm::InputTag>("trigger");
   inputTag_TriggerResults_ = iConfig.getParameter<edm::InputTag>("triggerResults");
   inputTag_TriggerEvent_   = iConfig.getParameter<edm::InputTag>("triggerEvent");
   parameter_TriggerName_   = iConfig.getParameter<std::string>("triggerName");
   parameter_ProcessName_   = iConfig.getParameter<std::string>("processName");
   HLTPatterns_             = iConfig.getParameter<std::vector<std::string> >("HLTPaths");
   doAllHLT                 = false;
   for(auto i = HLTPatterns_.begin(); i != HLTPatterns_.end(); ++i)
   {
     if(*i == "@")
     {
       doAllHLT = true;
       HLTPatterns_.clear();
       break;
     }
   }

   parameter_LeptonCut_ = iConfig.getParameter<double>("leptonCut");
   parameter_TauCut_    = iConfig.getParameter<double>("tauCut");
   parameter_JetCut_    = iConfig.getParameter<double>("jetCut");
   parameter_JetEtaCut_ = iConfig.getParameter<double>("jetEtaCut");

   parameter_printTree_ = iConfig.getUntrackedParameter<bool>("printTree");
   parameter_FullTree_  = iConfig.getUntrackedParameter<bool>("FullTree");



   edm::Service<TFileService> rootFile;
   hGenMET = rootFile->make<TH1F>("hGenMET", "Gen MET;MET (GeV);Events", 70, 0, 700);
   hGenNElectrons = rootFile->make<TH1F>("hGenNElectrons", "Gen NElectrons;N;Events", 12, 0, 12);
   hGenElectron1Pt = rootFile->make<TH1F>("hGenElectron1Pt", "1st Gen electron pt;P_{t} (GeV);Events", 60, 0, 600);
   hGenNMuons = rootFile->make<TH1F>("hGenNMuons", "Gen NMuons;N;Events", 5, 0, 5);
   hGenMuon1Pt = rootFile->make<TH1F>("hGenMuon1Pt", "1st Gen muon pt;P_{t} (GeV);Events", 60, 0, 600);
   hGenHT = rootFile->make<TH1F>("hGenHT", "Gen HT;HT (GeV);Events", 200, 0, 2000);
   hGenNJets = rootFile->make<TH1F>("hGenNJets", "Gen NJets;N;Events", 100, 0, 100);
   hGenJet1Pt = rootFile->make<TH1F>("hGenJet1Pt", "1st Gen jet pt;P_{t} (GeV);Events", 80, 0, 800);

   hPFMET = rootFile->make<TH1F>("hPFMET", "MET;MET (GeV);Events", 70, 0, 700);
   hNElectrons = rootFile->make<TH1F>("hNElectrons", "NElectrons;N;Events", 12, 0, 12);
   hElectron1Pt = rootFile->make<TH1F>("hElectron1Pt", "1st electron pt;P_{t} (GeV);Events", 60, 0, 600);
   hNMuons = rootFile->make<TH1F>("hNMuons", "NMuons;N;Events", 5, 0, 5);
   hMuon1Pt = rootFile->make<TH1F>("hMuon1Pt", "1st muon pt;P_{t} (GeV);Events", 60, 0, 600);
   hHT = rootFile->make<TH1F>("hHT", "HT;HT (GeV);Events", 200, 0, 2000);
   hNJets = rootFile->make<TH1F>("hNJets", "NJets;N;Events", 100, 0, 100);
   hJet1Pt = rootFile->make<TH1F>("hJet1Pt", "1st jet pt;P_{t} (GeV);Events", 80, 0, 800);

   hNTaus = rootFile->make<TH1F>("hNTaus", "NTaus;N;Events", 100, 0, 100);
   hTau1Pt = rootFile->make<TH1F>("hTau1Pt", "1st tau pt;P_{t} (GeV);Events", 70, 0, 700);

   hGenElectron1PtFilt = rootFile->make<TH1F>("hGenElectron1PtFilt", "1st Gen electron pt;P_{t} (GeV);Events", 60, 0, 600);
   hElectron1PtFilt = rootFile->make<TH1F>("hElectron1PtFilt", "1st electron pt;P_{t} (GeV);Events", 60, 0, 600);
   hGenMuon1PtFilt = rootFile->make<TH1F>("hGenMuon1PtFilt", "1st Gen muon pt;P_{t} (GeV);Events", 60, 0, 600);
   hMuon1PtFilt = rootFile->make<TH1F>("hMuon1PtFilt", "1st muon pt;P_{t} (GeV);Events", 60, 0, 600);
   hGenElectron1PtFilt2 = rootFile->make<TH1F>("hGenElectron1PtFilt2", "1st Gen electron pt;P_{t} (GeV);Events", 60, 0, 600);
   hElectron1PtFilt2 = rootFile->make<TH1F>("hElectron1PtFilt2", "1st electron pt;P_{t} (GeV);Events", 60, 0, 600);
   hGenMuon1PtFilt2 = rootFile->make<TH1F>("hGenMuon1PtFilt2", "1st Gen muon pt;P_{t} (GeV);Events", 60, 0, 600);
   hMuon1PtFilt2 = rootFile->make<TH1F>("hMuon1PtFilt2", "1st muon pt;P_{t} (GeV);Events", 60, 0, 600);

   hNElectronsCut = rootFile->make<TH1F>("hNElectronsCut", "NElectrons;N;Events", 12, 0, 12);
   hElectron1PtCut = rootFile->make<TH1F>("hElectron1PtCut", "1st electron pt;P_{t} (GeV);Events", 60, 0, 600);
   hNMuonsCut = rootFile->make<TH1F>("hNMuonsCut", "NMuons;N;Events", 5, 0, 5);
   hMuon1PtCut = rootFile->make<TH1F>("hMuon1PtCut", "1st muon pt;P_{t} (GeV);Events", 60, 0, 600);
   hHTCut = rootFile->make<TH1F>("hHTCut", "HT;HT (GeV);Events", 200, 0, 2000);
   hNJetsCut = rootFile->make<TH1F>("hNJetsCut", "NJets;N;Events", 100, 0, 100);
   hJet1PtCut = rootFile->make<TH1F>("hJet1PtCut", "1st jet pt;P_{t} (GeV);Events", 80, 0, 800);
   hNTausCut = rootFile->make<TH1F>("hNTausCut", "NTaus;N;Events", 100, 0, 100);
   hTau1PtCut = rootFile->make<TH1F>("hTau1PtCut", "1st tau pt;P_{t} (GeV);Events", 70, 0, 700);

   stauM = rootFile->make<TH1F>("stauM", "stau Mass;M (GeV);", 500, 0, 500);
   neutralinoM = rootFile->make<TH1F>("neutralinoM", "neutralino Mass;M (GeV);", 500, 0, 500);

   hInvMass = rootFile->make<TH1F>("InvMass", "LepTau Invariant Mass;M (GeV);Events", 30, 0, 300);
   hInvMassCut = rootFile->make<TH1F>("InvMassCut", "LepTau Invariant Mass;M (GeV);Events", 30, 0, 300);
}


MyValidator::~MyValidator()
{ 
   // do anything here that needs to be done at desctruction time
}


//
// member functions
//

// ------------ method called for each event  ------------
void
MyValidator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<reco::GenParticleCollection> genparticles;
  iEvent.getByLabel(inputTag_GenParticles_, genparticles);
  reco::GenParticleCollection::const_iterator genparticle;

  edm::Handle<reco::GenJetCollection> genjets;
  iEvent.getByLabel(inputTag_GenJets_, genjets);
  reco::GenJetCollection::const_iterator genjet;

  edm::Handle<reco::PFMETCollection> pfMETs;
  iEvent.getByLabel(inputTag_PFMET_, pfMETs);
  reco::PFMETCollection::const_iterator pfMET = pfMETs->begin();

  edm::Handle<reco::PFCandidateCollection> pfcandidates;
  iEvent.getByLabel(inputTag_PFCandidates_, pfcandidates);
  reco::PFCandidateCollection::const_iterator pfcandidate;

  edm::Handle<reco::PFJetCollection> pfjets;
  iEvent.getByLabel(inputTag_PFJets_, pfjets);
  reco::PFJetCollection::const_iterator pfjet;

  edm::Handle<reco::PFTauCollection> pftaus;
  iEvent.getByLabel(inputTag_PFTaus_, pftaus);
  reco::PFTauCollection::const_iterator pftau;


  reco::Particle::PolarLorentzVector genparticleP4 = PolarLorentzVector(0,0,0,0);
  int    nGenElectrons=0,    nGenMuons=0,    nGenJets=0;
  double maxGenElectronPt=0, maxGenMuonPt=0, maxGenJetPt=0, genHT=0;
  reco::Particle::PolarLorentzVector PFcandP4 = PolarLorentzVector(0,0,0,0);
  int    nElectrons=0,    nMuons=0,    nTaus=0,    nJets=0;
  double maxElectronPt=0, maxMuonPt=0, maxTauPt=0, maxJetPt=0, HT=0;
  int    nElectronsCut=0,    nMuonsCut=0,    nTausCut=0,    nJetsCut=0;
  double maxElectronPtCut=0, maxMuonPtCut=0, maxTauPtCut=0, maxJetPtCut=0, HTCut=0;

  reco::Particle::PolarLorentzVector lepton=PolarLorentzVector(0,0,0,0), tau=PolarLorentzVector(0,0,0,0);
  reco::Particle::PolarLorentzVector leptonCut=PolarLorentzVector(0,0,0,0), tauCut=PolarLorentzVector(0,0,0,0);


  for(genparticle = genparticles->begin(); genparticle != genparticles->end(); ++genparticle)
  {
    if(abs(genparticle->pdgId()) == 1000022) // neutralino
    {
      neutralinoM->Fill((genparticle->p4()).M());
    }
    if(abs(genparticle->pdgId()) == 1000015) // stau
    {
      stauM->Fill((genparticle->p4()).M());
    }
    if(genparticle->status() == 1) //Only "stable particles"
    {
      if(abs(genparticle->pdgId()) != 12      && // electron neutrino
         abs(genparticle->pdgId()) != 14      && // muon neutrino
         abs(genparticle->pdgId()) != 16      && // tau neutrino
         abs(genparticle->pdgId()) != 1000022 && // neutralino
         (genparticle->p4()).E()   != 0          // Particles at rest, to remove NaN results
         )
      {
        if(verbose)
        {
          std::cout << "Pt: " << (genparticle->p4()).pt() << std::endl;
          if(isnan((genparticle->p4()).pt()))
          {
            ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > tempP4 = genparticle->p4();
            std::cout << "\tE:" << tempP4.E() << std::endl;
            std::cout << "\tEt:" << tempP4.Et() << std::endl;
            std::cout << "\tEta:" << tempP4.Eta() << std::endl;
            std::cout << "\tM:" << tempP4.M() << std::endl;
            std::cout << "\tP2:" << tempP4.P2() << std::endl;
            std::cout << "\tPhi:" << tempP4.Phi() << std::endl;
            std::cout << "\tPx:" << tempP4.Px() << std::endl;
            std::cout << "\tPy:" << tempP4.Py() << std::endl;
            std::cout << "\tPz:" << tempP4.Pz() << std::endl;
          }
        }
        genparticleP4 += genparticle->p4();
      }

      if(abs(genparticle->pdgId()) == 11) // electron
      {
        ++nGenElectrons;
        if(genparticle->pt() > maxGenElectronPt)
          maxGenElectronPt = genparticle->pt();
      }

      if(abs(genparticle->pdgId()) == 13) // muon
      {
        ++nGenMuons;
        if(genparticle->pt() > maxGenMuonPt)
          maxGenMuonPt = genparticle->pt();
      }
    }
  }

  if(verbose)
  {
    std::cout << "GenParticle MET: " << genparticleP4.pt() << std::endl;
    std::cout << "\tE:" << genparticleP4.E() << std::endl;
    std::cout << "\tEt:" << genparticleP4.Et() << std::endl;
    std::cout << "\tEta:" << genparticleP4.Eta() << std::endl;
    std::cout << "\tM:" << genparticleP4.M() << std::endl;
    std::cout << "\tP2:" << genparticleP4.P2() << std::endl;
    std::cout << "\tPhi:" << genparticleP4.Phi() << std::endl;
    std::cout << "\tPx:" << genparticleP4.Px() << std::endl;
    std::cout << "\tPy:" << genparticleP4.Py() << std::endl;
    std::cout << "\tPz:" << genparticleP4.Pz() << std::endl;
  }

  for(genjet = genjets->begin(); genjet != genjets->end(); ++genjet)
  {
    ++nGenJets;
    genHT += genjet->pt();
    if(genjet->pt() > maxGenJetPt)
      maxGenJetPt = genjet->pt();
  }

  for(pfcandidate = pfcandidates->begin(); pfcandidate != pfcandidates->end(); ++pfcandidate)
  {
    PFcandP4 += pfcandidate->p4();

    if(abs(pfcandidate->pdgId()) == 11) // electron
    {
      ++nElectrons;
      if(pfcandidate->pt() > maxElectronPt)
      {
        maxElectronPt = pfcandidate->pt();
        lepton = pfcandidate->p4();
      }
    }

    if(abs(pfcandidate->pdgId()) == 13) // muon
    {
      ++nMuons;
      if(pfcandidate->pt() > maxMuonPt)
      {
        maxMuonPt = pfcandidate->pt();
        if(maxMuonPt > maxElectronPt)
          lepton = pfcandidate->p4();
      }
    }

    if(pfcandidate->pt() < parameter_LeptonCut_) continue;

    if(abs(pfcandidate->pdgId()) == 11) // electron
    {
      ++nElectronsCut;
      if(pfcandidate->pt() > maxElectronPtCut)
      {
        maxElectronPtCut = pfcandidate->pt();
        leptonCut = pfcandidate->p4();
      }
    }

    if(abs(pfcandidate->pdgId()) == 13) // muon
    {
      ++nMuonsCut;
      if(pfcandidate->pt() > maxMuonPtCut)
      {
        maxMuonPtCut = pfcandidate->pt();
        if(maxMuonPtCut > maxElectronPtCut)
          leptonCut = pfcandidate->p4();
      }
    }
  }

  for(pfjet = pfjets->begin(); pfjet != pfjets->end(); ++pfjet)
  {
    ++nJets;
    HT += pfjet->pt();
    if(pfjet->pt() > maxJetPt)
      maxJetPt = pfjet->pt();

    if(pfjet->pt() < parameter_JetCut_) continue;
    if(abs(pfjet->eta()) > parameter_JetEtaCut_) continue;

    ++nJetsCut;
    HTCut += pfjet->pt();
    if(pfjet->pt() > maxJetPtCut)
      maxJetPtCut = pfjet->pt();
  }

  for(pftau = pftaus->begin(); pftau != pftaus->end(); ++pftau)
  {
    ++nTaus;
    if(pftau->pt() > maxTauPt)
    {
      maxTauPt = pftau->pt();
      tau = pftau->p4();
    }

    if(pftau->pt() < parameter_TauCut_) continue;

    ++nTausCut;
    if(pftau->pt() > maxTauPtCut)
    {
      maxTauPtCut = pftau->pt();
      tauCut = pftau->p4();
    }
  }



  hGenMET->Fill(genparticleP4.pt());
  hGenNElectrons->Fill(nGenElectrons);
  if(nGenElectrons != 0)
    hGenElectron1Pt->Fill(maxGenElectronPt);
  hGenNMuons->Fill(nGenMuons);
  if(nGenMuons != 0)
    hGenMuon1Pt->Fill(maxGenMuonPt);
  hGenHT->Fill(genHT);
  hGenNJets->Fill(nGenJets);
  if(nGenJets != 0)
    hGenJet1Pt->Fill(maxGenJetPt);

  hPFMET->Fill(pfMET->pt());
  hNElectrons->Fill(nElectrons);
  if(nElectrons != 0)
    hElectron1Pt->Fill(maxElectronPt);
  hNMuons->Fill(nMuons);
  if(nMuons != 0)
    hMuon1Pt->Fill(maxMuonPt);
  hHT->Fill(HT);
  hNJets->Fill(nJets);
  if(nJets != 0)
    hJet1Pt->Fill(maxJetPt);
  hNTaus->Fill(nTaus);
  if(nTaus != 0)
    hTau1Pt->Fill(maxTauPt);

  hNElectronsCut->Fill(nElectronsCut);
  if(nElectronsCut != 0)
    hElectron1PtCut->Fill(maxElectronPtCut);
  hNMuonsCut->Fill(nMuonsCut);
  if(nMuonsCut != 0)
    hMuon1PtCut->Fill(maxMuonPtCut);
  hHTCut->Fill(HTCut);
  hNJetsCut->Fill(nJetsCut);
  if(nJetsCut != 0)
    hJet1PtCut->Fill(maxJetPtCut);
  hNTausCut->Fill(nTausCut);
  if(nTausCut != 0)
    hTau1PtCut->Fill(maxTauPtCut);

  if(nTaus != 0 && nMuons+nElectrons != 0)
    hInvMass->Fill((lepton+tau).M());
  if(nTausCut != 0 && nMuonsCut+nElectronsCut != 0)
    hInvMassCut->Fill((leptonCut+tauCut).M());


  if(verbose)
    std::cout << "Content of TriggerEvent: " << inputTag_Trigger_.encode() << std::endl;

  edm::Handle<trigger::TriggerEvent> triggerHandle;
  iEvent.getByLabel(inputTag_Trigger_, triggerHandle);
  if(triggerHandle.isValid())
  {
    if(verbose)
    {
      std::cout << "\tUsed Processname: " << triggerHandle->usedProcessName() << std::endl;

      const trigger::size_type nC(triggerHandle->sizeCollections());
      std::cout << "\tNumber of packed Collections: " << nC << std::endl;
      std::cout << "\t  The Collections: #, tag, 1-past-end index" << std::endl;
      for(trigger::size_type iC=0; iC!=nC; ++iC)
      {
        std::cout << "\t  " << iC << ", " << triggerHandle->collectionTag(iC).encode() << ", " << triggerHandle->collectionKey(iC) << std::endl;
      }

      const trigger::size_type nO(triggerHandle->sizeObjects());
      std::cout << "\tNumber of TriggerObjects: " << nO << std::endl;
      std::cout << "\t  The TriggerObjects: #, id, pt, eta, phi, mass" << std::endl;
      const trigger::TriggerObjectCollection& TOC(triggerHandle->getObjects());
      for(trigger::size_type iO=0; iO!=nO; ++iO)
      {
        const trigger::TriggerObject& TO(TOC[iO]);
        std::cout << "\t  " << iO << " " << TO.id() << " " << TO.pt() << " " << TO.eta() << " " << TO.phi() << " " << TO.mass() << std::endl;
      }

      const trigger::size_type nF(triggerHandle->sizeFilters());
      std::cout << "\tNumber of TriggerFilters: " << nF << std::endl;
      std::cout << "\t  The Filters: #, tag, #ids/#keys, the id/key pairs" << std::endl;
      for(trigger::size_type iF=0; iF!=nF; ++iF)
      {
        const trigger::Vids& VIDS (triggerHandle->filterIds(iF));
        const trigger::Keys& KEYS(triggerHandle->filterKeys(iF));
        const trigger::size_type nI(VIDS.size());
        const trigger::size_type nK(KEYS.size());

        std::cout << "\t  " << iF << " " << triggerHandle->filterTag(iF).encode() << " " << nI << "/" << nK << " the pairs: ";
        const trigger::size_type n((nK>nI)?nK:nI);
        for(trigger::size_type i = 0; i != n; ++i)
        {
          std::cout << " " << VIDS[i] << "/" << KEYS[i];
        }
        std::cout << std::endl;
        assert(nK == nI);
      }
    }
  }
  else
  {
    std::cout << "Handle invalid! Check InputTag provided." << std::endl;
    assert(triggerHandle.isValid());
  }


//  edm::Handle<edm::TriggerResults> trigResults;
//  edm::InputTag trigResultsTag("TriggerResults","","HLT");
//  iEvent.getByLabel(trigResultsTag,trigResults);

  iEvent.getByLabel(inputTag_TriggerResults_,triggerResultsHandle_);
  if(!triggerResultsHandle_.isValid())
  {
    std::cout << "Error in getting TriggerResults product from Event!" << std::endl;
    assert(triggerResultsHandle_.isValid());
  }
  const edm::TriggerNames& trigNames = iEvent.triggerNames(*triggerResultsHandle_);

  unsigned int n = trigNames.size();
  if(verbose)
  {
    std::cout << "N trig: " << n << std::endl;
    for(unsigned int i  = 0; i < n; ++i)
    {
      std::cout << "\t" << trigNames.triggerName(i) << ": ";
      std::cout << "WasRun=" << triggerResultsHandle_->wasrun(i) << "; ";
      std::cout << "Accept=" << triggerResultsHandle_->accept(i) << "; ";
      std::cout << "Error=" << triggerResultsHandle_->error(i) << "; " << std::endl;
    }
  }
  HLTPathsByName_.clear();
  HLTPathsByIndex_.clear();
  HLTPaths_.clear();
  if(doAllHLT)
  {
    for(unsigned int i = 0; i < n; ++i)
    {
      HLTPathsByName_.push_back(trigNames.triggerName(i));
      HLTPathsByIndex_.push_back(i);
    }
  }
  else
  {
    for(auto pattern = HLTPatterns_.begin(); pattern != HLTPatterns_.end(); ++pattern)
    {
      if(edm::is_glob(*pattern))
      {
        std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(trigNames.triggerNames(), *pattern);
        if(!matches.empty())
          for(auto i = matches.begin(); i != matches.end(); ++i)
            HLTPathsByName_.push_back(*(*i));
      }
      else
      {
        HLTPathsByName_.push_back(*pattern);
      }
    }

    HLTPathsByIndex_.resize(HLTPathsByName_.size());
    for(unsigned int i = 0; i < HLTPathsByName_.size(); ++i)
    {
      HLTPathsByIndex_[i] = trigNames.triggerIndex(HLTPathsByName_[i]);
    }
  }
  HLTPaths_.resize(HLTPathsByName_.size());
  boost::regex re ("(^.+)(_v[0-9]+$)");
  std::string temp = "$1";
  for(unsigned int i = 0; i < HLTPathsByName_.size(); ++i)
  {
    if(HLTPathsByIndex_[i] >= n) continue;
    HLTPaths_[i] = boost::regex_replace(HLTPathsByName_[i], re, temp);
    if(triggerResultsHandle_->accept(HLTPathsByIndex_[i]))
    {
      if(filteredEvents.find(HLTPaths_[i]) == filteredEvents.end())
        filteredEvents[HLTPaths_[i]] = 0;
      filteredEvents[HLTPaths_[i]] = filteredEvents[HLTPaths_[i]] + 1;
    }
  }
  ++nEvents;

  std::vector<const reco::GenParticle*> allGenParticles;
  std::vector<const reco::GenParticle*> staus;
  std::vector<const reco::GenParticle*> leptons;
  for(genparticle = genparticles->begin(); genparticle != genparticles->end(); ++genparticle)
  {
    allGenParticles.push_back(& * genparticle);
    if(abs(genparticle->pdgId()) == 1000015 && genparticle->status() == 3) //Find staus from Matrix Element, save them to start tree creation
      staus.push_back(& * genparticle);
    if((abs(genparticle->pdgId()) == 11 || abs(genparticle->pdgId()) == 13) && genparticle->status() == 1)
      leptons.push_back(& * genparticle);
  }

  unsigned long nLeptons = 0;
  bool el = false, mu = false;
  bool veto = false;
  for(auto lepton = leptons.begin(); lepton != leptons.end(); ++lepton)
  {
    reco::GenParticle* isStau = (reco::GenParticle*)(((*lepton)->mother())->mother());
    if(abs(isStau->pdgId()) == 15 && isStau->status() == 3)
    {
      ++nLeptons;
      if(abs((*lepton)->pdgId()) == 11)
        el = true;
      else
        mu = true;
      if(((*lepton)->p4()).E() == 0)
        veto = true;
    }
  }

  if(nGenElectrons != 0 && el)
    hGenElectron1PtFilt->Fill(maxGenElectronPt);
  if(nElectrons != 0 && el)
    hElectron1PtFilt->Fill(maxElectronPt);
  if(nGenMuons != 0 && mu)
    hGenMuon1PtFilt->Fill(maxGenMuonPt);
  if(nMuons != 0 && mu)
    hMuon1PtFilt->Fill(maxMuonPt);
  if(nGenElectrons != 0 && el && !veto)
    hGenElectron1PtFilt2->Fill(maxGenElectronPt);
  if(nElectrons != 0 && el && !veto)
    hElectron1PtFilt2->Fill(maxElectronPt);
  if(nGenMuons != 0 && mu && !veto)
    hGenMuon1PtFilt2->Fill(maxGenMuonPt);
  if(nMuons != 0 && mu && !veto)
    hMuon1PtFilt2->Fill(maxMuonPt);
  if(!veto)
    ++nEvents2;

  assert(nLeptons <= 2);
  switch(nLeptons)
  {
  case 0:
    ++nhh;
    if(!veto)
      ++nhh2;
    break;
  case 1:
    if(el)
    {
      ++neh;
      ++ne;
      if(!veto)
      {
        ++neh2;
        ++ne2;
      }
    }
    else
    {
      ++nmuh;
      ++nmu;
      if(!veto)
      {
        ++nmuh2;
        ++nmu2;
      }
    }
    break;
  case 2:
    if(el && mu)
    {
      ++nemu;
      ++ne;
      ++nmu;
      if(!veto)
      {
        ++nemu2;
        ++ne2;
        ++nmu2;
      }
    }
    else
    {
      if(el)
      {
        ++nee;
        ++ne;
        if(!veto)
        {
          ++nee2;
          ++ne2;
        }
      }
      else
      {
        ++nmumu;
        ++nmu;
        if(!veto)
        {
          ++nmumu2;
          ++nmu2;
        }
      }
    }
    break;
  }

  if(parameter_printTree_)
  {
    std::cout << "Event " << iEvent.id() << std::endl;

    for(size_t i = 0; i < staus.size(); ++i)
    {
      if(i == staus.size()-1)
        printParticleAndDaughters((staus[i]), "  ", true);
      else
        printParticleAndDaughters((staus[i]), "  ");
    }

    std::cout << std::endl;
  }
//  assert(1<0);
//  assert(!veto);
}

void MyValidator::printParticleAndDaughters(const reco::GenParticle* particle, std::string prefix, bool last)
{
  std::cout << prefix;
  if(last)
    std::cout << "╚ ";
  else
    std::cout << "╠ ";

  bool sign = (particle->pdgId() > 0);
  switch(abs(particle->pdgId()))
  {
  case 1:
    if(!sign)
      std::cout << "~";
    std::cout << "d";
    break;
  case 2:
    if(!sign)
      std::cout << "~";
    std::cout << "u";
    break;
  case 3:
    if(!sign)
      std::cout << "~";
    std::cout << "s";
    break;
  case 4:
    if(!sign)
      std::cout << "~";
    std::cout << "c";
    break;
  case 5:
    if(!sign)
      std::cout << "~";
    std::cout << "b";
    break;
  case 6:
    if(!sign)
      std::cout << "~";
    std::cout << "t";
    break;
  case 11:
    std::cout << "Electron";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 12:
    std::cout << "Nu_Electron";
    break;
  case 13:
    std::cout << "Mu";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 14:
    std::cout << "Nu_Mu";
    break;
  case 15:
    std::cout << "Tau";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 16:
    std::cout << "Nu_Tau";
    break;
  case 21:
    std::cout << "g";
    break;
  case 22:
    std::cout << "Gamma";
    break;
  case 23:
    std::cout << "Z0";
    break;
  case 24:
    std::cout << "W";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 25:
    std::cout << "H0";
    break;
  case 111:
    std::cout << "Pi0";
    break;
  case 113:
    std::cout << "rho0";
    break;
  case 130:
    std::cout << "KL0";
    break;
  case 211:
    std::cout << "Pi";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 213:
    std::cout << "rho";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 310:
    std::cout << "KS0";
    break;
  case 321:
    std::cout << "K";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 20213:
    std::cout << "a1";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 1000015:
    std::cout << "Stau";
    if(sign)
      std::cout << "+";
    else
      std::cout << "-";
    break;
  case 1000022:
    std::cout << "Neutralino";
    break;
  default:
    std::cout << "Unknown";
  }
  std::cout << " {pdgId: " << particle->pdgId() << "}";
  std::cout << " {status: " << particle->status() << "}";

  if(parameter_FullTree_)
  {
    std::cout << " {E: " << particle->p4().E() << "}";
    std::cout << " {P: (" << particle->p4().Px() << ", " << particle->p4().Py() << ", " << particle->p4().Pz() << ")}";
  }

  std::cout << std::endl;

  std::string newPrefix = prefix;
  if(last)
    newPrefix += "   ";
  else
    newPrefix += "║  ";

  int n = particle->numberOfDaughters();
  for(int i = 0; i < n; ++i)
  {
    if(i == n-1)
      printParticleAndDaughters((reco::GenParticle*)(particle->daughter(i)), newPrefix, true);
    else
      printParticleAndDaughters((reco::GenParticle*)(particle->daughter(i)), newPrefix);
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
MyValidator::beginJob()
{
  nEvents = 0;
  nEvents2 = 0;

  nee = 0;
  nmumu = 0;
  nemu = 0;
  neh = 0;
  nmuh = 0;
  nhh = 0;
  ne = 0;
  nmu = 0;

  nee2 = 0;
  nmumu2 = 0;
  nemu2 = 0;
  neh2 = 0;
  nmuh2 = 0;
  nhh2 = 0;
  ne2 = 0;
  nmu2 = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyValidator::endJob() 
{
  std::cout << "Total events: " << nEvents << " (" << nEvents2 << ")" << std::endl;
  std::cout << "Events with electrons: " << ne << " (" << ne2 << ")" << std::endl;
  std::cout << "Events with muons: " << nmu << " (" << nmu2 << ")" << std::endl;
  std::cout << " Di-electron events: " << nee << " (" << nee2 << ")" << std::endl;
  std::cout << " Di-muon events: " << nmumu << " (" << nmumu2 << ")" << std::endl;
  std::cout << " Electron-Muon events: " << nemu << " (" << nemu2 << ")" << std::endl;
  std::cout << " Full hadronic events: " << nhh << " (" << nhh2 << ")" << std::endl;
  std::cout << " Semi-leptonic events (electron): " << neh << " (" << neh2 << ")" << std::endl;
  std::cout << " Semi-leptonic events (muon): " << nmuh << " (" << nmuh2 << ")" << std::endl;

  std::cout << "HLT:" << std::endl;
  for(auto i = filteredEvents.begin(); i != filteredEvents.end(); ++i)
  {
    std::cout << "  " << i->first << ": " << i->second << " => " << ((double)(i->second) * 100.)/nEvents << "% (" << ((double)(i->second) * 100.)/nEvents2 << ")" << std::endl;
  }
}

// ------------ method called when starting to processes a run  ------------
void 
MyValidator::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
//  bool changed(true);
//  if(hltConfig_.init(iRun,iSetup,parameter_ProcessName_,changed))
//  {
//    if(changed)
//    {
//      if(parameter_TriggerName_!="@")
//      {
//        const unsigned int n(hltConfig_.size());
//        const unsigned int triggerIndex(hltConfig_.triggerIndex(parameter_TriggerName_));
//        if(triggerIndex >= n)
//        {
//          std::cout << "TriggerName: " << parameter_TriggerName_ << " not available in (new) config!" << std::endl;
//          std::cout << "Available TriggerNames are:" << std::endl;
//          hltConfig_.dump("Triggers");
//        }
//      }
//    }
//    hltConfig_.dump("ProcessName");
//    hltConfig_.dump("GlobalTag");
//    hltConfig_.dump("TableName");
//    hltConfig_.dump("Streams");
//    hltConfig_.dump("Datasets");
//    hltConfig_.dump("PrescaleTable");
//    hltConfig_.dump("ProcessPSet");
//  }
//  else
//  {
//    std::cout << "Config extraction failed with process name: " << parameter_ProcessName_ << std::endl;
//  }
}

// ------------ method called when ending the processing of a run  ------------
void 
MyValidator::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MyValidator::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MyValidator::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyValidator::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyValidator);
