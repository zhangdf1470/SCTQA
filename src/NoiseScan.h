#ifndef _NoiseScan
#define _NoiseScan

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

class NoiseScan{

 private:
  TChain* chain;
  TH1F* hhit[nside];
  TH1F* hthr[nside][nstrip];
  TH1F* hnoise;
  TH1F* hvt50;
  TF1 *func[nside][nstrip];

  bool finitialize; 

 public:
  NoiseScan();
  ~NoiseScan();
  void ReadTree(int runid, std::string &srun);
  void Initialize();
  void FillEvents();
  void PlotSummary(std::string srun);
  void PlotStripThrshold(std::string srun, Double_t thr_dac[nside][nstrip]);
  void Analyze(int runid);
  void CheckNoisyStrips(int runid);
  void MakePlots(int runid);
  void CheckNoisyStrips(int runid, Double_t thr_dac[nside][nstrip]);
};

#endif

