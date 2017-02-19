#include <vector>
#include <utility>
#include <iostream>

#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TCut.h"
#include "TColor.h"
#include "TString.h"

TFile* file1;
TFile* file2;
TTree* tree1;
TTree* tree2;
TString name1;
TString name2;

TCut base = "diphotonMVA>-0.05 && category>=4 && (sampleType<0 || (CMS_hgg_mass>130)) && sampleType>-400 && leadJPt>30 && subleadJPt>20 && MJJ>250";
TCut tight = "leadPtOverM>0.5 && subleadPtOverM>0.3 && leadJPt>30 && subleadJPt>30 && TMath::Abs(deltaEtaJJ)>3 && TMath::Abs(Zep)<2.5 && MJJ>500 && TMath::Abs(deltaPhiJJGamGam)>2.6";
TCut loose = "leadPtOverM>0.5 && subleadPtOverM>0.3 && leadJPt>30 && subleadJPt>20 && TMath::Abs(deltaEtaJJ)>3 && TMath::Abs(Zep)<2.5 && MJJ>250 && TMath::Abs(deltaPhiJJGamGam)>2.6";
TCut cat4 = "diphotonMVA>-0.05 && category==4";
TCut cat5 = "diphotonMVA>-0.05 && category==5";

void CreateHisto(std::vector<std::pair<TTree*, TString> > trees, TString name, TCut cut, Int_t bins, Double_t min, Double_t max, TString directory = "~/www/Properties/output/", Bool_t left = false)
{
  Int_t colors[] = {kRed, kBlue, kGreen};

  if(trees.size() > 4)
  {
    throw "CreateHisto:: Error - Too many trees\n";
  }

  TCanvas* canvas = new TCanvas(name, name, 800, 600);
  TH1F** histos = new TH1F*[trees.size()];
  Double_t maxVal = 0;

  for(Int_t i = 0; i < trees.size(); i++)
  {
    histos[i] = new TH1F(trees[i].second, trees[i].second, bins, min, max);
    Long64_t temp = trees[i].first->Draw(name+">>"+trees[i].second, cut, "norm hist");
    if(temp < 0)
    {
      std::cout << "Branch: " << name << "; not found in one of the trees, skipping this plot." << std::endl;
      return;
    }

    if(histos[i]->GetMaximum() > maxVal)
      maxVal = histos[i]->GetMaximum();
    if(name != "MJJ" && i < 2)
    {
      delete histos[i];
    }
  }

  if(name == "MJJ")
  {
    histos[0]->Divide(histos[1]);
    histos[0]->Draw();

    canvas->SaveAs(directory+name+"_div.png");
    canvas->SaveAs(directory+name+"_div.pdf");
    canvas->SaveAs(directory+name+"_div.root");
    canvas->SaveAs(directory+name+"_div.C");
    delete histos[0];
    delete histos[1];
  }

  for(Int_t i = 0; i < trees.size(); i++)
  {
    histos[i] = new TH1F(trees[i].second, trees[i].second, bins, min, max);

    histos[i]->SetStats(false);
    histos[i]->SetBit(TH1::kNoTitle);
    if(i > 0)
      histos[i]->SetLineColor(colors[i-1]);
    histos[i]->Sumw2();

    if(i == 0)
    {
      trees[i].first->Draw(name+">>"+trees[i].second, cut, "norm hist");
      histos[i]->GetYaxis()->SetRangeUser(0, maxVal*1.1);
      histos[i]->GetXaxis()->SetTitle(name);
    }
    else
    {
      trees[i].first->Draw(name+">>"+trees[i].second, cut, "norm same hist");
    }
  }

  if(left)
    //canvas->BuildLegend(0.12, 0.12, 0.4, 0.25);
    canvas->BuildLegend(0.12, 0.75, 0.4, 0.88);
  else
    canvas->BuildLegend(0.6, 0.75, 0.88, 0.88);
  canvas->Write();
  name.ReplaceAll("TMath::", "");
  name.ReplaceAll("(", "_");
  name.ReplaceAll(")", "_");
  name.ReplaceAll("{", "_");
  name.ReplaceAll("}", "_");
  canvas->SaveAs(directory+name+".png");
  canvas->SaveAs(directory+name+".pdf");
  canvas->SaveAs(directory+name+".root");
  canvas->SaveAs(directory+name+".C");
}

void CreateHistos(std::vector<std::pair<TTree*, TString> > trees, TCut cut, TString directory = "~/www/Properties/output")
{
  if(trees.size() > 4)
  {
    throw "CreateHistos:: Error - Too many trees\n";
  }

  TFile* outFile = new TFile(directory+".root", "RECREATE");

  directory += "/";

  CreateHisto(trees, "abs(deltaPhiJJ)", cut, 30, 0, TMath::Pi(), directory);
  CreateHisto(trees, "abs(deltaPhiJJL)", cut, 50, 0, TMath::Pi(), directory);
  CreateHisto(trees, "abs(deltaPhiJJS)", cut, 30, 0, TMath::Pi(), directory);
  CreateHisto(trees, "abs(deltaPhiGamGam)", cut, 30, -0, TMath::Pi(), directory, true);
  CreateHisto(trees, "etaJJ", cut, 40, -4., 4., directory);

  CreateHisto(trees, "TMath::Cos(thetaJ1)", cut, 23, -1., 1., directory);
  CreateHisto(trees, "cosThetaJ1", cut, 23, -1., 1., directory);
  CreateHisto(trees, "TMath::Cos(thetaJ2)", cut, 21, -1., 1., directory);
  CreateHisto(trees, "cosThetaJ2", cut, 23, -1., 1., directory);
  CreateHisto(trees, "abs(TMath::Cos(thetaJ1))", cut, 23, 0., 1., directory);
  CreateHisto(trees, "abs(cosThetaJ1)", cut, 23, 0., 1., directory);
  CreateHisto(trees, "abs(TMath::Cos(thetaJ2))", cut, 21, 0., 1., directory);
  CreateHisto(trees, "abs(cosThetaJ2)", cut, 23, 0., 1., directory);

  CreateHisto(trees, "absThetaL", cut, 23, 0., 1., directory);
  CreateHisto(trees, "cosThetaL", cut, 23, 0., 1., directory);
  CreateHisto(trees, "abs(cosThetaL)", cut, 23, 0., 1., directory);
  CreateHisto(trees, "absThetaS", cut, 21, 0., 1., directory);
  CreateHisto(trees, "cosThetaS", cut, 23, 0., 1., directory);
  CreateHisto(trees, "abs(cosThetaS)", cut, 23, 0., 1., directory);

  CreateHisto(trees, "diphoPtOverM", cut, 60, 0, 2.9, directory);
  CreateHisto(trees, "leadPtOverM", cut, 60, 0.2, 2.9, directory);
  CreateHisto(trees, "subleadPtOverM", cut, 50, 0.1, 1.3, directory);
  CreateHisto(trees, "leadJPt", cut, 40, 0, 230, directory);
  CreateHisto(trees, "subleadJPt", cut, 50, 0, 170, directory);
  CreateHisto(trees, "abs(deltaEtaJJ)", cut, 40, 0, 9, directory);
  CreateHisto(trees, "abs(Zep)", cut, 40, 0, 5, directory);
  CreateHisto(trees, "MJJ", cut, 40, 0, 1500, directory);
  CreateHisto(trees, "abs(deltaPhiJJGamGam)", cut, 42, 0, TMath::Pi()+0.2, directory, true);

  outFile->Close();
}

void Compare(std::vector<std::pair<TTree*, TString> > trees, TString outdir = /*"test/"*/"~/www/Properties/")
{
  if(trees.size() > 4)
  {
    throw "Compare:: Error - Too many trees\n";
  }

  TString Directory = outdir;
  for(Int_t i = 0; i < trees.size(); i++)
  {
    if(i != 0)
      Directory += "vs";
    Directory += trees[i].second;
  }
  Directory.ReplaceAll("{", "_");
  Directory.ReplaceAll("}", "_");
  Directory.ReplaceAll(" ", "_");

  std::string temp = ("rm -R "+Directory+"/").Data();
  system(temp.c_str());
  temp = "mkdir -p "+Directory;
  system(temp.c_str());
  temp = ("ln -s ~/www/indexer/index.php "+Directory+"/").Data();
  system(temp.c_str());

  std::pair<TCut, TString> Preselection (base,                    "preselection");
  std::pair<TCut, TString> Tight        (cat4 /*base && tight*/,           "tight");
  std::pair<TCut, TString> Loose        (cat5 /*base && loose && !tight*/, "loose");

  std::pair<TCut, TString>* cuts[] = { &Preselection,
                                       &Tight,
                                       &Loose
                                     };

  for(Int_t i = 0; i < sizeof(cuts)/sizeof(std::pair<TCut, TString>*); i++)
  {
    temp = "rm -R "+Directory+"/"+cuts[i]->second;
    system(temp.c_str());
    temp = "mkdir "+Directory+"/"+cuts[i]->second;
    system(temp.c_str());
    temp = ("ln -s ~/www/indexer/index.php "+Directory+"/"+cuts[i]->second+"/").Data();
    system(temp.c_str());
    CreateHistos(trees, cuts[i]->first, Directory+"/"+cuts[i]->second);
  }
}

void Compare()
{
  TFile* File    = new TFile("./Trees3.root", "READ");

  TTree* POWHEG_t       = (TTree*)File->Get("vbf_m125_8TeV"          );
  TTree* VBFNLOs0_t     = (TTree*)File->Get("vbfnlo_m125_8TeV"       );
  TTree* VBFNLOs2_t     = (TTree*)File->Get("vbfnlo_spin2_m125_8TeV" );
  TTree* Back_t1        = (TTree*)File->Get("diphojet_8TeV"          );
  TTree* Back_t2        = (TTree*)File->Get("dipho_Box_25_8TeV"      );
  TTree* Back_t3        = (TTree*)File->Get("dipho_Box_250_8TeV"     );
  TTree* ggH_t        = (TTree*)File->Get("ggh_m125_8TeV"     );

  std::pair<TTree*, TString> POWHEG   (POWHEG_t,   "POWHEG_{RECO}"     );
  std::pair<TTree*, TString> VBFNLOs0 (VBFNLOs0_t, "VBFNLO_{s0_{RECO}}");
  std::pair<TTree*, TString> VBFNLOs2 (VBFNLOs2_t, "VBFNLO_{s2_{RECO}}");
  std::pair<TTree*, TString> back1    (Back_t1,    "diphojet 8TeV"     );
  std::pair<TTree*, TString> back2    (Back_t2,    "dipho box 25 8TeV" );
  std::pair<TTree*, TString> back3    (Back_t3,    "dipho box 250 8TeV");
  std::pair<TTree*, TString> ggH    (ggH_t,    "ggH");

  std::vector<std::pair<TTree*, TString> > comp;
  std::vector<std::pair<TTree*, TString> > toRun;
  std::vector<std::pair<TTree*, TString> > backs;

  comp.push_back(POWHEG  );
  comp.push_back(VBFNLOs0);

  toRun.push_back(VBFNLOs0);
  toRun.push_back(VBFNLOs2);
  Compare(toRun);
  toRun.push_back(ggH);

  backs.push_back(back1);
  backs.push_back(back2);
  backs.push_back(back3);

  Compare(comp );
  Compare(backs);
  Compare(toRun);
}
