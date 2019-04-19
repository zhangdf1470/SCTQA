#ifndef _ThreePointGain
#define _ThreePointGain

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
#include "TVectorF.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TLatex.h"

class ThreePointGain
{
  private:
    // histograms of gain
    TH1F* hgain;
    TH1F* hgain_top;
    TH1F* hgain_bottom;
    TH1F* hgain_strip;
    TH1F* hgain_strip_top;
    TH1F* hgain_strip_bottom;
    // histograms of noise
    TH1F* hnoise;
    TH1F* hnoise_top;
    TH1F* hnoise_bottom;
    TH1F* hnoise_strip;
    TH1F* hnoise_strip_top;
    TH1F* hnoise_strip_bottom;
    // histograms of offset
    TH1F* hoffset;
    TH1F* hoffset_top;
    TH1F* hoffset_bottom;
    TH1F* hoffset_strip;
    TH1F* hoffset_strip_top;
    TH1F* hoffset_strip_bottom;
    // hitograms of DAC Threshold for 1 fC
    TH1F* hdacthr;
    TH1F* hdacthr_top;
    TH1F* hdacthr_bottom;
    TH1F* hdacthr_strip;
    TH1F* hdacthr_strip_top;
    TH1F* hdacthr_strip_bottom;

    // vector of bad strips ID
    TVectorF* NBadStrips ;
    TVectorF* TopNDeadStrips ;
    TVectorF* TopDeadStripIDs[nrun_gain] ;
    TVectorF* TopNLowEffStrips ;
    TVectorF* TopLowEffStripIDs[nrun_gain] ;
    TVectorF* TopNNoisyStrips ;
    TVectorF* TopNoisyStripIDs[nrun_gain] ;
    TVectorF* TopNXtalkStrips ;
    TVectorF* TopXtalkStripIDs[nrun_gain] ;
    TVectorF* BottomNDeadStrips ;
    TVectorF* BottomDeadStripIDs[nrun_gain] ;
    TVectorF* BottomNLowEffStrips ;
    TVectorF* BottomLowEffStripIDs[nrun_gain] ;
    TVectorF* BottomNNoisyStrips ;
    TVectorF* BottomNoisyStripIDs[nrun_gain] ;
    TVectorF* BottomNXtalkStrips ;
    TVectorF* BottomXtalkStripIDs[nrun_gain] ;

    // Graph of injected charge VS extracted charge
    TGraphErrors* gthree[nside][nstrip];
 
    TF1* func[nside][nstrip];
    double thr_dac[nside][nstrip];

    TFile* outFile ;

    bool finitialize;

  public:
    std::vector<int> vbad_strip[nrun_gain][nbad][nside];

  public:
    ThreePointGain();
    ~ThreePointGain();
    void Initialize();
    void FillEvents(TChain* chain, TH1F* hhit[nside], TH1F* hthr[nside][nstrip]);
    void PlotSummary(std::string srun[nrun_gain]);
    void PlotStripGain(std::string srun[nrun_gain]);
    void MakePlots(int runid[nrun_gain]);
    void CheckNoisyStrips(int runid);


   public:
     TH1F* bookTH1F(std::string name, std::string title,
               std::string xlabel,std::string ylabel,
               int xbins, double xlow, double xhigh,
               bool sumw2 = false, bool overflow=true)
    {
      TH1F* tmp = new TH1F( name.c_str(), title.c_str(), xbins, xlow, xhigh);
      tmp->GetXaxis()->SetTitle(xlabel.c_str()) ;
      tmp->GetYaxis()->SetTitle(ylabel.c_str()) ;
      if(sumw2) tmp->Sumw2() ;
      if(overflow) tmp->StatOverflows() ;
      return tmp ;
    }
};

#endif

