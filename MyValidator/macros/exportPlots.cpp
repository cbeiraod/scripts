#include<iostream>
#include<vector>
#include<map>

#include<TROOT.h>
#include<TSystem.h>
#include<TFile.h>
#include<TDirectory.h>
#include<TCanvas.h>
#include<TH1F.h>

int exportPlots(std::string outdir, std::string rootFile, std::string basename)
{
  std::cout << "I am running" << std::endl;
  std::map<std::string, std::string> plots;

  TFile inFile(rootFile.c_str(), "READ");
  TDirectory* plotDir = (TDirectory*) inFile.Get("validator");
//  plotDir->ls();

  plots["hGenMET"]         = "GenMET";
  plots["hGenNElectrons"]  = "GenNElectrons";
  plots["hGenElectron1Pt"] = "GenElectron1Pt";
  plots["hGenNMuons"]      = "GenNMuons";
  plots["hGenMuon1Pt"]     = "GenMuon1Pt";
  plots["hGenHT"]          = "GenHT";
  plots["hGenNJets"]       = "GenNJets";
  plots["hGenJet1Pt"]      = "GenJet1Pt";
  plots["hPFMET"]          = "PFMET";
  plots["hNElectrons"]     = "NElectrons";
  plots["hElectron1Pt"]    = "Electron1Pt";
  plots["hNMuons"]         = "NMuons";
  plots["hMuon1Pt"]        = "Muon1Pt";
  plots["hHT"]             = "HT";
  plots["hNJets"]          = "NJets";
  plots["hJet1Pt"]         = "Jet1Pt";
  plots["hNTaus"]          = "NTaus";
  plots["hTau1Pt"]         = "Tau1Pt";
  plots["hNElectronsCut"]  = "NElectrons_Cut";
  plots["hElectron1PtCut"] = "Electron1Pt_Cut";
  plots["hNMuonsCut"]      = "NMuons_Cut";
  plots["hMuon1PtCut"]     = "Muon1Pt_Cut";
  plots["hHTCut"]          = "HT_Cut";
  plots["hNJetsCut"]       = "NJets_Cut";
  plots["hJet1PtCut"]      = "Jet1Pt_Cut";
  plots["hNTausCut"]       = "NTaus_Cut";
  plots["hTau1PtCut"]      = "Tau1Pt_Cut";
  plots["stauM"]           = "StauM";
  plots["neutralinoM"]     = "NeutralinoM";
  plots["InvMass"]         = "InvMas";
  plots["InvMassCut"]      = "InvMass_Cut";

  TCanvas c1("c1", "c1", 800, 600);

  for(std::map<std::string, std::string>::iterator i = plots.begin(); i != plots.end(); ++i)
  {
//    std::cout << i->first << ": " << i->second << std::endl;
    TH1F* hist = (TH1F*) plotDir->Get((i->first).c_str());
    hist->Draw();

    c1.SaveAs((outdir+"/"+basename+"_"+i->second+".png").c_str());
    c1.SaveAs((outdir+"/"+basename+"_"+i->second+".pdf").c_str());
    c1.SaveAs((outdir+"/"+basename+"_"+i->second+".root").c_str());
  }

  return 0;
}
