#include<iostream>
#include<string>
#include<vector>

#include<TROOT.h>
#include<TSystem.h>
#include<TFile.h>
#include<TStyle.h>
#include<TCanvas.h>
#include<TH1F.h>

int combinedPlots(std::string outdir, std::string rootDir)
{
  std::vector<TFile*> files;

  std::map<std::string, std::vector<std::string> > deltaMs;
  std::map<std::string, std::string> plots;

  {
    std::vector<std::string> tempVec;

    tempVec.push_back(rootDir+"/TStauStau_50_40_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_290_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_480_5E33_Validation.root");
    deltaMs["5E33_deltaM10"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_50_40_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_290_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_480_8E33_Validation.root");
    deltaMs["8E33_deltaM10"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_50_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_250_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_450_5E33_Validation.root");
    deltaMs["5E33_deltaM50"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_50_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_250_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_450_8E33_Validation.root");
    deltaMs["8E33_deltaM50"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_100_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_200_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_400_5E33_Validation.root");
    deltaMs["5E33_deltaM100"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_100_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_200_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_400_8E33_Validation.root");
    deltaMs["8E33_deltaM100"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_150_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_150_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_350_5E33_Validation.root");
    deltaMs["5E33_deltaM150"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_150_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_150_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_350_8E33_Validation.root");
    deltaMs["8E33_deltaM150"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_300_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_200_5E33_Validation.root");
    deltaMs["5E33_deltaM300"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_300_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_200_8E33_Validation.root");
    deltaMs["8E33_deltaM300"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_300_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_150_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_200_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_250_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_290_5E33_Validation.root");
    deltaMs["5E33_Stau300"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_300_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_150_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_200_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_250_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_290_8E33_Validation.root");
    deltaMs["8E33_Stau300"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_500_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_200_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_350_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_400_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_450_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_480_5E33_Validation.root");
    deltaMs["5E33_Stau500"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_500_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_200_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_350_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_400_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_450_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_480_8E33_Validation.root");
    deltaMs["8E33_Stau500"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_50_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_100_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_150_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_0_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_0_5E33_Validation.root");
    deltaMs["5E33_Neutralino0"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_50_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_100_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_150_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_300_0_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_0_8E33_Validation.root");
    deltaMs["8E33_Neutralino0"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_300_200_5E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_200_5E33_Validation.root");
    deltaMs["5E33_Neutralino200"] = tempVec;

    tempVec.clear();
    tempVec.push_back(rootDir+"/TStauStau_300_200_8E33_Validation.root");
    tempVec.push_back(rootDir+"/TStauStau_500_200_8E33_Validation.root");
    deltaMs["8E33_Neutralino200"] = tempVec;

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
    plots["InvMass"]         = "InvMass";
    plots["InvMassCut"]      = "InvMass_Cut";

    plots["hGenElectron1PtFilt"]  = "GenElectron1PtFilt";
    plots["hElectron1PtFilt"]     = "Electron1PtFilt";
    plots["hGenMuon1PtFilt"]      = "GenMuon1PtFilt";
    plots["hMuon1PtFilt"]         = "Muon1PtFilt";
    plots["hGenElectron1PtFilt2"] = "GenElectron1PtFilt2";
    plots["hElectron1PtFilt2"]    = "Electron1PtFilt2";
    plots["hGenMuon1PtFilt2"]     = "GenMuon1PtFilt2";
    plots["hMuon1PtFilt2"]        = "Muon1PtFilt2";
  }

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TCanvas c1("c1", "c1", 800, 600);

  for(std::map<std::string, std::vector<std::string> >::iterator i = deltaMs.begin(); i != deltaMs.end(); ++i)
  {
    for(std::vector<TFile*>::iterator j = files.begin(); j != files.end(); ++j)
    {
      (*j)->Close();
      delete (*j);
    }
    files.clear();

    for(std::vector<std::string>::iterator j = i->second.begin(); j != i->second.end(); ++j)
    {
      files.push_back(new TFile((*j).c_str(), "READ"));
    }


    for(std::map<std::string, std::string>::iterator j = plots.begin(); j != plots.end(); ++j)
    {
      int count = 0;

      for(std::vector<TFile*>::iterator file = files.begin(); file != files.end(); ++file)
      {
        TH1F* hist = (TH1F*) (*file)->Get(("validator/"+j->first).c_str());
        if(hist == NULL)
          continue;
        hist->Scale(1/(hist->Integral()));

        std::string name = (*file)->GetName();

        std::string::size_type found = name.find("TStauStau_");
        if(found != std::string::npos)
          name.replace(0, found, "");

        found = name.find(".root");
        if(found != std::string::npos)
          name.replace(found, 5, "");

        found = name.find("_Validation");
        if(found != std::string::npos)
          name.replace(found, 11, "");

        found = name.find("E33");
        if(found != std::string::npos)
          name.replace(found-2, 5, "");

        hist->SetNameTitle(name.c_str(), name.c_str());

        switch(count)
        {
        case 0:
          hist->SetLineColor(kBlue);
          break;
        case 1:
          hist->SetLineColor(kRed);
          break;
        case 2:
          hist->SetLineColor(kGreen);
          break;
        case 3:
          hist->SetLineColor(kMagenta);
          break;
        case 4:
          hist->SetLineColor(kYellow);
          break;
        case 5:
          hist->SetLineColor(kCyan);
          break;
        }

        if(count == 0)
          hist->Draw();
        else
          hist->Draw("Same");

        ++count;
      }

      c1.BuildLegend();

      c1.SaveAs((outdir+"/TStauStau_"+i->first+"_"+j->second+".png").c_str());
      c1.SaveAs((outdir+"/TStauStau_"+i->first+"_"+j->second+".pdf").c_str());
      c1.SaveAs((outdir+"/TStauStau_"+i->first+"_"+j->second+".root").c_str());
    }
  }








  //Combine Pt (e, mu, tau, jet1)
    //DeltaM = 10
    //DeltaM = 150
    //DeltaM = 300
  //Combine MET
  return 0;
}
