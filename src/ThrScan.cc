#include "ThrScan.h"
#include "NoiseScan.h"
#include <fstream>
#include <sstream>
#include <time.h>
#include <map>
#include <stdio.h>
#include "TSystem.h"
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

ThrScan::ThrScan(){
  finitialize = false;
}

ThrScan::~ThrScan(){
  if(finitialize){
    delete hbad;
    delete hnoise;
    delete hvt50;
    for(int i=0; i<nside; i++){
      delete hhit[i];
      for(int j=0; j<nstrip; j++){
	delete hthr[i][j];
	delete func[i][j];
	delete hxtalk[i][j];
      }
    }
  }
}

void ThrScan::Initialize(){
  finitialize = true;

  hbad = new TH1F("hbad", "Bad strips", nbad, -0.5, (double)nbad - 0.5);
  hbad->SetXTitle("Category");
  hbad->SetYTitle("Strips");
  std::string slabel[4];
  slabel[0] = "Dead";
  slabel[1] = "Low eff";
  slabel[2] = "Noisy";
  slabel[3] = "Xtalk";
  for(int i=0; i<nbad; i++) hbad->GetXaxis()->SetBinLabel(i+1, slabel[i].c_str());

  hnoise = new TH1F("hnoise", "ENC (Gain: 50mV/fC)", 100, 0., 3500.);
  hnoise->SetXTitle("ENC[e]");
  hnoise->SetYTitle("Strips");

  hvt50 = new TH1F("hvt50", "Vt50 (Gain: 50mV/fC)", 100, 0., 4.);
  hvt50->SetXTitle("Vt50[fC]");
  hvt50->SetYTitle("Strips");

  for(int i=0; i<nside; i++){
    std::ostringstream ohhit;
    ohhit << "hhit_" << i;
    hhit[i] = new TH1F((ohhit.str()).c_str(), "", nstrip, -0.5, (double)nstrip - 0.5);
    hhit[i]->SetXTitle("Strip Number");

    for(int j=0; j<nstrip; j++){
      std::ostringstream ohthr;
      ohthr << "hthr_" << i << "_" << j;
      hthr[i][j] = new TH1F((ohthr.str()).c_str(), "", ntag, -0.5, (double)ntag - 0.5);
      hthr[i][j]->SetXTitle("Threshold DAC[mV]");
      hthr[i][j]->SetYTitle("Hits");

      std::ostringstream ohxtalk;
      ohxtalk << "hxtalk_" << i << "_" << j;
      hxtalk[i][j] = new TH1F((ohxtalk.str()).c_str(), "", ntag, -0.5, (double)ntag - 0.5);
      hxtalk[i][j]->SetXTitle("Xtalkeshold DAC[mV]");
      hxtalk[i][j]->SetYTitle("Hits");

      std::ostringstream ofunc;
      ofunc << "func_" << i << "_" << j;
      func[i][j] = new TF1((ofunc.str()).c_str(),"[0]*TMath::Erfc((x-[1])/([2]*TMath::Sqrt(2)))", 0.5, (double)ntag + 0.5);
      func[i][j]->SetLineColor(40);
      func[i][j]->SetLineWidth(1);
    }
  }
}

void ThrScan::ReadTree(int runid, std::string &srun){
  std::ostringstream ofile;  
  ofile << "feb-802932E1-";
  char runstring[32];
  sprintf(runstring, "%06d", runid);
  ofile << runstring;

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

void ThrScan::FillEvents(){
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
      if(strip1[j] < nstrip){
	if(calmode == 0 && strip1[j]%4 == 0) hthr[0][strip1[j]]->Fill((double)tag1);
	else if(calmode == 1 && strip1[j]%4 == 1) hthr[0][strip1[j]]->Fill((double)tag1);
	else if(calmode == 2 && strip1[j]%4 == 2) hthr[0][strip1[j]]->Fill((double)tag1);
	else if(calmode == 3 && strip1[j]%4 == 3) hthr[0][strip1[j]]->Fill((double)tag1);
	else hxtalk[0][strip1[j]]->Fill((double)tag1);
      }
    }

    for(int j=0; j<nhit2; j++){
      hhit[1]->Fill((double)strip2[j]);
      if(strip2[j] < nstrip){
	if(calmode == 0 && strip2[j]%4 == 0) hthr[1][strip2[j]]->Fill((double)tag1);
	else if(calmode == 1 && strip2[j]%4 == 1) hthr[1][strip2[j]]->Fill((double)tag1);
	else if(calmode == 2 && strip2[j]%4 == 2) hthr[1][strip2[j]]->Fill((double)tag1);
	else if(calmode == 3 && strip2[j]%4 == 3) hthr[1][strip2[j]]->Fill((double)tag1);
	else hxtalk[1][strip2[j]]->Fill((double)tag1);
      }
    }
  }
}

void ThrScan::FitScurve(Double_t parth[nside][nstrip][4]){
  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      int nbin = hthr[i][j]->GetNbinsX();
      double thr = 0;
      for(int m=0; m<nbin; m++){
	double nhit = hthr[i][j]->GetBinContent(m+1);
	if(nhit > max_thr/2.) thr = hthr[i][j]->GetXaxis()->GetBinCenter(m+1);
      }

      func[i][j]->SetParameter(1, thr);
      func[i][j]->SetParameter(2, 5.);
      func[i][j]->FixParameter(0, max_thr/2.);

      std::ostringstream ofunc;
      ofunc << "func_" << i << "_" << j;
      hthr[i][j]->Fit((ofunc.str()).c_str(), "NQ");

      Double_t par[3];
      func[i][j]->GetParameters(&par[0]);
      func[i][j]->GetParameters(&par[0]);
      parth[i][j][0] = par[1] * 2.5; //Vt50 (mV) 
      parth[i][j][1] = par[2] * 2.5; //Sigma (mV)
      thr_dac[i][j] = par[1];

      parth[i][j][2] = func[i][j]->GetParError(1) * 2.5; //Error on Vt50 (mV) 
      parth[i][j][3] = func[i][j]->GetParError(2) * 2.5; //Error on Sigma (mV)
    }
  }
}

void ThrScan::PlotSummary(std::string srun){
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
  spng += "_ThrScanSummary.pdf";
  c1->Print(spng.c_str());

  std::cout << std::endl;
  std::cout << "=== Noise & Vt50(@Gain=50mV/fC) ==" << std::endl;
  std::cout << "* Noise: " << hnoise->GetMean() << " +- " << hnoise->GetRMS() << " electrons" << std::endl;
  std::cout << "* Vt50: " << hvt50->GetMean() << " +- " << hvt50->GetRMS() << " fC" << std::endl;
  std::cout << "==================================" << std::endl;
  std::cout << std::endl;

  delete c1;
  delete hbad;
  delete hnoise;
  delete hvt50;
  finitialize = false;
}

void ThrScan::PlotStripThrshold(std::string srun){
  std::string spdf = "run_";
  spdf += srun;
  std::string spdf0 = spdf;
  std::string spdf1 = spdf;
  std::string spdf2 = spdf;
  spdf0 += "_ThrScanAll.pdf[";
  spdf1 += "_ThrScanAll.pdf";
  spdf2 += "_ThrScanAll.pdf]";

  TCanvas *c2 = new TCanvas("c2", "small cluster energy", 0, 0, 800, 800);
  c2->Divide(8,8);
  int nn = 0;
  c2->Print(spdf0.c_str());
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
      nn++;
    }
  }
  c2->Print(spdf1.c_str());
  c2->Print(spdf2.c_str());

  delete c2;

  for(int i=0; i<nside; i++){
    delete hhit[i];
    for(int j=0; j<nstrip; j++){
      delete hthr[i][j];
      delete func[i][j];
    }
  }
  finitialize = false;
}

void ThrScan::FindBadStrips(){

  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      int nbin = hthr[i][j]->GetNbinsX();
      Double_t nhit_max = 0;
      for(int m=0; m<nbin; m++){
       	Double_t nhit = hthr[i][j]->GetBinContent(m+1);

	if(nhit_max < nhit) nhit_max = nhit;
      }
      
      if(nhit_max == 0){// Dead
	vbad_strip[0][i].push_back(j);
	hbad->Fill(0.);
        // Dengfeng added some lines to print the bad strip ID
        std::cout<<"Dead Strip: Side_"<<i<<", Strip_"<<j<<std::endl ;
	continue;
      }else if(nhit_max > 0 && nhit_max < 90){//Low efficiency
	vbad_strip[1][i].push_back(j);
	hbad->Fill(1.);
        // Dengfeng added some lines to print the bad strip ID
        std::cout<<"Low Efficiency Strip: Side_"<<i<<", Strip_"<<j<<std::endl ;
	continue;
      }

      int binid_noisy = 0;
      int nhit_pre = 0;
      bool flag_noisy = false;
      for(int m=0; m<nbin; m++){
       	Double_t nhit = hthr[i][j]->GetBinContent(nbin-m+1);
	if(nhit > 0){
	  if(nhit > 10 && nhit_pre == 0){
	    vbad_strip[2][i].push_back(j);
	    hbad->Fill(2.);
	    flag_noisy = true;
            // Dengfeng added some lines to print the bad strip ID
            std::cout<<"Noisy Strip: Side_"<<i<<", Strip_"<<j<<std::endl ;
	  }
	  break;
	}
	nhit_pre = nhit;
      }
      if(flag_noisy) continue;

      int binid_xtalk = 0;
      for(int m=0; m<nbin; m++){
	Double_t nhit = hthr[i][j]->GetBinContent(m+1);
	if(nhit > 95) binid_xtalk = m;
      }

      Double_t hit_xtalk = hxtalk[i][j]->GetBinContent(binid_xtalk+1);
      if(hit_xtalk > 10){
	vbad_strip[3][i].push_back(j);
	hbad->Fill(3.);
        // Dengfeng added some lines to print the bad strip ID
        std::cout<<"Xtalk Strip: Side_"<<i<<", Strip_"<<j<<std::endl ;
	continue;
      }
    }//for(int j=0; j<nstrip; j++)
  }//for(int i=0; i<nside; i++)

}

void ThrScan::PlotBadStrips(std::string srun){
  std::string spng = "run_";
  spng += srun;
  spng += "_bad.pdf";
  
  TCanvas *c = new TCanvas("c", "", 800, 600);
  c->Print((spng+"[").c_str()) ;
  hbad->Draw();
  c->Print(spng.c_str());

  for(int i=0; i<nbad; i++){
    for(int j=0; j<nside; j++){
      for(std::vector<int>::iterator it = vbad_strip[i][j].begin(); it!=vbad_strip[i][j].end(); it++)
      {
        std::string tag="";
        if(i==0) tag="Dead Strip";
        else if(i==1) tag="Low Effiency Strip)";
        else if(i==2) tag="Noisy Strip";
        else if(i==3) tag="Xtalk Strip";
        if(i < 3){
          hthr[j][*it]->Draw();
        }else if(i == 3){
          hthr[j][*it]->Draw();
	  hxtalk[j][*it]->SetLineColor(kRed);
	  hxtalk[j][*it]->Draw("same");
        }
        TLatex latex;
        latex.SetNDC() ;
        latex.SetTextAlign(13);  //align at top
        if(j==0)
        {
          latex.DrawLatex(0.2,0.9,TString::Format("Top Sensor, Strip: %i", (*it)+1));
          latex.DrawLatex(0.2,0.85,tag.c_str());
        }
        else
        {
          latex.DrawLatex(0.2,0.9,TString::Format("Back Sensor, Strip: %i", (*it)+1));
          latex.DrawLatex(0.2,0.85,tag.c_str());
        }
        c->Print(spng.c_str());
      }
    }//for(int j=0; j<nside; j++){
  }//for(int i=0; i<nbad; i++)
  c->Print((spng+"]").c_str());
  delete c;

  std::cout << std::endl;
  std::cout << "=== Bad strips ===============" << std::endl;
  for(int i=0; i<nbad; i++){
    double nbad = hbad->GetBinContent(i+1);
    const char* clabel = hbad->GetXaxis()->GetBinLabel(i+1);
    std::cout << "* " << clabel << ": " << nbad << std::endl;
  }
  std::cout << "==============================" << std::endl;
  std::cout << std::endl;
}


void ThrScan::Analyze(int runid){
  std::string srun;
  ReadTree(runid, srun);
  Initialize();
  FillEvents();

  Double_t parth[nside][nstrip][4];
  FitScurve(parth);

  for(int i=0; i<nside; i++){
    for(int j=0; j<nstrip; j++){
      Double_t noise = parth[i][j][1] * 0.05 / 0.0001602 / 2.5;
      hnoise->Fill(noise);

      Double_t vt50 = parth[i][j][0] * 0.05 / 2.5;
      hvt50->Fill(vt50);
    }
  }

  FindBadStrips();
}

void ThrScan::MakePlots(int runid){

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetPalette(1);

  Analyze(runid);

  std::ostringstream orun;
  orun << runid;
  std::string srun = orun.str();

  PlotSummary(srun);
  PlotStripThrshold(srun);
}

void ThrScan::CheckNoisyStrips(int runid){
  std::cout << "Analysing run" << runid << std::endl;

  NoiseScan* m_NoiseScan = new NoiseScan();
  m_NoiseScan->CheckNoisyStrips(runid, thr_dac);
  
  delete m_NoiseScan;
}
