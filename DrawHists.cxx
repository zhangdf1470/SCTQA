#include <iostream>

#include "TSystem.h"

//#include "atlasrootstyle/AtlasLabels.C"
//#include "atlasrootstyle/AtlasStyle.C"
//#include "atlasrootstyle/AtlasUtils.C"

#include "utils.h"

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

  TVectorF* TopDeadStripIDs[3] ;
  TVectorF* TopLowEffStripIDs[3] ;
  TVectorF* TopNoisyStripIDs[3] ;
  TVectorF* TopXtalkStripIDs[3] ;
  TVectorF* BottomDeadStripIDs[3] ;
  TVectorF* BottomLowEffStripIDs[3] ;
  TVectorF* BottomNoisyStripIDs[3] ;
  TVectorF* BottomXtalkStripIDs[3] ;
  for(int irun=0 ; irun<3 ; irun++)
  {
    TopDeadStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("TopDeadStripIDs%d", irun+1)) ;
    TopLowEffStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("TopLowEffStripIDs%d", irun+1)) ;
    TopNoisyStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("TopNoisyStripIDs%d", irun+1)) ;
    TopXtalkStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("TopXtalkStripIDs%d", irun+1)) ;
    BottomDeadStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("BottomDeadStripIDs%d", irun+1)) ;
    BottomLowEffStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("BottomLowEffStripIDs%d", irun+1)) ;
    BottomNoisyStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("BottomNoisyStripIDs%d", irun+1)) ;
    BottomXtalkStripIDs[irun] = (TVectorF*)inFile->Get(TString::Format("BottomXtalkStripIDs%d", irun+1)) ;
  }
  TH1F* hdeadstripID_top1 = bookTH1F("hdeadstripID_top1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hdeadstripID_top1->SetMarkerColor(1) ;
  TH1F* hdeadstripID_top2 = bookTH1F("hdeadstripID_top2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hdeadstripID_top2->SetMarkerColor(2) ;
  TH1F* hdeadstripID_top3 = bookTH1F("hdeadstripID_top3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hdeadstripID_top3->SetMarkerColor(4) ;
  vector< TH1F* > hdeadstripID_top_vect ;
  hdeadstripID_top_vect.push_back(hdeadstripID_top1) ;
  hdeadstripID_top_vect.push_back(hdeadstripID_top2) ;
  hdeadstripID_top_vect.push_back(hdeadstripID_top3) ;
  TH1F* hnoisystripID_top1 = bookTH1F("hnoisystripID_top1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hnoisystripID_top1->SetMarkerColor(1) ;
  TH1F* hnoisystripID_top2 = bookTH1F("hnoisystripID_top2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hnoisystripID_top2->SetMarkerColor(2) ;
  TH1F* hnoisystripID_top3 = bookTH1F("hnoisystripID_top3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hnoisystripID_top3->SetMarkerColor(4) ;
  vector< TH1F* > hnoisystripID_top_vect ;
  hnoisystripID_top_vect.push_back(hnoisystripID_top1) ;
  hnoisystripID_top_vect.push_back(hnoisystripID_top2) ;
  hnoisystripID_top_vect.push_back(hnoisystripID_top3) ;
  TH1F* hloweffstripID_top1 = bookTH1F("hloweffstripID_top1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hloweffstripID_top1->SetMarkerColor(1) ;
  TH1F* hloweffstripID_top2 = bookTH1F("hloweffstripID_top2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hloweffstripID_top2->SetMarkerColor(2) ;
  TH1F* hloweffstripID_top3 = bookTH1F("hloweffstripID_top3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hloweffstripID_top3->SetMarkerColor(4) ;
  vector< TH1F* > hloweffstripID_top_vect ;
  hloweffstripID_top_vect.push_back(hloweffstripID_top1) ;
  hloweffstripID_top_vect.push_back(hloweffstripID_top2) ;
  hloweffstripID_top_vect.push_back(hloweffstripID_top3) ;
  TH1F* hxtalkstripID_top1 = bookTH1F("hxtalkstripID_top1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hxtalkstripID_top1->SetMarkerColor(1) ;
  TH1F* hxtalkstripID_top2 = bookTH1F("hxtalkstripID_top2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hxtalkstripID_top2->SetMarkerColor(2) ;
  TH1F* hxtalkstripID_top3 = bookTH1F("hxtalkstripID_top3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hxtalkstripID_top3->SetMarkerColor(4) ;
  vector< TH1F* > hxtalkstripID_top_vect ;
  hxtalkstripID_top_vect.push_back(hxtalkstripID_top1) ;
  hxtalkstripID_top_vect.push_back(hxtalkstripID_top2) ;
  hxtalkstripID_top_vect.push_back(hxtalkstripID_top3) ;

  TH1F* hdeadstripID_bottom1 = bookTH1F("hdeadstripID_bottom1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hdeadstripID_bottom1->SetMarkerColor(1) ;
  TH1F* hdeadstripID_bottom2 = bookTH1F("hdeadstripID_bottom2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hdeadstripID_bottom2->SetMarkerColor(2) ;
  TH1F* hdeadstripID_bottom3 = bookTH1F("hdeadstripID_bottom3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hdeadstripID_bottom3->SetMarkerColor(4) ;
  vector< TH1F* > hdeadstripID_bottom_vect ;
  hdeadstripID_bottom_vect.push_back(hdeadstripID_bottom1) ;
  hdeadstripID_bottom_vect.push_back(hdeadstripID_bottom2) ;
  hdeadstripID_bottom_vect.push_back(hdeadstripID_bottom3) ;
  TH1F* hnoisystripID_bottom1 = bookTH1F("hnoisystripID_bottom1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hnoisystripID_bottom1->SetMarkerColor(1) ;
  TH1F* hnoisystripID_bottom2 = bookTH1F("hnoisystripID_bottom2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hnoisystripID_bottom2->SetMarkerColor(2) ;
  TH1F* hnoisystripID_bottom3 = bookTH1F("hnoisystripID_bottom3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hnoisystripID_bottom3->SetMarkerColor(4) ;
  vector< TH1F* > hnoisystripID_bottom_vect ;
  hnoisystripID_bottom_vect.push_back(hnoisystripID_bottom1) ;
  hnoisystripID_bottom_vect.push_back(hnoisystripID_bottom2) ;
  hnoisystripID_bottom_vect.push_back(hnoisystripID_bottom3) ;
  TH1F* hloweffstripID_bottom1 = bookTH1F("hloweffstripID_bottom1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hloweffstripID_bottom1->SetMarkerColor(1) ;
  TH1F* hloweffstripID_bottom2 = bookTH1F("hloweffstripID_bottom2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hloweffstripID_bottom2->SetMarkerColor(2) ;
  TH1F* hloweffstripID_bottom3 = bookTH1F("hloweffstripID_bottom3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hloweffstripID_bottom3->SetMarkerColor(4) ;
  vector< TH1F* > hloweffstripID_bottom_vect ;
  hloweffstripID_bottom_vect.push_back(hloweffstripID_bottom1) ;
  hloweffstripID_bottom_vect.push_back(hloweffstripID_bottom2) ;
  hloweffstripID_bottom_vect.push_back(hloweffstripID_bottom3) ;
  TH1F* hxtalkstripID_bottom1 = bookTH1F("hxtalkstripID_bottom1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hxtalkstripID_bottom1->SetMarkerColor(1) ;
  TH1F* hxtalkstripID_bottom2 = bookTH1F("hxtalkstripID_bottom2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hxtalkstripID_bottom2->SetMarkerColor(2) ;
  TH1F* hxtalkstripID_bottom3 = bookTH1F("hxtalkstripID_bottom3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hxtalkstripID_bottom3->SetMarkerColor(4) ;
  vector< TH1F* > hxtalkstripID_bottom_vect ;
  hxtalkstripID_bottom_vect.push_back(hxtalkstripID_bottom1) ;
  hxtalkstripID_bottom_vect.push_back(hxtalkstripID_bottom2) ;
  hxtalkstripID_bottom_vect.push_back(hxtalkstripID_bottom3) ;

  TH1F* hbadstripID_top1 = bookTH1F("hbadstripID_top1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hbadstripID_top1->SetMarkerColor(1) ;
  TH1F* hbadstripID_top2 = bookTH1F("hbadstripID_top2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hbadstripID_top2->SetMarkerColor(2) ;
  TH1F* hbadstripID_top3 = bookTH1F("hbadstripID_top3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hbadstripID_top3->SetMarkerColor(4) ;
  vector< TH1F* > hbadstripID_top_vect ;
  hbadstripID_top_vect.push_back(hbadstripID_top1) ;
  hbadstripID_top_vect.push_back(hbadstripID_top2) ;
  hbadstripID_top_vect.push_back(hbadstripID_top3) ;
  TH1F* hbadstripID_bottom1 = bookTH1F("hbadstripID_bottom1", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hbadstripID_bottom1->SetMarkerColor(1) ;
  TH1F* hbadstripID_bottom2 = bookTH1F("hbadstripID_bottom2", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hbadstripID_bottom2->SetMarkerColor(2) ;
  TH1F* hbadstripID_bottom3 = bookTH1F("hbadstripID_bottom3", "", "Strip ID", "", 768, 0.5, 768+0.5) ;
  hbadstripID_bottom3->SetMarkerColor(4) ;
  vector< TH1F* > hbadstripID_bottom_vect ;
  hbadstripID_bottom_vect.push_back(hbadstripID_bottom1) ;
  hbadstripID_bottom_vect.push_back(hbadstripID_bottom2) ;
  hbadstripID_bottom_vect.push_back(hbadstripID_bottom3) ;

  for(int irun=0 ; irun<3 ; irun++)
  {
    for(int iElement=0 ; iElement<TopDeadStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hdeadstripID_top_vect.at(irun)->SetBinContent((*TopDeadStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_top_vect.at(irun)->SetBinContent((*TopDeadStripIDs[irun])[iElement], irun+1) ;
    }
    for(int iElement=0 ; iElement<TopLowEffStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hloweffstripID_top_vect.at(irun)->SetBinContent((*TopLowEffStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_top_vect.at(irun)->SetBinContent((*TopLowEffStripIDs[irun])[iElement], irun+1) ;
    }
    for(int iElement=0 ; iElement<TopNoisyStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hnoisystripID_top_vect.at(irun)->SetBinContent((*TopNoisyStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_top_vect.at(irun)->SetBinContent((*TopNoisyStripIDs[irun])[iElement], irun+1) ;
    }
    for(int iElement=0 ; iElement<TopXtalkStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hxtalkstripID_top_vect.at(irun)->SetBinContent((*TopXtalkStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_top_vect.at(irun)->SetBinContent((*TopXtalkStripIDs[irun])[iElement], irun+1) ;
    }
    for(int iElement=0 ; iElement<BottomDeadStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hdeadstripID_bottom_vect.at(irun)->SetBinContent((*BottomDeadStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_bottom_vect.at(irun)->SetBinContent((*BottomDeadStripIDs[irun])[iElement], irun+1) ;
    }
    for(int iElement=0 ; iElement<BottomLowEffStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hloweffstripID_bottom_vect.at(irun)->SetBinContent((*BottomLowEffStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_bottom_vect.at(irun)->SetBinContent((*BottomLowEffStripIDs[irun])[iElement], irun+1) ;
    }
    for(int iElement=0 ; iElement<BottomNoisyStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hnoisystripID_bottom_vect.at(irun)->SetBinContent((*BottomNoisyStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_bottom_vect.at(irun)->SetBinContent((*BottomNoisyStripIDs[irun])[iElement], irun+1) ;
    }
    for(int iElement=0 ; iElement<BottomXtalkStripIDs[irun]->GetNoElements() ; iElement++)
    {
      hxtalkstripID_bottom_vect.at(irun)->SetBinContent((*BottomXtalkStripIDs[irun])[iElement], irun+1) ;
      hbadstripID_bottom_vect.at(irun)->SetBinContent((*BottomXtalkStripIDs[irun])[iElement], irun+1) ;
    }
  }


  // get histograms of gain, noise, offset, dac threashold for 1 fC
  TH1F* hnoise_strip_top = (TH1F*) inFile->Get("hnoise_strip_top") ;
  TH1F* hnoise_strip_bottom = (TH1F*) inFile->Get("hnoise_strip_bottom") ;
  TH1F* hgain_strip_top = (TH1F*) inFile->Get("hgain_strip_top") ;
  TH1F* hgain_strip_bottom = (TH1F*) inFile->Get("hgain_strip_bottom") ;
  TH1F* hoffset_strip_top = (TH1F*) inFile->Get("hoffset_strip_top") ;
  TH1F* hoffset_strip_bottom = (TH1F*) inFile->Get("hoffset_strip_bottom") ;
  TH1F* hdacthr_strip_top = (TH1F*) inFile->Get("hdacthr_strip_top") ;
  TH1F* hdacthr_strip_bottom = (TH1F*) inFile->Get("hdacthr_strip_bottom") ;
  // 2D histogram of noise VS gain
  TH2F* h_noise_vs_gain_top = bookTH2F("h_noise_vs_gain_top", "", "Gain[mV/fC]", "ENC[e]", 40, 30, 70, 30, 1000, 2500) ;
  TH2F* h_noise_vs_gain_bottom = bookTH2F("h_noise_vs_gain_bottom", "", "Gain[mV/fC]", "ENC[e]", 40, 30, 70, 30, 1000, 2500) ;
  for(int i=1 ; i<=hnoise_strip_top->GetNbinsX() ; i++)
  {
    if(hgain_strip_top->GetBinContent(i)==0)
      continue ;
    h_noise_vs_gain_top->Fill(hgain_strip_top->GetBinContent(i), hnoise_strip_top->GetBinContent(i)) ;
    h_noise_vs_gain_bottom->Fill(hgain_strip_bottom->GetBinContent(i), hnoise_strip_bottom->GetBinContent(i)) ;
  }

  // making plots
  makeLongerPlot(hnoise_strip_top, "Noise_Strip_Top", "Hist") ;
  makeLongerPlot(hnoise_strip_bottom, "Noise_Strip_Bottom", "Hist") ;
  makeLongerPlot(hgain_strip_top, "Gain_Strip_Top", "Hist") ;
  makeLongerPlot(hgain_strip_bottom, "Gain_Strip_Bottom", "Hist") ;
  makeLongerPlot(hoffset_strip_top, "Offset_Strip_Top", "Hist") ;
  makeLongerPlot(hoffset_strip_bottom, "Offset_Strip_Bottom", "Hist") ;
  makeLongerPlot(hdacthr_strip_top, "DACThreshold_Strip_Top", "Hist") ;
  makeLongerPlot(hdacthr_strip_bottom, "DACThreshold_Strip_Bottom", "Hist") ;

  string drawoptions[3]={"p", "p", "p"} ;
  string legendentries[3]={"GainRun1: 1.5 fC", "GainRun2: 2 fC", "GainRun3: 2.5 fC"} ;
  hdeadstripID_top1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hdeadstripID_top1->GetYaxis()->SetNdivisions(4) ;
  hloweffstripID_top1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hloweffstripID_top1->GetYaxis()->SetNdivisions(4) ;
  hnoisystripID_top1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hnoisystripID_top1->GetYaxis()->SetNdivisions(4) ;
  hxtalkstripID_top1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hxtalkstripID_top1->GetYaxis()->SetNdivisions(4) ;
  hbadstripID_top1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hbadstripID_top1->GetYaxis()->SetNdivisions(4) ;
  hdeadstripID_bottom1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hdeadstripID_bottom1->GetYaxis()->SetNdivisions(4) ;
  hloweffstripID_bottom1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hloweffstripID_bottom1->GetYaxis()->SetNdivisions(4) ;
  hnoisystripID_bottom1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hnoisystripID_bottom1->GetYaxis()->SetNdivisions(4) ;
  hxtalkstripID_bottom1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hxtalkstripID_bottom1->GetYaxis()->SetNdivisions(4) ;
  hbadstripID_bottom1->GetYaxis()->SetRangeUser(0.5, 4.5) ;
  hbadstripID_bottom1->GetYaxis()->SetNdivisions(4) ;
  if(hdeadstripID_top1->GetEntries()!=0||hdeadstripID_top2->GetEntries()!=0||hdeadstripID_top3->GetEntries()!=0)
    draw3LongHists("DeadStripID_Top",hdeadstripID_top1, hdeadstripID_top2, hdeadstripID_top3, drawoptions, "Dead Strip", legendentries) ;
  if(hloweffstripID_top1->GetEntries()!=0||hloweffstripID_top2->GetEntries()!=0||hloweffstripID_top3->GetEntries()!=0)
    draw3LongHists("LowEffStripID_Top",hloweffstripID_top1, hloweffstripID_top2, hloweffstripID_top3, drawoptions, "LowEff Strip", legendentries) ;
  if(hnoisystripID_top1->GetEntries()!=0||hnoisystripID_top2->GetEntries()!=0||hnoisystripID_top3->GetEntries()!=0)
    draw3LongHists("NoisyStripID_Top",hnoisystripID_top1, hnoisystripID_top2, hnoisystripID_top3, drawoptions, "Noisy Strip", legendentries) ;
  if(hxtalkstripID_top1->GetEntries()!=0||hxtalkstripID_top2->GetEntries()!=0||hxtalkstripID_top3->GetEntries()!=0)
    draw3LongHists("XtalkStripID_Top",hxtalkstripID_top1, hxtalkstripID_top2, hxtalkstripID_top3, drawoptions, "Xtalk Strip", legendentries) ;
  if(hbadstripID_top1->GetEntries()!=0||hbadstripID_top2->GetEntries()!=0||hbadstripID_top3->GetEntries()!=0)
    draw3LongHists("BadStripID_Top",hbadstripID_top1, hbadstripID_top2, hbadstripID_top3, drawoptions, "Bad Strip", legendentries) ;
  if(hdeadstripID_bottom1->GetEntries()!=0||hdeadstripID_bottom2->GetEntries()!=0||hdeadstripID_bottom3->GetEntries()!=0)
    draw3LongHists("DeadStripID_Bottom",hdeadstripID_bottom1, hdeadstripID_bottom2, hdeadstripID_bottom3, drawoptions, "Dead Strip", legendentries) ;
  if(hloweffstripID_bottom1->GetEntries()!=0||hloweffstripID_bottom2->GetEntries()!=0||hloweffstripID_bottom3->GetEntries()!=0)
    draw3LongHists("LowEffStripID_Bottom",hloweffstripID_bottom1, hloweffstripID_bottom2, hloweffstripID_bottom3, drawoptions, "LowEff Strip", legendentries) ;
  if(hnoisystripID_bottom1->GetEntries()!=0||hnoisystripID_bottom2->GetEntries()!=0||hnoisystripID_bottom3->GetEntries()!=0)
    draw3LongHists("NoisyStripID_Bottom",hnoisystripID_bottom1, hnoisystripID_bottom2, hnoisystripID_bottom3, drawoptions, "Noisy Strip", legendentries) ;
  if(hxtalkstripID_bottom1->GetEntries()!=0||hxtalkstripID_bottom2->GetEntries()!=0||hxtalkstripID_bottom3->GetEntries()!=0)
    draw3LongHists("XtalkStripID_Bottom",hxtalkstripID_bottom1, hxtalkstripID_bottom2, hxtalkstripID_bottom3, drawoptions, "Xtalk Strip", legendentries) ;
  if(hbadstripID_bottom1->GetEntries()!=0||hbadstripID_bottom2->GetEntries()!=0||hbadstripID_bottom3->GetEntries()!=0)
    draw3LongHists("BadStripID_Bottom",hbadstripID_bottom1, hbadstripID_bottom2, hbadstripID_bottom3, drawoptions, "Bad Strip", legendentries) ;
  
  TCanvas* c = new TCanvas("c", "", 1200, 600) ;
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
  delete c ;

  gStyle->SetPadRightMargin(0.15);
  c = new TCanvas("c", "", 800, 600) ;
  h_noise_vs_gain_top->Draw("colz")  ;
  c->Print("Noise_vs_Gain_2D_Top.pdf") ;
  h_noise_vs_gain_bottom->Draw("colz")  ;
  c->Print("Noise_vs_Gain_2D_Bottom.pdf") ;
  delete c ;

  return 0 ;
}
