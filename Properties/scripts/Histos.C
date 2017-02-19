#include "TString.h"
#include "TTree.h"
#include "TH1F.h"
#include "TMath.h"
#include "TCut.h"

TFile* file;
TTree* vbf;
TTree* ggh;
TTree* back;
TTree* data;

TCut base = "diphotonMVA>-0.05 && category>=4 && (sampleType<0 || (mass>130)) && sampleType>-400";
TCut tight = "leadPtOverM>0.5 && subleadPtOverM>0.3 && leadJPt>30 && subleadJPt>30 && TMath::Abs(deltaEtaJJ)>3 && TMath::Abs(Zep)<2.5 && MJJ>500 && TMath::Abs(deltaPhiJJGamGam)>2.6";
TCut loose = "leadPtOverM>0.5 && subleadPtOverM>0.3 && leadJPt>30 && subleadJPt>20 && TMath::Abs(deltaEtaJJ)>3 && TMath::Abs(Zep)<2.5 && MJJ>250 && TMath::Abs(deltaPhiJJGamGam)>2.6";
TCut cat4 = "diphotonMVA>-0.05 && category==4";
TCut cat5 = "diphotonMVA>-0.05 && category==5";

void CreateHisto(TString name, TString cond, Int_t bins, Double_t min, Double_t max)
{
  TCanvas* canvas = new TCanvas(name, name, 800, 600);

  TH1F* vbfH  = new TH1F("VBF",  "VBF",  bins, min, max);
  TH1F* gghH  = new TH1F("ggH",  "ggH",  bins, min, max);
  TH1F* backH = new TH1F("back", "back", bins, min, max);
  TH1F* dataH = new TH1F("data", "data", bins, min, max);

  vbfH->SetStats(false);
  vbfH->SetBit(TH1::kNoTitle);
  vbfH->SetLineColor(kRed);
  gghH->SetLineColor(kBlue);
  backH->SetLineColor(kGreen);
  vbfH->Sumw2();
  gghH->Sumw2();
  backH->Sumw2();
  dataH->Sumw2();

  vbf->Draw(name+">>VBF", cond, "norm hist");
  ggh->Draw(name+">>ggH", cond, "norm same hist");
  back->Draw(name+">>back", cond, "norm same hist");
  data->Draw(name+">>data", cond, "norm same e");
  //2.*(deltaPhiJJ<0)*(TMath::Pi()) +

  vbfH->GetYaxis()->SetRangeUser(0, 0.4);
  canvas->BuildLegend();

  canvas->Write();

  //delete vbfH;
  //delete gghH;
  //delete backH;
  //delete canvas;
}

void Create2DHisto(TString name, TString cond, Int_t binsx, Double_t minx, Double_t maxx, Int_t binsy, Double_t miny, Double_t maxy)
{
  TCanvas* canvas = new TCanvas(name, name, 1050, 350+50);

  TH1F* vbfH  = new TH2F("VBF",  "VBF",  binsx, minx, maxx, binsy, miny, maxy);
  TH1F* gghH  = new TH2F("ggH",  "ggH",  binsx, minx, maxx, binsy, miny, maxy);
  TH1F* backH = new TH2F("back", "back", binsx, minx, maxx, binsy, miny, maxy);

  vbfH->SetStats(false);
  gghH->SetStats(false);
  backH->SetStats(false);
  //vbfH->SetBit(TH1::kNoTitle);
  //vbfH->Sumw2();
  //gghH->Sumw2();
  //backH->Sumw2();

  vbf->Draw(name+">>VBF", cond, "norm colz");
  ggh->Draw(name+">>ggH", cond, "norm colz");
  back->Draw(name+">>back", cond, "norm colz");
  //2.*(deltaPhiJJ<0)*(TMath::Pi()) +

  vbfH->GetYaxis()->SetRangeUser(0, 0.2);
  canvas->BuildLegend();

  canvas->Write();

  //delete vbfH;
  //delete gghH;
  //delete backH;
  //delete canvas;
}

void Histos(TString fileName)
{
  file = new TFile(fileName, "READ");
  TFile* outFile = new TFile("output.root", "RECREATE");

  vbf  = (TTree*)file->Get("vbf_m125_8TeV");
  ggh  = (TTree*)file->Get("ggh_m125_8TeV");
  back = (TTree*)file->Get("diphojet_8TeV");
  data = (TTree*)file->Get("Data");

  // "etaJJ", "deltaPhiJJ", "deltaPhiGamGam"

  TString cond = base && ( tight );

  CreateHisto("TMath::Abs(deltaPhiJJ)", cond, 6, 0, TMath::Pi());
  CreateHisto("TMath::Abs(deltaPhiGamGam)", cond, 6, -0, TMath::Pi());
  CreateHisto("etaJJ", cond, 13, -4., 4.);

  CreateHisto("TMath::Cos(thetaJ1)", cond, 13, -1., 1.);
  CreateHisto("TMath::Cos(thetaJ2)", cond, 13, -1., 1.);

}

void Histos()
{
  Histos("./histograms_CMS-HGG_test.root");
}
