#include <iostream>
#include <string>

#include "TFile.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TVectorF.h"

#include "utils.h"
//#include "SummarizeQA.h"

using namespace std;
int SummarizeQA()
{
  // applying ATLAS Style
  SetAtlasStyle() ;

  TString baseinPath = gSystem->pwd() ;
  baseinPath.ReplaceAll("user", "work") ;
  TString inPath = baseinPath+"/plotting/" ;
  if(gSystem->AccessPathName(inPath))
  {
    cout<<inPath<<" does not exist!!!!"<<endl ;
    return 0;
  }

  TFile* inFile = new TFile(inPath+"FASERSCTModuleTest.root", "read") ;
  if(not inFile)
  {
    cout<<"Can not open inFile!!!!"<<endl ;
    exit ;
  }
  
  TTree* tree = (TTree*) inFile->Get("tree") ;
  long long ModuleID = 0 ;
  int ThresholdRunNumber1 = 0 ;
  int ThresholdRunNumber2 = 0 ;
  int ThresholdRunNumber3 = 0 ;
  int NDead = 0 ;
  int NLoweff = 0 ;
  int NNoisy = 0 ;
  int NXtalk = 0 ;

  tree->SetBranchAddress("ModuleID", &ModuleID) ;
  tree->SetBranchAddress("ThresholdRunNumber1", &ThresholdRunNumber1) ;
  tree->SetBranchAddress("ThresholdRunNumber2", &ThresholdRunNumber2) ;
  tree->SetBranchAddress("ThresholdRunNumber3", &ThresholdRunNumber3) ;
  tree->SetBranchAddress("NDead", &NDead) ;
  tree->SetBranchAddress("NLoweff", &NLoweff) ;
  tree->SetBranchAddress("NNoisy", &NNoisy) ;
  tree->SetBranchAddress("NXtalk", &NXtalk) ;
  
  int nEntries=tree->GetEntries();
  float noise[nEntries] ;
  float noise_top[nEntries] ;
  float noise_bottom[nEntries] ;
  float gain[nEntries] ;
  float gain_top[nEntries] ;
  float gain_bottom[nEntries] ;

  int ngainrun = 3 ;

  vector< vector <int> > nbadstrips ;
  int nbadstrip_arrary[nEntries] ;

  vector< vector <int> > topndeadstrips ;
  vector< vector <int> > topdeadstripsIDs ;
  vector< vector <int> > topnloweffstrips ;
  vector< vector <int> > toploweffstripsIDs ;
  vector< vector <int> > topnnoisystrips ;
  vector< vector <int> > topoisystripIDs ;
  vector< vector <int> > topnxtalkstrips ;
  vector< vector <int> > topxtalkstripsIDs ;

  vector< vector< int > > bottomndeadstrips ;
  vector< vector< int > > bottomnloweffstrips ;
  vector< vector< int > > bottomnnoisystrips ;
  vector< vector< int > > bottomnxtalkstrips ;
  vector< vector< int > > bottomdeadstripIDs ;
  vector< vector< int > > bottomloweffstripIDs ;
  vector< vector< int > > bottomnoisystripIDs ;
  vector< vector< int > > bottomxtalkstripIDs ;
  for(int irun = 0 ; irun<ngainrun ; irun++) 
  {
    nbadstrips.push_back(vector <int>()) ;

    topndeadstrips.push_back(vector <int>()) ;
    topnloweffstrips.push_back(vector <int>());
    topnnoisystrips.push_back(vector <int>()) ;
    topnxtalkstrips.push_back(vector <int>()) ;
    topdeadstripsIDs.push_back(vector <int>()) ;
    toploweffstripsIDs.push_back(vector <int>()) ;
    topoisystripIDs.push_back(vector <int>()) ;
    topxtalkstripsIDs.push_back(vector <int>()) ;

    bottomnloweffstrips.push_back(vector <int>()) ;
    bottomnnoisystrips.push_back(vector <int>()) ;
    bottomnxtalkstrips.push_back(vector <int>()) ;
    bottomndeadstrips.push_back(vector <int>()) ;
    bottomloweffstripIDs.push_back(vector <int>()) ;
    bottomnoisystripIDs.push_back(vector <int>()) ;
    bottomxtalkstripIDs.push_back(vector <int>()) ;
    bottomdeadstripIDs.push_back(vector <int>()) ;
  }

  //begin loop over tree
  for(int ientry=0 ; ientry<tree->GetEntries(); ientry++)
  {
    tree->GetEntry(ientry) ;
    nbadstrip_arrary[ientry] = NDead+NLoweff+NNoisy+NXtalk ;
    inPath = baseinPath+"/outPut/"+to_string(ModuleID)+"/" ;
    TString inFileName = "3PointsMeasurement_"+std::to_string(ThresholdRunNumber1)+"_"+std::to_string(ThresholdRunNumber2)+"_"+std::to_string(ThresholdRunNumber3)+".root" ;
    if(gSystem->AccessPathName(inPath+inFileName))
    {
      cout<<"Error: "<<inPath+inFileName<<" does not exist!!!!"<<endl ;
      return 0 ;
    }
    TFile* inMeasurementFile = new TFile(inPath+inFileName, "read") ;

    // extract bad stips
    TVectorF* NBadStrips = (TVectorF*) inMeasurementFile->Get("NBadStrips") ;
    TVectorF* TopNDeadStrip = (TVectorF*) inMeasurementFile->Get("TopNDeadStrip") ;
    TVectorF* TopNLowEffStrip = (TVectorF*) inMeasurementFile->Get("TopNLowEffStrip") ;
    TVectorF* TopNNoisyStrip = (TVectorF*) inMeasurementFile->Get("TopNNoisyStrip") ;
    TVectorF* TopNXtalkStrip = (TVectorF*) inMeasurementFile->Get("TopNXtalkStrip") ;
    TVectorF* BottomNDeadStrip = (TVectorF*) inMeasurementFile->Get("BottomNDeadStrip") ;
    TVectorF* BottomNLowEffStrip = (TVectorF*) inMeasurementFile->Get("BottomNLowEffStrip") ;
    TVectorF* BottomNNosiyStrip = (TVectorF*) inMeasurementFile->Get("BottomNNosiyStrip") ;
    TVectorF* BottomNXtalkStrip = (TVectorF*) inMeasurementFile->Get("BottomNXtalkStrip") ;
    for(int irun=0 ; irun<ngainrun ; irun++)
    {
      nbadstrips.at(irun).push_back((*NBadStrips)[irun]) ;
      topndeadstrips.at(irun).push_back((*TopNDeadStrip)[irun]) ;
      topnloweffstrips.at(irun).push_back((*TopNLowEffStrip)[irun]) ;
      topnnoisystrips.at(irun).push_back((*TopNNoisyStrip)[irun]) ;
      topnxtalkstrips.at(irun).push_back((*TopNDeadStrip)[irun]) ;
      bottomndeadstrips.at(irun).push_back((*TopNXtalkStrip)[irun]) ;
      bottomnloweffstrips.at(irun).push_back((*TopNLowEffStrip)[irun]) ;
      bottomnnoisystrips.at(irun).push_back((*TopNNoisyStrip)[irun]) ;
      bottomnxtalkstrips.at(irun).push_back((*TopNXtalkStrip)[irun]) ;
    }
    delete NBadStrips ;
    delete TopNDeadStrip ;
    delete TopNLowEffStrip ;
    delete TopNNoisyStrip ;
    delete TopNXtalkStrip ;

    // extract module average gain and noise
    TH1F* hnoise = (TH1F*) inMeasurementFile->Get("hnoise") ;
    TH1F* hnoise_top = (TH1F*) inMeasurementFile->Get("hnoise_top") ;
    TH1F* hnoise_bottom = (TH1F*) inMeasurementFile->Get("hnoise_bottom") ;
    TH1F* hgain = (TH1F*) inMeasurementFile->Get("hgain") ;
    TH1F* hgain_top = (TH1F*) inMeasurementFile->Get("hgain_top") ;
    TH1F* hgain_bottom = (TH1F*) inMeasurementFile->Get("hgain_bottom") ;
    noise[ientry] = hnoise->GetMean() ;
    noise_top[ientry] = hnoise_top->GetMean() ;
    noise_bottom[ientry] = hnoise_bottom->GetMean() ;
    gain[ientry] = hgain->GetMean() ;
    gain_top[ientry] = hgain_top->GetMean() ;
    gain_bottom[ientry] = hgain_bottom->GetMean() ;
    delete hnoise ;
    delete hnoise_top ;
    delete hnoise_bottom ;
    delete inMeasurementFile ;
  }
  // end loop over tree
  //delete tree ;
  //delete inFile ;

  for(int iEntry=0 ; iEntry<nEntries ; iEntry++)
  {
    if(nbadstrip_arrary[iEntry]!=nbadstrips.at(1).at(iEntry))
    {
      tree->GetEntry(iEntry) ;
      cout<<ModuleID<<endl ;
      cout<<nbadstrip_arrary[iEntry]<<":"<<nbadstrips.at(1).at(iEntry)<<endl ;
    }
  }
  // specify outPath and outFile 
  TString outPath = baseinPath+"/outPut/SummaryQA/" ;
  if(gSystem->AccessPathName(outPath))
    gSystem->Exec("mkdir -p "+outPath) ;

  TFile* outFile = new TFile(outPath+"SummaryQA.root", "recreate") ;

  // histograms of nosie
  TH1F* h_noise = bookTH1F("h_noise", "noise extracted from response curve test, injected charge is 2 fC", "ENC, Response Curve Test[e]", "Modules",  150, 1000, 2500) ;
  TH1F* h_noise_top = bookTH1F("h_noise_top", "noise,charge 2 fC", "ENC, Response Curve Test[e]", "Modules",  150, 1000, 2500) ;
  TH1F* h_noise_bottom = bookTH1F("h_noise_bottom", "noise, charge 2 fC", "ENC, Response Curve Test[e]", "Modules",  150, 1000, 2500) ;
  // histograms of gain
  TH1F* h_gain = bookTH1F("h_gain", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_top = bookTH1F("h_gain_top", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_bottom = bookTH1F("h_gain_bottom", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  // histograms of bad strips
  TH1F* h_badstrip = bookTH1F("h_badstrip", "Bad Strips in Noisy strips from Gain Run1, xtalk strip from Run3, low eff and dead strip from run2", "Bad Strips", "Modules", 15, 0, 15) ;

  TH1F* h_badstrip_gainrun1 = bookTH1F("h_badstrip_gainrun1", "Bad Strips in Gain Run1", "Bad Strips", "Modules", 15, 0, 15) ;
  TH1F* h_badstrip_gainrun2 = bookTH1F("h_badstrip_gainrun2", "Bad Strips in Gain Run2", "Bad Strips", "Modules", 15, 0, 15) ;
  TH1F* h_badstrip_gainrun3 = bookTH1F("h_badstrip_gainrun3", "Bad Strips in Gain Run3", "Bad Strips", "Modules", 15, 0, 15) ;
  vector< TH1F* > h_badstrip_vect ;
  h_badstrip_vect.push_back(h_badstrip_gainrun1) ;
  h_badstrip_vect.push_back(h_badstrip_gainrun2) ;
  h_badstrip_vect.push_back(h_badstrip_gainrun3) ;
  TH1F* h_deadstrip_gainrun1 = bookTH1F("h_deadstrip_gainrun1", "Dead Strips in Gain Run1", "Dead Strips", "Modules", 15, 0, 15) ;
  TH1F* h_deadstrip_gainrun2 = bookTH1F("h_deadstrip_gainrun2", "Dead Strips in Gain Run2", "Dead Strips", "Modules", 15, 0, 15) ;
  TH1F* h_deadstrip_gainrun3 = bookTH1F("h_deadstrip_gainrun3", "Dead Strips in Gain Run3", "Dead Strips", "Modules", 15, 0, 15) ;
  vector< TH1F* > h_deadstrip_vect ;
  h_deadstrip_vect.push_back(h_deadstrip_gainrun1) ;
  h_deadstrip_vect.push_back(h_deadstrip_gainrun2) ;
  h_deadstrip_vect.push_back(h_deadstrip_gainrun3) ;
  TH1F* h_noisystrip_gainrun1 = bookTH1F("h_noisystrip_gainrun1", "Noisy Strips in Gain Run1", "Noisy Strips", "Modules", 15, 0, 15) ;
  TH1F* h_noisystrip_gainrun2 = bookTH1F("h_noisystrip_gainrun2", "Noisy Strips in Gain Run2", "Noisy Strips", "Modules", 15, 0, 15) ;
  TH1F* h_noisystrip_gainrun3 = bookTH1F("h_noisystrip_gainrun3", "Noisy Strips in Gain Run3", "Noisy Strips", "Modules", 15, 0, 15) ;
  vector< TH1F* > h_noisystrip_vect ;
  h_noisystrip_vect.push_back(h_noisystrip_gainrun1) ;
  h_noisystrip_vect.push_back(h_noisystrip_gainrun2) ;
  h_noisystrip_vect.push_back(h_noisystrip_gainrun3) ;
  TH1F* h_loweffstrip_gainrun1 = bookTH1F("h_loweffstrip_gainrun1", "Low Efficiency Strips in Gain Run1", "Low Efficiency Strips", "Modules", 15, 0, 15) ;
  TH1F* h_loweffstrip_gainrun2 = bookTH1F("h_loweffstrip_gainrun2", "Low Efficiency Strips in Gain Run2", "Low Efficiency Strips", "Modules", 15, 0, 15) ;
  TH1F* h_loweffstrip_gainrun3 = bookTH1F("h_loweffstrip_gainrun3", "Low Efficiency Strips in Gain Run3", "Low Efficiency Strips", "Modules", 15, 0, 15) ;
  vector< TH1F* > h_loweffstrip_vect ;
  h_loweffstrip_vect.push_back(h_loweffstrip_gainrun1) ;
  h_loweffstrip_vect.push_back(h_loweffstrip_gainrun2) ;
  h_loweffstrip_vect.push_back(h_loweffstrip_gainrun3) ;
  TH1F* h_xtalkstrip_gainrun1 = bookTH1F("h_xtalkstrip_gainrun1", "Xtalk Strips in Gain Run1", "Xtalk Strips", "Modules", 15, 0, 15) ;
  TH1F* h_xtalkstrip_gainrun2 = bookTH1F("h_xtalkstrip_gainrun2", "Xtalk Strips in Gain Run2", "Xtalk Strips", "Modules", 15, 0, 15) ;
  TH1F* h_xtalkstrip_gainrun3 = bookTH1F("h_xtalkstrip_gainrun3", "Xtalk Strips in Gain Run3", "Xtalk Strips", "Modules", 15, 0, 15) ;
  vector< TH1F* > h_xtalkstrip_vect ;
  h_xtalkstrip_vect.push_back(h_xtalkstrip_gainrun1) ;
  h_xtalkstrip_vect.push_back(h_xtalkstrip_gainrun2) ;
  h_xtalkstrip_vect.push_back(h_xtalkstrip_gainrun3) ;

  for(int ibin=1; ibin<=15 ; ibin++)
  {
    for(int irun=0 ; irun<ngainrun ; irun++)
    {
      if(ibin==15)
      {
        h_badstrip->GetXaxis()->SetBinLabel(ibin, TString::Format("#geq%d", 15-1));
        h_badstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("#geq%d", 15-1));
        h_deadstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("#geq%d", 15-1));
        h_loweffstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("#geq%d", 15-1));
        h_noisystrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("#geq%d", 15-1));
        h_xtalkstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("#geq%d", 15-1));
      }
      else
      {
        h_badstrip->GetXaxis()->SetBinLabel(ibin, TString::Format("%d", ibin-1));
        h_badstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("%d", ibin-1));
        h_deadstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("%d", ibin-1));
        h_loweffstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("%d", ibin-1));
        h_noisystrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("%d", ibin-1));
        h_xtalkstrip_vect.at(irun)->GetXaxis()->SetBinLabel(ibin, TString::Format("%d", ibin-1));
      }
    }
  }
  
  for(int ientry=0 ; ientry<nEntries; ientry++)
  {
    h_noise->Fill(noise[ientry]) ;
    h_noise_top->Fill(noise_top[ientry]) ;
    h_noise_bottom->Fill(noise_bottom[ientry]) ;
    h_gain->Fill(gain[ientry]) ;
    h_gain_top->Fill(gain_top[ientry]) ;
    h_gain_bottom->Fill(gain_bottom[ientry]) ;
    for(int irun=0 ; irun<ngainrun ; irun++)
    {
      h_badstrip_vect.at(irun)->Fill(nbadstrips.at(irun).at(ientry)) ;
      h_deadstrip_vect.at(irun)->Fill(topndeadstrips.at(irun).at(ientry)+bottomndeadstrips.at(irun).at(ientry)) ;
      h_loweffstrip_vect.at(irun)->Fill(topnloweffstrips.at(irun).at(ientry)+bottomnloweffstrips.at(irun).at(ientry)) ;
      h_noisystrip_vect.at(irun)->Fill(topnnoisystrips.at(irun).at(ientry)+bottomnnoisystrips.at(irun).at(ientry)) ;
      h_xtalkstrip_vect.at(irun)->Fill(topnxtalkstrips.at(irun).at(ientry)+bottomnxtalkstrips.at(irun).at(ientry)) ;
    }
    h_badstrip->Fill(topndeadstrips.at(1).at(ientry)+bottomndeadstrips.at(1).at(ientry)+topnloweffstrips.at(1).at(ientry)+bottomnloweffstrips.at(1).at(ientry)+topnnoisystrips.at(0).at(ientry)+bottomnnoisystrips.at(0).at(ientry)+topnxtalkstrips.at(2).at(ientry)+bottomnxtalkstrips.at(2).at(ientry)) ;
  }
  h_badstrip_gainrun1->SetBinContent(h_badstrip_gainrun1->GetNbinsX(), h_badstrip_gainrun1->GetBinContent(h_badstrip_gainrun1->GetNbinsX())+h_badstrip_gainrun1->GetBinContent(h_badstrip_gainrun1->GetNbinsX()+1)) ;
  h_badstrip_gainrun2->SetBinContent(h_badstrip_gainrun2->GetNbinsX(), h_badstrip_gainrun2->GetBinContent(h_badstrip_gainrun2->GetNbinsX())+h_badstrip_gainrun2->GetBinContent(h_badstrip_gainrun2->GetNbinsX()+1)) ;
  h_badstrip_gainrun3->SetBinContent(h_badstrip_gainrun3->GetNbinsX(), h_badstrip_gainrun3->GetBinContent(h_badstrip_gainrun3->GetNbinsX())+h_badstrip_gainrun3->GetBinContent(h_badstrip_gainrun3->GetNbinsX()+1)) ;
  h_badstrip->SetBinContent(h_badstrip->GetNbinsX(), h_badstrip->GetBinContent(h_badstrip->GetNbinsX())+h_badstrip->GetBinContent(h_badstrip->GetNbinsX()+1)) ;

  h_noise->GetYaxis()->SetRangeUser(0, h_noise->GetMaximum()*1.2) ;
  h_noise_top->GetYaxis()->SetRangeUser(0, h_noise_top->GetMaximum()*1.2) ;
  h_noise_bottom->GetYaxis()->SetRangeUser(0, h_noise_bottom->GetMaximum()*1.2) ;
  h_gain->GetYaxis()->SetRangeUser(0, h_gain->GetMaximum()*1.2) ;
  h_gain_top->GetYaxis()->SetRangeUser(0, h_gain_top->GetMaximum()*1.2) ;
  h_gain_bottom->GetYaxis()->SetRangeUser(0, h_gain_bottom->GetMaximum()*1.2) ;
  makePlot(h_noise, (string)outPath+"Noise", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_noise_top, (string)outPath+"Noise_Top", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_noise_bottom, (string)outPath+"Noise_Bottom", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_gain, (string)outPath+"Gain", "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
  makePlot(h_gain_top, (string)outPath+"Gain_Top", "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
  makePlot(h_gain_bottom, (string)outPath+"Gain_Bottom", "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
  for(int irun=0 ; irun<ngainrun ; irun++)
  {
    makePlot(h_badstrip_vect.at(irun), (string)TString::Format(outPath+"BadStrips_GainRun%d",irun+1), "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
    makePlot(h_deadstrip_vect.at(irun), (string)TString::Format(outPath+"DeadStrips_GainRun%d",irun+1), "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
    makePlot(h_loweffstrip_vect.at(irun), (string)TString::Format(outPath+"LowEfficiencyStrips_GainRun%d",irun+1), "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
    makePlot(h_noisystrip_vect.at(irun), (string)TString::Format(outPath+"NoisyStrips_GainRun%d",irun+1), "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
    makePlot(h_xtalkstrip_vect.at(irun), (string)TString::Format(outPath+"XtalkStrips_GainRun%d",irun+1), "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
  }
  makePlot(h_badstrip, (string)outPath+"BadStrips", "Hist", 0.6, 0.85, "", 0.7, 0.75, 1) ;
  outFile->Write() ;
  outFile->Close() ;
  delete outFile ;

  return 0 ;
}
