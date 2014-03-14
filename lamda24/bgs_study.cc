#include "TROOT.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TMath.h"
#include "TCut.h"

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooCategory.h"
#include "RooBernstein.h"
#include "RooExponential.h"
#include "RooProdPdf.h"
#include "RooPlot.h"

#include "RooStats/BernsteinCorrection.h"

#include <algorithm>
#include <vector>
#include <iostream>

//RooDataSet* getDS(TFile* _file0, RooArgSet var);
//RooAbsPdf* getBKG(RooDataSet* ds, RooRealVar& higgs_mass);

//TTree* getTree(TString var);
//TH1F* FillHist(TH1F* hist, TTree* tree, TString var, TString cuts);
//TH1F* getHist(TString var, Double_t min, Double_t max, Int_t divs = 0, TString cuts = "");
//void genVar(TString varName, TString var, Double_t min, Double_t max, Int_t div, Double_t weight);

class Bg_Study
{
public:
  Bg_Study();
  Bg_Study(bool do_signal, bool do_jetveto);

  ~Bg_Study();

  inline void setPlotDir(std::string dir) {_plotDir = dir;}
  inline std::string getPlotDir()         {return _plotDir;}

  inline void setSignalStatus(bool do_signal) {_do_signal = do_signal;}
  inline bool getSignalStatus()               {return _do_signal;}

  inline void setJetVetoStatus(bool do_jetveto) {_do_jetveto = do_jetveto;}
  inline bool getJetVetoStatus()                {return _do_jetveto;}
  inline void setJetVeto(std::string jetveto)   {_JetVeto = jetveto.c_str();}
  inline TCut getJetVeto()                      {return _JetVeto;}

  bool setFile(std::string filename);
  bool checkFile();

  void addXVar(std::string name, double xMin, double xMax, std::string unit = "");
  void setWeight(std::string name, double xMin, double xMax, std::string unit = "");

  void addRegion(std::string type, std::string name, std::string xVar, double xMin, double xMax);
  void addRegion(std::vector<std::string> type, std::string name, std::string xVar, double xMin, double xMax);
  void resetRegions();

  void addChannel(std::string name);

  void buildDataset();

  inline void setVerboseLevel(int level) {verbose = level;}

private:
  std::string _plotDir;
  bool _do_signal, _do_jetveto;

  TCut _JetVeto;

  TFile* _file;

  std::map<std::string, RooRealVar*> _xVar;
  RooRealVar* _weight;

  std::map<std::string, std::vector<std::string> > _Region;
  TCategory _Channels;

  int verbose;
  bool _datasetReady;

protected:

};

int bgs_study(bool do_signal = true, bool do_jetveto = false)
{
  int verbose = 0;
  Bg_Study myBGStudy(do_signal, do_jetveto);

  myBGStudy.setVerboseLevel(verbose);

  if(verbose) std::cout << "Setting input file" << std::endl;
  myBGStudy.setFile("histograms_CMS-HGG.root");
  if(verbose) std::cout << "Setting the xVar (higgs_mass)" << std::endl;
  myBGStudy.addXVar("higgs_mass",100.,180.);
  if(verbose) std::cout << "Setting the event weights (evweight)" << std::endl;
  myBGStudy.setWeight("evweight",0.,20.);

  if(verbose) std::cout << "Adding the signal and sideband regions" << std::endl;
  myBGStudy.addRegion(   "Signal",    "Signal", "higgs_mass", 120, 130);
  myBGStudy.addRegion("Sidebands", "Sideband1", "higgs_mass", 100, 120);
  myBGStudy.addRegion("Sidebands", "Sideband2", "higgs_mass", 130, 180);

  if(verbose) std::cout << "Done" << std::endl;
  return 0;
}

Bg_Study::Bg_Study()
:_plotDir("Plots/"),
_do_signal(true),
_do_jetveto(false),
_JetVeto("Higgs_pT<100")
{
  _file = NULL;
  _weight = NULL;
  verbose = 0;
}

Bg_Study::Bg_Study(bool do_signal, bool do_jetveto)
:_plotDir("Plots/"),
_do_signal(do_signal),
_do_jetveto(do_jetveto),
_JetVeto("Higgs_pT<100")
{
  _file = NULL;
  _weight = NULL;
  verbose = 0;
}

Bg_Study::~Bg_Study()
{
  if(verbose) std::cout << "Bg_Study::~Bg_Study - Trying to close the opened file, if any" << std::endl;
  if(_file != NULL)
  {
    _file->Close();
    delete _file;
  }

  if(verbose) std::cout << "Bg_Study::~Bg_Study - Deleting xVar, if they exists" << std::endl;
  for(auto i = _xVar.begin(); i != _xVar.end(); ++i)
  {
    delete i->second;
  }

  if(verbose) std::cout << "Bg_Study::~Bg_Study - Deleting weight, if it exists" << std::endl;
  if(_weight != NULL)
  {
    delete _weight;
  }

  if(verbose) std::cout << "Bg_Study::~Bg_Study - Done removing Bg_Study from memory" << std::endl;
}

bool Bg_Study::setFile(std::string filename)
{
  _datasetReady = false;
  if(_file != NULL)
    delete _file;

  _file = new TFile(filename.c_str(), "READ");

  if(_file)
    return true;
  else
    return false;
}

bool Bg_Study::checkFile()
{
  if(_file)
    return true;
  else
    return false;
}

void Bg_Study::addXVar(std::string name, double xMin, double xMax, std::string unit)
{
  _datasetReady = false;
  if(verbose) std::cout << "Bg_Study::addXVar - Removing old xVar (" << name << "), if it exists" << std::endl;
  if(_xVar.find(name) != _xVar.end())
    delete _xVar[name];
  if(verbose) std::cout << "Bg_Study::addXVar - Allocating new xVar (" << name << ")" << std::endl;
  _xVar[name] = new RooRealVar(name.c_str(), name.c_str(), xMin, xMax, unit.c_str());
  _xVar[name]->setBins(10000, "cache");
}

void Bg_Study::setWeight(std::string name, double xMin, double xMax, std::string unit)
{
  _datasetReady = false;
  if(verbose) std::cout << "Bg_Study::setWeight - Removing old weight, if it exists" << std::endl;
  if(_weight != NULL)
    delete _weight;
  if(verbose) std::cout << "Bg_Study::setWeight - Allocating new weight" << std::endl;
  _weight = new RooRealVar(name.c_str(), name.c_str(), xMin, xMax, unit.c_str());
}

void Bg_Study::addRegion(std::string type, std::string name, std::string xVar, double xMin, double xMax)
{
  _datasetReady = false;
  if(_xVar.find(xVar) != _xVar.end())
  {
    if(!_xVar[xVar]->hasRange(name.c_str()))
      _xVar[xVar]->setRange(name.c_str(),xMin,xMax);

    if(_Region.find(type) != _Region.end())
    {
      if(std::find(_Region[type].begin(), _Region[type].end(), name) == _Region[type].end())
        _Region[type].push_back(name);
    }
    else
    {
      std::vector<std::string> temp;
      temp.push_back(name);
      _Region[type] = temp;
    }
  }
}

void Bg_Study::addRegion(std::vector<std::string> type, std::string name, std::string xVar, double xMin, double xMax)
{
  _datasetReady = false;
  if(_xVar.find(xVar) != _xVar.end())
  {
    if(!_xVar[xVar]->hasRange(name.c_str()))
      _xVar[xVar]->setRange(name.c_str(),xMin,xMax);

    for(auto i = type.begin(); i != type.end(); ++i)
    {
      if(_Region.find(*i) != _Region.end())
      {
        if(std::find(_Region[*i].begin(), _Region[*i].end(), name) == _Region[*i].end())
          _Region[*i].push_back(name);
      }
      else
      {
        std::vector<std::string> temp;
        temp.push_back(name);
        _Region[*i] = temp;
      }
    }
  }
}

void Bg_Study::resetRegions()
{
  _datasetReady = false;
  for(auto i = _xVar.begin(); i != _xVar.end(); ++i)
  {
    for(auto j = _Region.begin(); j != _Region.end(); ++j)
    {
      for(auto k = (j->second).begin(); k != (j->second).end(); ++k)
      {
        if(i->second->hasRange((*k).c_str()))
          i->second->removeRange((*k).c_str());
      }
    }
  }

  _Region.clear();
}

void Bg_Study::buildDataset()
{
  if(_datasetReady)
    return;

  RooArgSet tempSet("DatasetVars");
  for(auto i = _xVars.begin(); i != _xVars.end(); ++i)
    tempSet.add(*(i->second));
  if(_weight != NULL)
    tempSet.add(*_weight);

  _datasetReady = true;
}
