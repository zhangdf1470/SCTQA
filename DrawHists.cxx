#include <iostream>

#include "TSystem.h"

#include "atlasrootstyle/AtlasLabels.C"
#include "atlasrootstyle/AtlasStyle.C"
#include "atlasrootstyle/AtlasUtils.C"

using namespace std;
int DrawHists(string inFileName)
{
  //set ATLAS Style
  SetAtlasStyle() ;

  if(gSystem->AccessPathName(inFileName.c_str()))
  {
    cout<<inFileName<<" does not exist!!!!"<<endl ;
    return 0 ;
  }
  TFile* inFile = new TFile(inFileName.c_str(), "read") ;
  if(!inFile)
  {
    cout<<"Failed to open "<<inFileName<<endl ;
    return 0 ;
  }
  TH1F* hnoise_strip_top = (TH1F*) inFile->Get("hnoise_strip_top") ;
  TH1F* hnoise_strip_bottom = (TH1F*) inFile->Get("hnoise_strip_bottom") ;
  TH1F* hgain_strip_top = (TH1F*) inFile->Get("hgain_strip_top") ;
  TH1F* hgain_strip_bottom = (TH1F*) inFile->Get("hgain_strip_bottom") ;
  TH1F* hoffset_strip_top = (TH1F*) inFile->Get("hoffset_strip_top") ;
  TH1F* hoffset_strip_bottom = (TH1F*) inFile->Get("hoffset_strip_bottom") ;
  TH1F* hdacthr_strip_top = (TH1F*) inFile->Get("hdacthr_strip_top") ;
  TH1F* hdacthr_strip_bottom = (TH1F*) inFile->Get("hdacthr_strip_bottom") ;

  TCanvas* c = new TCanvas("c", "", 1200, 600) ;
  hnoise_strip_top->Draw() ;
  c->Print("Noise_Strip_Top.pdf") ;
  hnoise_strip_bottom->Draw() ;
  c->Print("Noise_Strip_Bottom.pdf") ;
  hgain_strip_top->Draw() ;
  c->Print("Gain_Strip_Top.pdf") ;
  hgain_strip_bottom->Draw() ;
  c->Print("Gain_Strip_Bottom.pdf") ;
  hoffset_strip_top->Draw() ;
  c->Print("Offset_Strip_Top.pdf") ;
  hoffset_strip_bottom->Draw() ;
  c->Print("Offset_Strip_Bottom.pdf") ;
  hdacthr_strip_top->Draw() ;
  c->Print("DACThreshold_Strip_Top.pdf") ;
  hdacthr_strip_bottom->Draw() ;
  c->Print("DACThreshold_Strip_Bottom.pdf") ;
  delete c ;

  c = new TCanvas("c", "", 2400, 1200) ;
  c->Divide(2,2) ;
  c->cd(1) ; 
  hnoise_strip_top->Draw() ;
  c->cd(2) ; 
  hnoise_strip_bottom->Draw() ;
  c->cd(3) ; 
  hgain_strip_top->Draw() ;
  c->cd(4) ; 
  hgain_strip_bottom->Draw() ;
  c->Print("Noise_vs_Gain.pdf") ;
  

  return 0 ;
}
