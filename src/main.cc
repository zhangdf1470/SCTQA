#include "main.h"
#include "NoiseScan.h"
#include "ThrScan.h"
#include "ThreePointGain.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <stdio.h>
#include "TChain.h"
#include "TROOT.h"
#include "TFile.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TKey.h"
#include "TMath.h"
#include "Riostream.h"
#include "TEventList.h"
#include "TDirectory.h"
#include "TObject.h"

//#include "AtlasLabels.C"
//#include "AtlasStyle.C"
//#include "AtlasUtils.C"
#include "AtlasLabels.h"
#include "AtlasStyle.h"
#include "AtlasUtils.h"

int main(int argc, char *argv[]){

  SetAtlasStyle() ;

  bool fthreshold = false;
  std::string srunid[nrun_all];
  int runid[nrun_all];
  if(argc == 2){
    srunid[0] = argv[1];
    runid[0] = atoi(srunid[0].c_str());
  }else if(argc == 3){
    srunid[0] = argv[1];
    runid[0] = atoi(srunid[0].c_str());
    std::string soption = argv[2];
    if(soption.find("t") != -1){
      fthreshold = true;
    }else{
      srunid[1] = argv[2];
      runid[1] = atoi(srunid[1].c_str());
    }
  }else if(argc == 4){
    srunid[0] = argv[1];
    srunid[1] = argv[2];
    srunid[2] = argv[3];
    runid[0] = atoi(srunid[0].c_str());
    runid[1] = atoi(srunid[1].c_str());
    runid[2] = atoi(srunid[2].c_str());
  }else if(argc == 5){
    srunid[0] = argv[1];
    srunid[1] = argv[2];
    srunid[2] = argv[3];
    srunid[3] = argv[4];
    runid[0] = atoi(srunid[0].c_str());
    runid[1] = atoi(srunid[1].c_str());
    runid[2] = atoi(srunid[2].c_str());
    runid[3] = atoi(srunid[3].c_str());
  }else{
    std::cout << "Usage:" << std::endl;
    std::cout << "Noise scan: $ana [run#]" << std::endl;
    std::cout << "Threshold scan: $ana [run#] t" << std::endl;
    std::cout << "3-point gain: $ana [run0#(1.5fC)] [run1#(2.0fC)] [run2#(2.5fC)]" << std::endl;
  }

  if(argc == 2){
    NoiseScan* m_NoiseScan = new NoiseScan();
    m_NoiseScan->MakePlots(runid[0]);
    delete m_NoiseScan;
  }else if(argc == 3){
    if(fthreshold){
      ThrScan* m_ThrScan = new ThrScan();
      m_ThrScan->MakePlots(runid[0]);
      delete m_ThrScan;
    }else{ //noise @ 1fC
      ThrScan* m_ThrScan = new ThrScan();
      m_ThrScan->MakePlots(runid[0]);
      m_ThrScan->CheckNoisyStrips(runid[1]);
      delete m_ThrScan;
    }
  }else if(argc == 4){
    ThreePointGain* m_ThreePointGain = new ThreePointGain();
    int runid_gain[nrun_gain];
    for(int i=0; i<nrun_gain; i++) runid_gain[i] = runid[i];
    m_ThreePointGain->MakePlots(runid_gain);
    delete m_ThreePointGain;
  }else if(argc == 5){
    ThreePointGain* m_ThreePointGain = new ThreePointGain();
    int runid_gain[nrun_gain];
    for(int i=0; i<nrun_gain; i++) runid_gain[i] = runid[i];
    m_ThreePointGain->MakePlots(runid_gain);
    m_ThreePointGain->CheckNoisyStrips(runid[nrun_all - 1]);
    delete m_ThreePointGain;
  }
  
  return 0;
}
