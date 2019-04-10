#ifndef _ThrScan
#define _ThrScan

#include "main.h"

// STL
#include <iostream>
#include <string.h>
#include <map>
#include <stdlib.h>
#include <dirent.h>
#include <iomanip>
#include <vector>
// ROOT
#include "TChain.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TLatex.h"

class ThrScan{

 private:
  TChain* chain;
  TH1F* hhit[nside];
  TH1F* hthr[nside][nstrip];
  TH1F* hxtalk[nside][nstrip];
  TH1F* hbad;
  TH1F* hnoise;
  TH1F* hvt50;
  TF1 *func[nside][nstrip];

  bool finitialize;

 public:
  std::vector<int> vbad_strip[nbad][nside];
  Double_t thr_dac[nside][nstrip];

 public:
  ThrScan();
  ~ThrScan();
  void ReadTree(int runid, std::string &srun);
  void Initialize();
  void FillEvents();
  void FitScurve(Double_t parth[nside][nstrip][4]);
  void PlotSummary(std::string srun);
  void PlotStripThrshold(std::string srun);
  void FindBadStrips();
  void PlotBadStrips(std::string srun);
  void Analyze(int runid);
  void MakePlots(int runid);
  void CheckNoisyStrips(int runid);
};

#endif

