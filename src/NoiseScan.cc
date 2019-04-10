#include "NoiseScan.h"
#include <fstream>
#include <sstream>
#include <time.h>
#include <map>
#include <stdio.h>
#include "TROOT.h"
#include "TSystem.h"
#include "TClass.h"
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

NoiseScan::NoiseScan(){
  finitialize = false;
}

NoiseScan::~NoiseScan(){
  if(finitialize){
    delete hnoise;
    delete hvt50;
    for(int i=0; i<nside; i++){
      delete hhit[i];
      for(int j=0; j<nstrip; j++){
	delete hthr[i][j];
	delete func[i][j];
      }
    }
  }
}

void NoiseScan::Initialize(){
  finitialize = true;

  hnoise = new TH1F("hnoise", "ENC (Gain: 50mV/fC)", 100, 0., 3500.);
  hnoise->SetXTitle("ENC[e]");
  hnoise->SetYTitle("Strips");

  hvt50 = new TH1F("hvt50", "Vt50 (Gain: 50mV/fC)", 100, 0., 3.);
  hvt50->SetXTitle("Vt50[fC]");
  hvt50->SetYTitle("Strips");

  for(int i=0; i<nside; i++){
    std::ostringstream ohhit;
    ohhit << "hhit_" << i;
    hhit[i] = new TH1F((ohhit.str()).c_str(), "", nstrip, 0.5, (double)nstrip + 0.5);
    hhit[i]->SetXTitle("Strip number");

    for(int j=0; j<nstrip; j++){
      std::ostringstream ohthr;
      ohthr << "hthr_" << i << "_" << j;
      hthr[i][j] = new TH1F((ohthr.str()).c_str(), "", ntag, 0.5, (double)ntag + 0.5);
      hthr[i][j]->SetXTitle("Threshold DAC");

      std::ostringstream ofunc;
      ofunc << "func_" << i << "_" << j;
      func[i][j] = new TF1((ofunc.str()).c_str(),"[0]*TMath::Erfc((x-[1])/([2]*TMath::Sqrt(2)))", 0.5, (double)ntag + 0.5);
      func[i][j]->SetLineColor(40);
      func[i][j]->SetLineWidth(1);
    }
  }
}

void NoiseScan::ReadTree(int runid, std::string &srun){
  std::ostringstream ofile;  
  ofile << "feb-802932E1-";
  char runstring[32];
  sprintf(runstring, "%06d", runid);
  ofile << runstring;
  //if(runid < 10) ofile << "00000" << runid;
  //else if(runid < 100) ofile << "0000" << runid;
  //else if(runid < 1000) ofile << "000" << runid;
  //else if(runid < 10000) ofile << "00" << runid;

  ofile << ".root";
  std::string sfile = ofile.str();

  int end = sfile.find(".root");
  int start = 0;
  if(atoi(sfile.substr(end - 4, 1).c_str()) > 0) start = end - 4;
  else if(atoi(sfile.substr(end - 3, 1).c_str()) > 0) start = end - 3;
  else if(atoi(sfile.substr(end - 2, 1).c_str()) > 0) start = end - 2;
  else if(atoi(sfile.substr(end - 1, 1).c_str()) > 0) start = end - 1;
  srun = sfile.substr(start, end - start);

  FileStat_t info;
  TString fin = sfile;
  if(gSystem->GetPathInfo(fin.Data(), info) != 0 ){
    std::cout << sfile << " does not exist!" << std::endl;
    exit(1);
  }

  chain = new TChain("mdfTree", "tree_title");
  chain->Add(sfile.c_str());
}

void NoiseScan::FillEvents(){
  int tag1;
  int strip1[nstrip];
  int strip2[nstrip];
  int nhit1;
  int nhit2;
  chain->SetBranchAddress("tag1", &tag1);
  chain->SetBranchAddress("list1", &strip1);
  chain->SetBranchAddress("list2", &strip2);
  chain->SetBranchAddress("nhitsSilicon1", &nhit1);
  chain->SetBranchAddress("nhitsSilicon2", &nhit2);

  int tag_pre = 0;
  int ncalmode = 0;
  Long64_t nentries = chain->GetEntries();
  for(Long64_t i=0; i<nentries; i++){
    chain->GetEntry(i);
    
    if(i != 0 && tag1 < tag_pre) ncalmode++;
    tag_pre = tag1;

    int calmode = 0;
    if(ncalmode%4 == 0) calmode = 0; // strip0, 4, 8,...
    else if(ncalmode%4 == 1) calmode = 2; // strip2, 6, 10,...
    else if(ncalmode%4 == 2) calmode = 1; // strip1, 5, 9,...
    else if(ncalmode%4 == 3) calmode = 3; // strip3, 7, 11,...

    for(int j=0; j<nhit1; j++){
      hhit[0]->Fill((double)strip1[j]);
      if(strip1[j] < nstrip) hthr[0][strip1[j]]->Fill((double)tag1);
    }

    for(int j=0; j<nhit2; j++){
      hhit[1]->Fill((double)strip2[j]);
      if(strip2[j] < nstrip) hthr[1][strip2[j]]->Fill((double)tag1);
    }
  }
}

void NoiseScan::PlotSummary(std::string srun){
  TCanvas *c1 = new TCanvas("c1", "small cluster energy", 0, 0, 600, 900);

  gStyle->SetOptStat(1110);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.9);
  gStyle->SetStatW(0.3);
  hnoise->SetStats(1);
  hvt50->SetStats(1);

  c1->Divide(2,3);
  for(int i=0; i<nstrip; i++){
    double nhit = hthr[0][i]->GetEntries();

    c1->cd(1); hhit[0]->Draw();
    c1->cd(2); hhit[1]->Draw();

    c1->cd(3);
    if(i == 0) hthr[0][i]->Draw("");
    else hthr[0][i]->Draw("psame");

    c1->cd(4);
    if(i == 0) hthr[0][i]->Draw("");
    else hthr[1][i]->Draw("psame");
  }

  c1->cd(5);
  hnoise->SetMaximum(hnoise->GetMaximum() * 1.2);
  hnoise->Draw();

  c1->cd(6);
  hvt50->SetMaximum(hvt50->GetMaximum() * 1.2);
  hvt50->Draw();

  std::string spng = "run_";
  spng += srun;
  spng += "_NoiseScanSummary.pdf";
  c1->Print(spng.c_str());

  std::cout << std::endl;
  std::cout << "=== Noise & Vt50(@Gain=50mV/fC) ==" << std::endl;
  std::cout << "* Noise: " << hnoise->GetMean() << " +- " << hnoise->GetRMS() << " electrons" << std::endl;
  std::cout << "* Vt50: " << hvt50->GetMean() << " +- " << hvt50->GetRMS() << " fC" << std::endl;
  std::cout << "==================================" << std::endl;
  std::cout << std::endl;

  delete c1;
}

void NoiseScan::PlotStripThrshold(std::string srun, Double_t thr_dac[nside][nstrip]){
  std::string spdf = "run_";
  spdf += srun;
  std::string spdf0 = spdf;
  std::string spdf1 = spdf;
  std::string spdf2 = spdf;
  spdf0 += "_NoiseScanAll.pdf[";
  spdf1 += "_NoiseScanAll.pdf";
  spdf2 += "_NoiseScanAll.pdf]";

  TCanvas *c2 = new TCanvas("c2", "small cluster energy", 0, 0, 800, 800);
  c2->Divide(8,8);
  int nn = 0;
  c2->Print(spdf0.c_str());
  TLine* line[nside][nstrip];
  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      std::ostringstream ohthr;
      ohthr << "hthr_" << i << "_" << j;
      hthr[i][j]->SetTitle((ohthr.str()).c_str());
      if(nn != 0 && nn%64 == 0){
	c2->Update();
	c2->Print(spdf1.c_str());
	c2->Clear();
	c2->Divide(8,8);
      }
      c2->cd(nn%64 + 1);
      hthr[i][j]->Draw();
      func[i][j]->Draw("same");
      line[i][j] = new TLine(thr_dac[i][j], 0., thr_dac[i][j], hthr[i][j]->GetMaximum() * 0.3);
      line[i][j]->SetLineColor(kRed);
      if(thr_dac[i][j] > 1.){
	gPad->SetLogy();
	line[i][j]->Draw();
      }
      nn++;
    }
  }
  c2->Print(spdf1.c_str());
  c2->Print(spdf2.c_str());

  delete c2;
}

void NoiseScan::Analyze(int runid){
  std::string srun;
  ReadTree(runid, srun);
  Initialize();
  FillEvents();

  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      int nbin = hthr[i][j]->GetNbinsX();
      double thr = 0;
      for(int m=0; m<nbin; m++){
	double nhit = hthr[i][j]->GetBinContent(m+1);
	if(nhit > max_noise/2.) thr = hthr[i][j]->GetXaxis()->GetBinCenter(m+1);
      }

      func[i][j]->SetParameter(1, 25.);
      func[i][j]->SetParameter(2, 5.);
      func[i][j]->FixParameter(0, max_noise/2.);

      std::ostringstream ofunc;
      ofunc << "func_" << i << "_" << j;
      hthr[i][j]->Fit((ofunc.str()).c_str(), "NQ");

      Double_t par[3];
      func[i][j]->GetParameters(&par[0]);

      Double_t noise = par[2] * 0.05 / 0.0001602;
      hnoise->Fill(noise);

      Double_t vt50 = par[1] * 0.05;
      hvt50->Fill(vt50);
    }
  }
}

void NoiseScan::MakePlots(int runid){

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetPalette(1);

  Analyze(runid);

  std::ostringstream orun;
  orun << runid;
  std::string srun = orun.str();

  PlotSummary(srun);
  Double_t thr_dac[nside][nstrip] = {};
  PlotStripThrshold(srun, thr_dac);
}

void NoiseScan::CheckNoisyStrips(int runid, Double_t thr_dac[nside][nstrip]){
  std::string srun;
  ReadTree(runid, srun);
  Initialize();
  FillEvents();

  int nnoise = 0;
  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      int binid = hthr[i][j]->GetXaxis()->FindBin(thr_dac[i][j]);
      Double_t occ =  hthr[i][j]->GetBinContent(binid)/(Double_t)max_noise;
      
      if(occ > 0.0005) nnoise++;
    }
  }

  PlotStripThrshold(srun, thr_dac);

  std::cout << std::endl;
  std::cout << "=== # strips (Noise occ. > 5x10^{-4} @ 1fC)==" << std::endl;
  std::cout << "* # of strips: " << nnoise << std::endl;
  std::cout << "=============================================" << std::endl;
  std::cout << std::endl;

}
