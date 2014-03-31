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
#include <iostream>

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

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"



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

      // ----------member data ---------------------------
      bool verbose;

      std::string param_modelTag_;

      edm::InputTag inputTag_GenParticles_;
      edm::InputTag inputTag_GenJets_;


      TH1F* hGenMET;
      TH1F* hGenNElectrons;
      TH1F* hGenElectron1Pt;
      TH1F* hGenNMuons;
      TH1F* hGenMuon1Pt;
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
   verbose = iConfig.getParameter<bool>("verbose");

   param_modelTag_ = iConfig.getParameter<std::string>("modelTag");

   inputTag_GenParticles_ = iConfig.getParameter<edm::InputTag>("genparticles");
   inputTag_GenJets_      = iConfig.getParameter<edm::InputTag>("genjets");


   edm::Service<TFileService> rootFile;
   hGenMET = rootFile->make<TH1F>("hGenMET", "Gen MET", 300, 0, 3000);
   hGenNElectrons =rootFile->make<TH1F>("hGenNElectrons", "Gen NElectrons", 20, 0, 20);
   hGenElectron1Pt = rootFile->make<TH1F>("hGenElectron1Pt", "1st Gen electron pt", 200, 0, 2000);
   hGenNMuons =rootFile->make<TH1F>("hGenNMuons", "Gen NMuons", 20, 0, 20);
   hGenMuon1Pt = rootFile->make<TH1F>("hGenMuon1Pt", "1st Gen muon pt", 200, 0, 2000);
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


  reco::Particle::PolarLorentzVector genparticleP4 = PolarLorentzVector(0,0,0,0);
  int    nGenElectrons=0,    nGenMuons=0;
  double maxGenElectronPt=0, maxGenMuonPt=0;


  for(genparticle = genparticles->begin(); genparticle != genparticles->end(); ++genparticle)
  {
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

  hGenMET->Fill(genparticleP4.pt());
  hGenNElectrons->Fill(nGenElectrons);
  if(nGenElectrons != 0)
    hGenElectron1Pt->Fill(maxGenElectronPt);
  hGenNMuons->Fill(nGenMuons);
  if(nGenMuons != 0)
    hGenMuon1Pt->Fill(maxGenMuonPt);
  //assert(1<0);
}


// ------------ method called once each job just before starting event loop  ------------
void 
MyValidator::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyValidator::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
MyValidator::beginRun(edm::Run const&, edm::EventSetup const&)
{
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
MyValidator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyValidator);
