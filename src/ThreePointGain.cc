#include "ThreePointGain.h"
#include "ThrScan.h"
#include "NoiseScan.h"
#include <fstream>
#include <sstream>
#include <time.h>
#include <map>
#include <stdio.h>
#include "TChain.h"
#include "TROOT.h"
#include "TFile.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TKey.h"
#include "Riostream.h"
#include "TEventList.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TLegend.h"
#include "TMath.h" 
#include "TEllipse.h" 

ThreePointGain::ThreePointGain(){

  finitialize = false;

  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      thr_dac[i][j] = 0;
    }
  }
}

ThreePointGain::~ThreePointGain(){
  if(finitialize){
    delete hgain;
    delete hgain_top;
    delete hgain_bottom;
    delete hgain_strip_top ;
    delete hgain_strip_bottom ;
    delete hoffset;
    delete hoffset_top;
    delete hoffset_bottom;
    delete hoffset_strip_top;
    delete hoffset_strip_bottom;
    delete hnoise;
    delete hnoise_top;
    delete hnoise_bottom;
    delete hnoise_strip_top;
    delete hnoise_strip_bottom;
    delete hdacthr;
    delete hdacthr_top;
    delete hdacthr_bottom;
    delete hdacthr_strip_top;
    delete hdacthr_strip_bottom;
    delete outFile ;
    for(int i=0; i<nside; i++){
      for(int j=0; j<nstrip; j++){
	delete gthree[i][j];
	delete func[i][j];
      }
    }
  }
}

void ThreePointGain::Initialize(){
  finitialize = true;

  // create out File
  outFile = new TFile("3PointsMeasurement.root", "recreate") ;
  // vector of bad strip IDs
  NBadStrips = new TVectorF(nrun_gain) ;
  TopNDeadStrips = new TVectorF(nrun_gain) ;
  TopNLowEffStrips = new TVectorF(nrun_gain) ;
  TopNNoisyStrips = new TVectorF(nrun_gain) ;
  TopNXtalkStrips = new TVectorF(nrun_gain) ;
  BottomNDeadStrips = new TVectorF(nrun_gain) ;
  BottomNLowEffStrips = new TVectorF(nrun_gain) ;
  BottomNNoisyStrips = new TVectorF(nrun_gain) ;
  BottomNXtalkStrips = new TVectorF(nrun_gain) ;
  for(int irun=0 ; irun<nrun_gain ; irun++)
  {
    TopDeadStripIDs[irun] = new TVectorF(nstrip) ;
    TopLowEffStripIDs[irun] = new TVectorF(nstrip) ;
    TopNoisyStripIDs[irun] = new TVectorF(nstrip) ;
    TopXtalkStripIDs[irun] = new TVectorF(nstrip) ;
    BottomDeadStripIDs[irun] = new TVectorF(nstrip) ;
    BottomNoisyStripIDs[irun] = new TVectorF(nstrip) ;
    BottomXtalkStripIDs[irun] = new TVectorF(nstrip) ;
    BottomLowEffStripIDs[irun] = new TVectorF(nstrip) ;
  }
  // new histograms of nosie
  hnoise = new TH1F("hnoise", "Nosie of the Module", 100, 0., 3500.);
  hnoise->SetDirectory(outFile) ;
  hnoise->SetXTitle("ENC[e]");
  hnoise->SetYTitle("Strips");
  hnoise_top = new TH1F("hnoise_top", "Nosie of the Top Sensor", 100, 0., 3500.);
  hnoise_top->SetDirectory(outFile) ;
  hnoise_top->SetXTitle("ENC[e]");
  hnoise_top->SetYTitle("Strips");
  hnoise_bottom = new TH1F("hnoise_bottom", "Nosie of the Back Sensor", 100, 0., 3500.);
  hnoise_bottom->SetDirectory(outFile) ;
  hnoise_bottom->SetXTitle("ENC[e]");
  hnoise_bottom->SetYTitle("Strips");
  hnoise_strip_top = new TH1F("hnoise_strip_top", "Nosie of each strip of the Top Sensor", nstrip , 0.5, nstrip+0.5);
  hnoise_strip_top->SetDirectory(outFile) ;
  hnoise_strip_top->SetYTitle("ENC[e]");
  hnoise_strip_top->SetXTitle("Top Strip ID");
  hnoise_strip_bottom = new TH1F("hnoise_strip_bottom", "Nosie of each strip of the Back Sensor", nstrip , 0.5, nstrip+0.5);
  hnoise_strip_bottom->SetDirectory(outFile) ;
  hnoise_strip_bottom->SetYTitle("ENC[e]");
  hnoise_strip_bottom->SetXTitle("Back Strip ID");

  // new histograms of gain
  hgain = new TH1F("hgain", "Gain", 100, 0., 100.);
  hgain->SetDirectory(outFile) ;
  hgain->SetXTitle("Gain[mV/fC]");
  hgain->SetYTitle("Strips");
  hgain_top = new TH1F("hgain_top", "Gain of Top Sensor", 100, 0., 100.);
  hgain_top->SetDirectory(outFile) ;
  hgain_top->SetXTitle("Top Gain[mV/fC]");
  hgain_top->SetYTitle("Strips");
  hgain_bottom = new TH1F("hgain_bottom", "Gain of Back Sensor", 100, 0., 100.);
  hgain_bottom->SetDirectory(outFile) ;
  hgain_bottom->SetXTitle("Back Gain[mV/fC]");
  hgain_bottom->SetYTitle("Strips");
  hgain_strip_top = new TH1F("hgain_strip_top", "Gain of each strip of Top Sensor", nstrip , 0.5, nstrip+0.5);
  hgain_strip_top->SetDirectory(outFile) ;
  hgain_strip_top->SetXTitle("Top Strip ID");
  hgain_strip_top->SetYTitle("Gain[mV/fC]");
  hgain_strip_bottom = new TH1F("hgain_strip_bottom", "Gain of each strip of Back Sensor", nstrip , 0.5, nstrip+0.5);
  hgain_strip_bottom->SetDirectory(outFile) ;
  hgain_strip_bottom->SetXTitle("Back Strip ID");
  hgain_strip_bottom->SetYTitle("Gain[mV/fC]");

  // new histograms of offset
  hoffset = new TH1F("hoffset", "Offset of the Module", 100, 0., 200.);
  hoffset->SetDirectory(outFile) ;
  hoffset->SetXTitle("Offset[mV]");
  hoffset->SetYTitle("Strips");
  hoffset_top = new TH1F("hoffset_top", "Offset of the Top Sensor", 100, 0., 200.);
  hoffset_top->SetDirectory(outFile) ;
  hoffset_top->SetXTitle("Offset[mV]");
  hoffset_top->SetYTitle("Strips");
  hoffset_bottom = new TH1F("hoffset_bottom", "Offset of the Back Sensor", 100, 0., 200.);
  hoffset_bottom->SetDirectory(outFile) ;
  hoffset_bottom->SetXTitle("Offset[mV]");
  hoffset_bottom->SetYTitle("Strips");
  hoffset_strip_top = new TH1F("hoffset_strip_top", "Offset of each strip of the Top Sensor", nstrip , 0.5, nstrip+0.5);
  hoffset_strip_top->SetDirectory(outFile) ;
  hoffset_strip_top->SetYTitle("Offset[mV]");
  hoffset_strip_top->SetXTitle("Top Strip ID");
  hoffset_strip_bottom = new TH1F("hoffset_strip_bottom", "Offset of each strip of the Back Sensor", nstrip , 0.5, nstrip+0.5);
  hoffset_strip_bottom->SetDirectory(outFile) ;
  hoffset_strip_bottom->SetYTitle("Offset[mV]");
  hoffset_strip_bottom->SetXTitle("Back Strip ID");

  // new histograms of DAC Threshold for 1 fC
  hdacthr = new TH1F("hdacthr", "DAC Threshold for 1 fC of the Module", 100, 0., 200.);
  hdacthr->SetDirectory(outFile) ;
  hdacthr->SetXTitle("1 fC Threshold[mV]");
  hdacthr->SetYTitle("Strips");
  hdacthr_top = new TH1F("hdacthr_top", "DAC Threshold for 1 fC of the Top Sensor", 100, 0., 200.);
  hdacthr_top->SetDirectory(outFile) ;
  hdacthr_top->SetXTitle("1 fC Threshold[mV]");
  hdacthr_top->SetYTitle("Strips");
  hdacthr_bottom = new TH1F("hdacthr_bottom", "DAC Threshold for 1 fC of the Back Sensor", 100, 0., 200.);
  hdacthr_bottom->SetDirectory(outFile) ;
  hdacthr_bottom->SetXTitle("1 fC Threshold[mV]");
  hdacthr_bottom->SetYTitle("Strips");
  hdacthr_strip_top = new TH1F("hdacthr_strip_top", "DAC Threshold for 1 fC of each strip of the Top Sensor", nstrip, 0.5, nstrip+0.5);
  hdacthr_strip_top->SetDirectory(outFile) ;
  hdacthr_strip_top->SetYTitle("1 fC Threshold[mV]");
  hdacthr_strip_top->SetXTitle("Top Strip ID");
  hdacthr_strip_bottom = new TH1F("hdacthr_strip_bottom", "DAC Threshold for 1 fC of of each strip of the Back Sensor", nstrip, 0.5, nstrip+0.5);
  hdacthr_strip_bottom->SetDirectory(outFile) ;
  hdacthr_strip_bottom->SetYTitle("1 fC Threshold[mV]");
  hdacthr_strip_bottom->SetXTitle("Back Strip ID");

  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      std::ostringstream ofunc;
      ofunc << "func_" << i << "_" << j;
      func[i][j] = new TF1((ofunc.str()).c_str(),"[0]+[1]*x", 0., 4.);
      func[i][j]->SetLineColor(kRed);
    }
  }      

}

void ThreePointGain::PlotSummary(std::string srun[]){
  TCanvas *c1 = new TCanvas("c1", "small cluster energy", 0, 0, 800, 600);

  //gStyle->SetOptStat(1110);
  gStyle->SetStatX(0.95);
  gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.3);
  hgain->SetStats(1);
  hgain_top->SetStats(1);
  hgain_bottom->SetStats(1);
  hgain_strip_top->SetStats(1);
  hgain_strip_bottom->SetStats(1);
  hoffset->SetStats(1);
  hnoise->SetStats(1);

  //hgain->SetMaximum(hgain->GetMaximum() * 1.2);
  hgain_top->SetMaximum(hgain_top->GetMaximum() * 1.2);
  hgain_bottom->SetMaximum(hgain_bottom->GetMaximum() * 1.2);
  hgain_strip_top->GetYaxis()->SetRangeUser(20, 70);
  hgain_strip_bottom->GetYaxis()->SetRangeUser(20, 70);

  //hoffset->SetMaximum(hoffset->GetMaximum() * 1.2);
  hoffset_strip_top->GetYaxis()->SetRangeUser(-20, 100);
  hoffset_strip_bottom->GetYaxis()->SetRangeUser(-20, 100);

  //hnoise->SetMaximum(hnoise->GetMaximum() * 1.2);
  hnoise_strip_top->GetYaxis()->SetRangeUser(1000, 2500);
  hnoise_strip_bottom->GetYaxis()->SetRangeUser(1000, 2500);

  hdacthr_strip_top->GetYaxis()->SetRangeUser(50, 150);
  hdacthr_strip_bottom->GetYaxis()->SetRangeUser(50, 150);

  // Draw histogram
  std::string spng = "run";
  for(int i=0; i<nrun_gain; i++){
    spng += "_";
    spng += srun[i];
  }
  spng += "_ThreePointGainSummary.pdf";
  c1->Print((spng+"[").c_str());

  hgain->Draw();
  c1->Print(spng.c_str());
  hgain_top->Draw();
  c1->Print(spng.c_str());
  hgain_bottom->Draw();
  c1->Print(spng.c_str());
  hgain_strip_top->Draw();
  c1->Print(spng.c_str());
  hgain_strip_bottom->Draw();
  c1->Print(spng.c_str());

  hoffset->Draw();
  c1->Print(spng.c_str());
  hoffset_top->Draw();
  c1->Print(spng.c_str());
  hoffset_bottom->Draw();
  c1->Print(spng.c_str());
  hoffset_strip_top->Draw();
  c1->Print(spng.c_str());
  hoffset_strip_bottom->Draw();
  c1->Print(spng.c_str());

  hnoise->Draw();
  c1->Print(spng.c_str());
  hnoise_top->Draw();
  c1->Print(spng.c_str());
  hnoise_bottom->Draw();
  c1->Print(spng.c_str());
  hnoise_strip_top->Draw();
  c1->Print(spng.c_str());
  hnoise_strip_bottom->Draw();
  c1->Print(spng.c_str());

  hdacthr->Draw();
  c1->Print(spng.c_str());
  hdacthr_top->Draw();
  c1->Print(spng.c_str());
  hdacthr_bottom->Draw();
  c1->Print(spng.c_str());
  hdacthr_strip_top->Draw();
  c1->Print(spng.c_str());
  hdacthr_strip_bottom->Draw();
  c1->Print(spng.c_str());

  c1->Print((spng+"]").c_str());

  // print the Gain of sensors
  std::cout << std::endl;
  std::cout << "=== Gain & Noise =============" << std::endl;
  std::cout << "* Gain : " << hgain->GetMean() << " +- " << hgain->GetRMS() << " mV/fC" << std::endl;
  std::cout << "* Gain of Top Sensor : " << hgain_top->GetMean() << " +- " << hgain_top->GetRMS() << " mV/fC" << std::endl;
  std::cout << "* Gain of Back Sensor : " << hgain_bottom->GetMean() << " +- " << hgain_bottom->GetRMS() << " mV/fC" << std::endl;
  std::cout << "* Noise: " << hnoise->GetMean() << " +- " << hnoise->GetRMS() << " electrons" << std::endl;
  std::cout << "==============================" << std::endl;
  std::cout << std::endl;

  delete c1;
}

void ThreePointGain::PlotStripGain(std::string srun[nrun_gain]){
  std::string spdf = "run";
  for(int i=0; i<nrun_gain; i++){
    spdf += "_";
    spdf += srun[i];
  }
  std::string spdf0 = spdf;
  std::string spdf1 = spdf;
  std::string spdf2 = spdf;
  spdf0 += "_ThreePointGainAll.pdf[";
  spdf1 += "_ThreePointGainAll.pdf";
  spdf2 += "_ThreePointGainAll.pdf]";

  TCanvas *c2 = new TCanvas("c2", "small cluster energy", 0, 0, 800, 600);
  c2->Divide(8,8);
  int nn = 0;
  c2->Print(spdf0.c_str());
  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      std::ostringstream ohthr;
      ohthr << "gain_" << i << "_" << j;
      gthree[i][j]->SetTitle((ohthr.str()).c_str());
      gthree[i][j]->Draw("apw");
      func[i][j]->Draw("same");
      TLatex latex;
      latex.SetNDC() ;
      latex.SetTextAlign(13);  //align at top
      if(i==0)
      {
        latex.DrawLatex(0.2,0.9,TString::Format("Top Sensor, Strip: %i", j+1));
        latex.DrawLatex(0.2,0.85,TString::Format("Gain: %0.2f mV/fC",hgain_strip_top->GetBinContent(j+1)));
        latex.DrawLatex(0.2,0.8,TString::Format("Offset: %0.2f mV",hoffset_strip_top->GetBinContent(j+1)));
        latex.DrawLatex(0.2,0.75,TString::Format("Noise: %0.2f electrons",hnoise_strip_top->GetBinContent(j+1)));
      }
      else
      {
        latex.DrawLatex(0.2,0.9,TString::Format("Back Sensor, Strip: %i", j+1));
        latex.DrawLatex(0.2,0.85,TString::Format("Gain: %0.2f mV/fC",hgain_strip_bottom->GetBinContent(j+1)));
        latex.DrawLatex(0.2,0.8,TString::Format("Offset: %0.2f mV",hoffset_strip_bottom->GetBinContent(j+1)));
        latex.DrawLatex(0.2,0.75,TString::Format("Noise: %0.2f electrons",hnoise_strip_bottom->GetBinContent(j+1)));
      }
      c2->Print(spdf1.c_str());
    }
  }
  c2->Print(spdf2.c_str());

  delete c2;
}

void ThreePointGain::MakePlots(int runid[nrun_gain]){

  //gStyle->SetOptStat(0);
  //gStyle->SetOptFit(0);
  gStyle->SetPalette(1);

  Double_t charge[nrun_gain] = {1.5, 2.0, 2.5}; //fC
  Double_t chargee[nrun_gain] = {0., 0., 0.}; //fC
  Double_t vt50[nside][nstrip][nrun_gain];
  Double_t vt50e[nside][nstrip][nrun_gain];
  Double_t sigma[nside][nstrip][nrun_gain];
  std::string srun[nrun_gain];
  for(int a=0; a<nrun_gain; a++){
    std::cout << "Analysing run" << runid[a] << std::endl;

    std::ostringstream orun;
    orun << runid[a];
    srun[a] = orun.str();

    ThrScan* m_ThrScan = new ThrScan();
    m_ThrScan->Analyze(runid[a]);

    Double_t parth[nside][nstrip][4];
    m_ThrScan->FitScurve(parth);

    for(int i=0; i<nside; i++){
      for(int j=0; j<nstrip; j++){  
	vt50[i][j][a] = parth[i][j][0];
	vt50e[i][j][a] = parth[i][j][2];
	sigma[i][j][a] = parth[i][j][1];
      }
      for(int n=0 ; n<nbad ; n++)
        vbad_strip[a][n][i] = m_ThrScan->vbad_strip[n][i] ;
    }

    //if(a == 1) m_ThrScan->PlotBadStrips(srun[a]);
    m_ThrScan->PlotBadStrips(srun[a]);
    delete m_ThrScan;
  }//for(int a=0; a<nrun_gain; a++)


  Initialize();

  for(int irun=0 ; irun<nrun_gain ; irun++)
  {
    (*NBadStrips)[irun] = vbad_strip[irun][0][0].size()+vbad_strip[irun][1][0].size()+vbad_strip[irun][2][0].size()+vbad_strip[irun][3][0].size()+vbad_strip[irun][0][1].size()+vbad_strip[irun][1][1].size()+vbad_strip[irun][2][1].size()+vbad_strip[irun][3][1].size() ;
    (*TopNDeadStrips)[irun] = vbad_strip[irun][0][0].size() ;
    (*TopNLowEffStrips)[irun] = vbad_strip[irun][1][0].size() ;
    (*TopNNoisyStrips)[irun] = vbad_strip[irun][2][0].size() ;
    (*TopNXtalkStrips)[irun] = vbad_strip[irun][3][0].size() ;
    (*BottomNDeadStrips)[irun] = vbad_strip[irun][0][0].size() ;
    (*BottomNLowEffStrips)[irun] = vbad_strip[irun][1][0].size() ;
    (*BottomNNoisyStrips)[irun] = vbad_strip[irun][2][0].size() ;
    (*BottomNXtalkStrips)[irun] = vbad_strip[irun][3][0].size() ;
    TopDeadStripIDs[irun]->ResizeTo(vbad_strip[irun][0][0].size()) ;
    TopLowEffStripIDs[irun]->ResizeTo(vbad_strip[irun][1][0].size()) ;
    TopNoisyStripIDs[irun]->ResizeTo(vbad_strip[irun][2][0].size()) ;
    TopXtalkStripIDs[irun]->ResizeTo(vbad_strip[irun][3][0].size()) ;
    BottomDeadStripIDs[irun]->ResizeTo(vbad_strip[irun][0][0].size()) ;
    BottomLowEffStripIDs[irun]->ResizeTo(vbad_strip[irun][1][0].size()) ;
    BottomNoisyStripIDs[irun]->ResizeTo(vbad_strip[irun][2][0].size()) ;
    BottomXtalkStripIDs[irun]->ResizeTo(vbad_strip[irun][3][0].size()) ;
    for(int isize = 0; isize<vbad_strip[irun][0][0].size() ; isize++)
      (*TopDeadStripIDs[irun])[isize] = vbad_strip[irun][0][0].at(isize)+1 ;
    for(int isize = 0; isize<vbad_strip[irun][1][0].size() ; isize++)
      (*TopLowEffStripIDs[irun])[isize] = vbad_strip[irun][1][0].at(isize)+1 ;
    for(int isize = 0; isize<vbad_strip[irun][2][0].size() ; isize++)
      (*TopNoisyStripIDs[irun])[isize] = vbad_strip[irun][2][0].at(isize)+1 ;
    for(int isize = 0; isize<vbad_strip[irun][3][0].size() ; isize++)
      (*TopXtalkStripIDs[irun])[isize] = vbad_strip[irun][3][0].at(isize)+1 ;
    for(int isize = 0; isize<vbad_strip[irun][0][1].size() ; isize++)
      (*BottomDeadStripIDs[irun])[isize] = vbad_strip[irun][0][1].at(isize)+1 ;
    for(int isize = 0; isize<vbad_strip[irun][1][1].size() ; isize++)
      (*BottomLowEffStripIDs[irun])[isize] = vbad_strip[irun][1][1].at(isize)+1 ;
    for(int isize = 0; isize<vbad_strip[irun][2][1].size() ; isize++)
      (*BottomNoisyStripIDs[irun])[isize] = vbad_strip[irun][2][1].at(isize)+1 ;
    for(int isize = 0; isize<vbad_strip[irun][3][1].size() ; isize++)
      (*BottomXtalkStripIDs[irun])[isize] = vbad_strip[irun][3][1].at(isize)+1 ;
  }

  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){  
      gthree[i][j] = new TGraphErrors(nrun_gain, charge, vt50[i][j], chargee, vt50e[i][j]);
      gthree[i][j]->GetXaxis()->SetTitle("Charge[fC]") ;
      gthree[i][j]->GetYaxis()->SetTitle("Vt50[mV]") ;

      std::ostringstream ofunc;
      ofunc << "func_" << i << "_" << j;
      gthree[i][j]->Fit((ofunc.str()).c_str(), "NQ");
      Double_t par[2];
      func[i][j]->GetParameters(&par[0]);

      // remove the bad strips
      bool isBad=false ;
      for(int m=0 ; m<nrun_gain ; m++)
      {
        for(int n=0 ; n<nbad ; n++)
        {
          std::vector<int>::iterator it = std::find(vbad_strip[m][n][i].begin(), vbad_strip[m][n][i].end(), j);
          if(it != vbad_strip[m][n][i].end())
          {
            isBad = true ;
            break ;
          }
        }
      }
      if(isBad)
        continue ;

      double offset = par[0]; // mV
      double gain = par[1]; // mV/fC
      double noise = sigma[i][j][1] / gain / 0.0001602; // electron
      // DAC Threshold for 1 fC
      thr_dac[i][j] = round((par[0] + par[1])); // mV
      // fill histograms
      hoffset->Fill(offset);
      hgain->Fill(gain);
      hnoise->Fill(noise);
      hdacthr->Fill(thr_dac[i][j]) ;
      if(i==0)
      {
        hgain_top->Fill(gain);
        hnoise_top->Fill(noise);
        hoffset_top->Fill(offset);
        hdacthr_top->Fill(thr_dac[i][j]) ;
        hgain_strip_top->SetBinContent(j+1, gain) ;
        hnoise_strip_top->SetBinContent(j+1, noise) ;
        hoffset_strip_top->SetBinContent(j+1, offset) ;
        hdacthr_strip_top->SetBinContent(j+1, thr_dac[0][j]) ;
      }
      else
      {
        hgain_bottom->Fill(gain);
        hnoise_bottom->Fill(noise);
        hoffset_bottom->Fill(offset);
        hdacthr_bottom->Fill(thr_dac[i][j]) ;
        hgain_strip_bottom->SetBinContent(j+1, gain) ;
        hnoise_strip_bottom->SetBinContent(j+1, noise) ;
        hoffset_strip_bottom->SetBinContent(j+1, offset) ;
        hdacthr_strip_bottom->SetBinContent(j+1, thr_dac[1][j]) ;
      }

    }
  }
  
  PlotSummary(srun);
  PlotStripGain(srun);

  // save out root file
  outFile->cd() ;
  NBadStrips->Write("NBadStrips") ;
  TopNDeadStrips->Write("TopNDeadStrip") ;
  TopNLowEffStrips->Write("TopNLowEffStrip") ;
  TopNNoisyStrips->Write("TopNNoisyStrip") ;
  TopNXtalkStrips->Write("TopNXtalkStrip") ;
  for(int irun=0 ; irun<nrun_gain ; irun++)
  {
    TopDeadStripIDs[irun]->Write(TString::Format("TopDeadStripIDs%d", irun+1)) ;
    TopLowEffStripIDs[irun]->Write(TString::Format("TopLowEffStripIDs%d", irun+1)) ;
    TopNoisyStripIDs[irun]->Write(TString::Format("TopNoisyStripIDs%d", irun+1)) ;
    TopXtalkStripIDs[irun]->Write(TString::Format("TopXtalkStripIDs%d", irun+1)) ;
    BottomDeadStripIDs[irun]->Write(TString::Format("BottomDeadStripIDs%d", irun+1)) ;
    BottomLowEffStripIDs[irun]->Write(TString::Format("BottomLowEffStripIDs%d", irun+1)) ;
    BottomNoisyStripIDs[irun]->Write(TString::Format("BottomNoisyStripIDs%d", irun+1)) ;
    BottomXtalkStripIDs[irun]->Write(TString::Format("BottomXtalkStripIDs%d", irun+1)) ;
  }
  outFile->Write() ;
}

void ThreePointGain::CheckNoisyStrips(int runid){
    std::cout << "Analysing run" << runid << std::endl;

    NoiseScan* m_NoiseScan = new NoiseScan();
    m_NoiseScan->CheckNoisyStrips(runid, thr_dac);

    delete m_NoiseScan;
}
