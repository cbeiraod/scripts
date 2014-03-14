#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TMath.h"

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooCategory.h"
#include "RooBernstein.h"
#include "RooExponential.h"
#include "RooProdPdf.h"
#include "RooPlot.h"

#include "RooStats/BernsteinCorrection.h"

//#define DO_SIG false
//#define APPEND "_onlyBG"
#define DO_SIG true
#define APPEND ""

//TH1F getHist(TTree *tree, TString *var, TString cuts = "");

RooDataSet* getDS(TFile* _file0, RooArgSet var);
RooAbsPdf* getBKG(RooDataSet* ds, RooRealVar& higgs_mass);

TTree* getTree(TString var);
TH1F* FillHist(TH1F* hist, TTree* tree, TString var, TString cuts);
TH1F* getHist(TString var, Double_t min, Double_t max, Int_t divs = 0, TString cuts = "");
void genVar(TString varName, TString var, Double_t min, Double_t max, Int_t div, Double_t weight);

TFile *_file0 = NULL;

int bgs_study_old()
{
  _file0 = TFile::Open("histograms_CMS-HGG.root");
  TFile *temp = new TFile("todelete.root", "RECREATE");
  //spin_trees->cd();

  RooRealVar higgs_mass("higgs_mass","higgs_mass",100,180);
  RooRealVar evweight("evweight","evweight",0,20);

  higgs_mass.setRange("Sideband1",100,120);
  higgs_mass.setRange("Signal",120,130);
  higgs_mass.setRange("Sideband2",130,180);
  higgs_mass.setBins(10000,"cache");

  RooDataSet* ds = getDS(_file0, RooArgSet(higgs_mass,evweight));
  RooAbsPdf* bkg = getBKG(ds, higgs_mass);

  RooPlot* xframe = higgs_mass.frame(RooFit::Title("Mass"));
  ds->plotOn(xframe, RooFit::DataError(RooAbsData::SumW2));
  bkg->plotOn(xframe);

  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
  xframe->Draw();
  TString name = "massFit";
  c1->SaveAs(name+APPEND+".png");
  delete c1;

  RooAbsReal* igx_sig = bkg->createIntegral(higgs_mass,RooFit::NormSet(higgs_mass),RooFit::Range("Signal"));
  //cout << "signal events = " << igx_sig->getVal() << endl;
  RooAbsReal* igx_side = bkg->createIntegral(higgs_mass,RooFit::NormSet(higgs_mass),RooFit::Range("Sideband1,Sideband2"));
  //cout << "sideband events = " << igx_side->getVal() << endl;
  Double_t weight = -igx_sig->getVal()/igx_side->getVal();

  genVar("mgg_sub", "higgs_mass", 100, 180, 10, weight);
  genVar("pT_sub", "sqrt(higgs_px*higgs_px+higgs_py+higgs_py)", 20, 140, 10, weight);
  genVar("y_sub", "atanh(higgs_pz/higgs_E)", -2, 2, 10, weight);
  genVar("costh_sub", "abs(costheta_cs)", 0, 1, 10, weight);

  return 0;
}

RooDataSet* getDS(TFile* _file0, RooArgSet var)
{
  TTree *tree;

  RooCategory c("c","c");
  c.defineType("qcd_30_ff");
  c.defineType("qcd_40_ff");
  c.defineType("qcd_30_pf");
  c.defineType("qcd_40_pf");
  c.defineType("gjet_20_pf");
  c.defineType("gjet_40_pf");
  c.defineType("diphojet");
  c.defineType("dipho_Box_25");
  c.defineType("dipho_Box_250");
  c.defineType("gjet_20_pp");
  c.defineType("gjet_40_pp");
  c.defineType("gg_grav");
  c.defineType("ggh");
  c.defineType("vbf");
  c.defineType("wzh");
  c.defineType("tth");

  tree = (TTree*)_file0->Get("spin_trees/qcd_30_8TeV_ff");
  RooDataSet ds_qcd_30_ff("ds_qcd_30_ff","ds_qcd_30_ff",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/qcd_40_8TeV_ff");
  RooDataSet ds_qcd_40_ff("ds_qcd_40_ff","ds_qcd_40_ff",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/qcd_30_8TeV_pf");
  RooDataSet ds_qcd_30_pf("ds_qcd_30_pf","ds_qcd_30_pf",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/qcd_40_8TeV_pf");
  RooDataSet ds_qcd_40_pf("ds_qcd_40_pf","ds_qcd_40_pf",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));

  tree = (TTree*)_file0->Get("spin_trees/gjet_20_8TeV_pf");
  RooDataSet ds_gjet_20_pf("ds_gjet_20_pf","ds_gjet_20_pf",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/gjet_40_8TeV_pf");
  RooDataSet ds_gjet_40_pf("ds_gjet_40_pf","ds_gjet_40_pf",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/diphojet_8TeV");
  RooDataSet ds_diphojet("ds_diphojet","ds_diphojet",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/dipho_Box_25_8TeV");
  RooDataSet ds_dipho_Box_25("ds_dipho_Box_25","ds_dipho_Box_25",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/dipho_Box_250_8TeV");
  RooDataSet ds_dipho_Box_250("ds_dipho_Box_250","ds_dipho_Box_250",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/gjet_20_8TeV_pp");
  RooDataSet ds_gjet_20_pp("ds_gjet_20_pp","ds_gjet_20_pp",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/gjet_40_8TeV_pp");
  RooDataSet ds_gjet_40_pp("ds_gjet_40_pp","ds_gjet_40_pp",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));

  tree = (TTree*)_file0->Get("spin_trees/gg_grav_m125_8TeV");
  RooDataSet ds_gg_grav("ds_gg_grav","ds_gg_grav",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/ggh_m125_8TeV");
  RooDataSet ds_ggh("ds_ggh","ds_ggh",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/vbf_m125_8TeV");
  RooDataSet ds_vbf("ds_vbf","ds_vbf",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/wzh_m125_8TeV");
  RooDataSet ds_wzh("ds_wzh","ds_wzh",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));
  tree = (TTree*)_file0->Get("spin_trees/tth_m125_8TeV");
  RooDataSet ds_tth("ds_tth","ds_tth",var,RooFit::Import(*tree), RooFit::WeightVar("evweight"));


  RooDataSet* good = new RooDataSet("BG_good_ds", "BG_good_ds", var, RooFit::Index(c), RooFit::WeightVar("evweight")
          , RooFit::Import("gjet_20_pf", ds_gjet_20_pf)
          , RooFit::Import("gjet_40_pf", ds_gjet_40_pf)
          , RooFit::Import("diphojet", ds_diphojet)
          , RooFit::Import("dipho_Box_25", ds_dipho_Box_25)
          //, RooFit::Import("dipho_Box_250", ds_dipho_Box_250)
          , RooFit::Import("gjet_20_pp", ds_gjet_20_pp)
          , RooFit::Import("gjet_40_pp", ds_gjet_40_pp)
          );

  RooDataSet* signal = new RooDataSet("sig_ds", "sig_ds", var, RooFit::Index(c), RooFit::WeightVar("evweight")
          , RooFit::Import("ggh", ds_ggh)
          , RooFit::Import("vbf", ds_vbf)
          , RooFit::Import("wzh", ds_wzh)
          , RooFit::Import("tth", ds_tth)
          //, RooFit::Import("gg_grav", ds_gg_grav)
          );

  RooDataSet* bad = new RooDataSet("BG_bad_ds", "BG_bad_ds", var, RooFit::Index(c), RooFit::WeightVar("evweight")
          , RooFit::Import("qcd_30_ff", ds_qcd_30_ff)
          , RooFit::Import("qcd_40_ff", ds_qcd_40_ff)
          , RooFit::Import("qcd_30_ff", ds_qcd_30_pf)
          , RooFit::Import("qcd_40_ff", ds_qcd_40_pf)
          );

#if DO_SIG
  good->append(*signal);
#endif
  RooDataSet* ds = good;

  ds_qcd_30_ff.Print();
  ds_qcd_40_ff.Print();
  ds_qcd_30_pf.Print();
  ds_qcd_40_pf.Print();
  ds_gjet_20_pf.Print();
  ds_gjet_40_pf.Print();
  ds_diphojet.Print();
  ds_dipho_Box_25.Print();
  ds_dipho_Box_250.Print();
  ds_gjet_20_pp.Print();
  ds_gjet_40_pp.Print();
  ds_ggh.Print();
  ds_vbf.Print();
  ds_wzh.Print();
  ds_tth.Print();
  ds_gg_grav.Print();
  ds->Print();

  return ds;
}

RooAbsPdf* getBKG(RooDataSet* ds, RooRealVar& higgs_mass)
{
  RooAbsPdf *bkg = NULL;

  RooRealVar *a1 = new RooRealVar("a1", "bkg parameter a1", 2.2, 0, 20);
  RooRealVar *a2 = new RooRealVar("a2", "bkg parameter a2", 1.6, 0, 20);
  RooRealVar *a3 = new RooRealVar("a3", "bkg parameter a3", 1.2, 0, 10);
  RooRealVar *a4 = new RooRealVar("a4", "bkg parameter a4", 1.0, 0, 10);
  RooRealVar *a5 = new RooRealVar("a5", "bkg parameter a5", 0.8, 0, 10);
  RooAbsPdf *pol = new RooBernstein("bkg_p", "pol background", higgs_mass, RooArgList(*a1, *a2));//, *a3, *a4, *a5));

  RooRealVar *expfac = new RooRealVar("expfac", "bkg exponential factor", -2, -10, 10);
  RooAbsPdf *exp = new RooExponential("bkg_e", "exp background", higgs_mass, *expfac);

  bkg = pol;
  bkg = exp;
  //bkg = new RooProdPdf("bkg","exp x pol background",RooArgList(*exp,*pol)); //Could not get it to work

  bkg->fitTo(*ds,RooFit::Range("Sideband1,Sideband2"));

  return bkg;
}

void genVar(TString varName, TString var, Double_t min, Double_t max, Int_t div, Double_t weight)
{
  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);

  TH1F* hist_sig = getHist(var, min, max, div, "higgs_mass > 120 && higgs_mass < 130");
  TH1F* hist_side = getHist(var, min, max, div, "higgs_mass < 120 || higgs_mass > 130");

  hist_sig->SetNameTitle("120m130", "120<m_{#gamma#gamma}<130");
  hist_side->SetNameTitle("m120130m", "m_{#gamma#gamma}<120 || m_{#gamma#gamma}>130");

  hist_sig->Add(hist_side, weight);
  hist_sig->Scale(1/TMath::Abs(hist_sig->Integral()));

  min = hist_sig->GetMinimum();
  max = hist_sig->GetMaximum();

  if(min < 0)
  {
    Double_t extra = (max-min)*0.1;
    hist_sig->GetYaxis()->SetRangeUser(min-extra, max+extra);
  }
  else
    hist_sig->GetYaxis()->SetRangeUser(0, 1.1*max);

  hist_sig->SetXTitle(varName);

  hist_sig->Draw();

  c1->SaveAs(varName+APPEND+".png");

  delete hist_sig;
  delete hist_side;
  delete c1;
}

TTree* getTree(TString var)
{
  return (TTree*)_file0->Get("spin_trees/"+var);
}

TH1F* getHist(TString var, Double_t min, Double_t max, Int_t divs, TString cuts)
{
  if(divs <= 0)
    divs = 10;

  TH1F* hist = new TH1F("hist", "hist", divs, min, max);
  hist->Sumw2();

  //FillHist(hist, getTree("qcd_30_8TeV_ff"), var, cuts);
  //FillHist(hist, getTree("qcd_40_8TeV_ff"), var, cuts);
  //FillHist(hist, getTree("qcd_30_8TeV_pf"), var, cuts);
  //FillHist(hist, getTree("qcd_40_8TeV_pf"), var, cuts);
  FillHist(hist, getTree("gjet_20_8TeV_pf"), var, cuts);
  FillHist(hist, getTree("gjet_40_8TeV_pf"), var, cuts);
  FillHist(hist, getTree("diphojet_8TeV"), var, cuts);
  FillHist(hist, getTree("dipho_Box_25_8TeV"), var, cuts);
  FillHist(hist, getTree("dipho_Box_250_8TeV"), var, cuts);
  FillHist(hist, getTree("gjet_20_8TeV_pp"), var, cuts);
  FillHist(hist, getTree("gjet_40_8TeV_pp"), var, cuts);
#if DO_SIG
  FillHist(hist, getTree("ggh_m125_8TeV"), var, cuts);
  FillHist(hist, getTree("vbf_m125_8TeV"), var, cuts);
  FillHist(hist, getTree("wzh_m125_8TeV"), var, cuts);
  FillHist(hist, getTree("tth_m125_8TeV"), var, cuts);
  //FillHist(hist, getTree("gg_grav_m125_8TeV"), var, cuts);
#endif

  hist->SetStats(false);
  hist->SetBit(TH1::kNoTitle);
  //hist->Scale(1/hist->Integral());

  return hist;
}

TH1F* FillHist(TH1F* hist, TTree* tree, TString var, TString cuts)
{
  tree->SetEstimate(tree->GetEntries());
  tree->Draw(var, (cuts==""?"evweight":"("+cuts+")*evweight"), "goff");

  if(tree->GetSelectedRows() <= 0)
    return hist;

  for(Int_t i = 0; i < tree->GetSelectedRows(); i++)
  {
    hist->Fill(tree->GetV1()[i], tree->GetW()[i]);
  }

  //tree->Draw(var+">>"+hist->GetName(), (cuts==""?"evweight":"("+cuts+")*evweight"), "goff"); // Only works when we want to fill a single histo from a single tree (not the case)

  return hist;
}
