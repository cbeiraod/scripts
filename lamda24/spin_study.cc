TH1F getHist(TTree *tree, TString *var, TString cuts = "");

int spin_study()
{
  TFile *_file0 = TFile::Open("histograms_CMS-HGG.root");
  spin_trees->cd();

  genVar("mgg", "higgs_mass", 100, 180, 40);
  genVar("pT", "sqrt(higgs_px*higgs_px+higgs_py+higgs_py)", 0, 140, 40);
  genVar("y", "atanh(higgs_pz/higgs_E)", -2.5, 2.5, 40);
  genVar("costh", "costheta_cs", -1, 1, 40);
}

void genVar(TString varName, TString var, Double_t min, Double_t max, Int_t div)
{
  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);

  hist   = getHist(var, min, max, div);
  hist_a = getHist(var, min, max, div, "higgs_mass < 120");
  hist_b = getHist(var, min, max, div, "higgs_mass > 120 && higgs_mass < 130");
  hist_c = getHist(var, min, max, div, "higgs_mass > 130");
  hist_ac = getHist(var, min, max, div, "higgs_mass < 120 || higgs_mass > 130");

  hist->SetNameTitle("Inclusive", "Inclusive");
  hist_a->SetNameTitle("m120", "m_{#gamma#gamma}<120");
  hist_b->SetNameTitle("120m130", "120<m_{#gamma#gamma}<130");
  hist_c->SetNameTitle("130m", "m_{#gamma#gamma}>130");
  hist_ac->SetNameTitle("m120130m", "m_{#gamma#gamma}<120 || m_{#gamma#gamma}>130");

  hist_a->SetLineColor(kBlue);
  hist_b->SetLineColor(kRed);
  hist_c->SetLineColor(kGreen);
  hist_ac->SetLineColor(kOrange);

  min = 0;
  max = hist->GetMaximum();
  if(hist_a->GetMaximum() > max)
    max = hist_a->GetMaximum();
  if(hist_b->GetMaximum() > max)
    max = hist_b->GetMaximum();
  if(hist_c->GetMaximum() > max)
    max = hist_c->GetMaximum();
  if(hist_ac->GetMaximum() > max)
    max = hist_ac->GetMaximum();
  hist->GetYaxis()->SetRangeUser(min, 1.1*max);

  hist->SetXTitle(varName);

  hist->Draw();
  hist_a->Draw("same");
  hist_b->Draw("same");
  hist_c->Draw("same");
  hist_ac->Draw("same");

  c1->BuildLegend(0.65, 0.75, 0.88, 0.88);

  c1->SaveAs(varName+".png");

  delete hist;
  delete hist_a;
  delete hist_b;
  delete hist_c;
  delete hist_ac;
  delete c1;
}

TH1F* getHist(TString var, Double_t min, Double_t max, Int_t divs = 0, TString cuts = "")
{
  if(divs <= 0)
    divs = 10;

  TH1F* hist = new TH1F("hist", "hist", divs, min, max);
  hist->Sumw2();

  //FillHist(hist, qcd_30_8TeV_ff, var, cuts);
  //FillHist(hist, qcd_40_8TeV_ff, var, cuts);
  //FillHist(hist, qcd_30_8TeV_pf, var, cuts);
  //FillHist(hist, qcd_40_8TeV_pf, var, cuts);
  FillHist(hist, gjet_20_8TeV_pf, var, cuts);
  FillHist(hist, gjet_40_8TeV_pf, var, cuts);
  FillHist(hist, diphojet_8TeV, var, cuts);
  FillHist(hist, dipho_Box_25_8TeV, var, cuts);
  FillHist(hist, dipho_Box_250_8TeV, var, cuts);
  FillHist(hist, gjet_20_8TeV_pp, var, cuts);
  FillHist(hist, gjet_40_8TeV_pp, var, cuts);

  hist->SetStats(false);
  hist->SetBit(TH1::kNoTitle);
  hist->Scale(1/hist->Integral());

  return hist;
}

TH1F* FillHist(TH1F* hist, TTree* tree, TString var, TString cuts)
{
  tree->SetEstimate(tree->GetEntries());
  tree->Draw(var, (cuts==""?"evweight":"("+cuts+")*evweight"), "goff");

  if(tree->GetSelectedRows() <= 0)
    return hist;

  double min = 0;
  for(Int_t i = 0; i < tree->GetSelectedRows(); i++)
  {
    if(i == 0 || tree->GetV1()[i] < min)
      min = tree->GetV1()[i];
    hist->Fill(tree->GetV1()[i], tree->GetW()[i]);
  }

  std::cout << "  Min: " << min << std::endl;

  //tree->Draw(var+">>"+hist->GetName(), (cuts==""?"evweight":"("+cuts+")*evweight"), "goff"); // Only works when we want to fill a single histo from a single tree (not the case)

  return hist;
}
