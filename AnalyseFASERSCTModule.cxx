#include "./atlasrootstyle/AtlasStyle.C"
#include "./atlasrootstyle/AtlasLabels.C"
#include "./atlasrootstyle/AtlasUtils.C"

TH1F* bookTH1F(std::string name, std::string title,
               std::string xlabel,std::string ylabel,
               int xbins, double xlow, double xhigh,
               bool sumw2 = true, bool overflow=true) ;
int makePlot(TH1F* h, string name, string drawoption, float xLabel, float yLabel, string text,  float xText, float yText, int textColor) ;

using namespace std;
int AnalyseFASERSCTModule()
{
  // Set ATLAS Style
  SetAtlasStyle() ;

  TString inPath = gSystem->pwd() ;
  inPath.ReplaceAll("user", "work") ;
  inPath += "/plotting/" ;
  TFile* inFile = new TFile(inPath+"FASERSCTModuleTest.root", "read") ;
  if(!inFile)
  {
    cout<<"Can not open in File!!!"<<endl ;
    return 0 ;
  }

  TTree* tree = (TTree*) inFile->Get("tree") ;
  if(!tree)
  {
    cout<<"Can not get in Tree!!!"<<endl ;
    return 0 ;
  }

  TFile* outFile = new TFile(inPath+"FASERSCTModuleQA.root", "recreate") ;

  TH1F* bookTH1F(std::string name, std::string title,
               std::string xlabel,std::string ylabel,
               int xbins, double xlow, double xhigh,
               bool sumw2 = true, bool overflow=true) ;

  TH1F* h_leakagecurrent = bookTH1F("h_leakagecurrent", "", "Module Leakage Current at 150V [#muA]", "Modules", 30, 0, 3) ;
  TH1F* h_leakagecurrent_good = bookTH1F("h_leakagecurrent_good", "", "Module Leakage Current at 150V [#muA]", "Modules", 30, 0, 3) ;
  TH1F* h_leakagecurrent_medium = bookTH1F("h_leakagecurrent_medium", "", "Module Leakage Current at 150V [#muA]", "Modules", 30, 0, 3) ;
  
  TH1F* h_noise_notest = bookTH1F("h_noise_notest", "nosie extracted from noise occupancay test, injected charge is 0", "ENC, Noise Occupancy Test[e]", "Modules", 200, 500, 2500) ;
  h_noise_notest->GetXaxis()->SetNdivisions(1007) ;

  TH1F* h_noise_notest_good = bookTH1F("h_noise_notest_good", "nosie extracted from noise occupancay test, injected charge is 0", "ENC, Noise Occupancy Test[e]", "Modules", 200, 500, 2500) ;
  h_noise_notest_good->GetXaxis()->SetNdivisions(1007) ;
  TH1F* h_noise_notest_medium = bookTH1F("h_noise_notest_medium", "nosie extracted from noise occupancay test, injected charge is 0", "ENC, Noise Occupancy Test[e]", "Modules", 200, 500, 2500) ;
  h_noise_notest_medium->GetXaxis()->SetNdivisions(1007) ;

  TH1F* h_gain = bookTH1F("h_gain", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_good = bookTH1F("h_gain_good", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_medium = bookTH1F("h_gain_medium", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_upper = bookTH1F("h_gain_upper", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_upper_good = bookTH1F("h_gain_upper_good", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_upper_medium = bookTH1F("h_gain_upper_medium", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_lower = bookTH1F("h_gain_lower", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_lower_good = bookTH1F("h_gain_lower_good", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_lower_medium = bookTH1F("h_gain_lower_medium", "", "Gain(mV/fC)", "Modules", 25, 35, 60) ;
  TH1F* h_gain_diff = bookTH1F("h_gain_diff", "", "Gain[Upper-Lower](mV/fC)", "Modules", 20, -10, 10) ;
  TH1F* h_gain_diff_good = bookTH1F("h_gain_diff_good", "", "Gain[Upper-Lower](mV/fC)", "Modules", 20, -10, 10) ;
  TH1F* h_gain_diff_medium = bookTH1F("h_gain_diff_medium", "", "Gain[Upper-Lower](mV/fC)", "Modules", 20, -10, 10) ;

  TH1F* h_noise_rctest = bookTH1F("h_noise_rctest", "noise extracted from response curve test, injected charge is 2 fC", "ENC, Response Curve Test[e]", "Modules",  150, 1000, 2500) ;
  TH1F* h_noise_rctest_good = bookTH1F("h_noise_rctest_good", "noise extracted from response curve test, injected charge is 2 fC", "ENC, Response Curve Test[e]", "Modules",  150, 1000, 2500) ;
  TH1F* h_noise_rctest_medium = bookTH1F("h_noise_rctest_medium", "noise extracted from response curve test, injected charge is 2 fC", "ENC, Response Curve Test[e]", "Modules",  150, 1000, 2500) ;

  TH1F* h_noise_diff = bookTH1F("h_noise_diff", "noise extracted from response curve test, injected charge is 2 fC", "#frac{RC ENC-NO ENC}{RC ENC}[%]", "Modules", 15, -5, 10) ;
  TH1F* h_noise_diff_good = bookTH1F("h_noise_diff_good", "noise extracted from response curve test, injected charge is 2 fC", "#frac{RC ENC-NO ENC}{RC ENC}[%]", "Modules", 15, -5, 10) ;
  TH1F* h_noise_diff_medium = bookTH1F("h_noise_diff_medium", "noise extracted from response curve test, injected charge is 2 fC", "#frac{RC ENC-NO ENC}{RC ENC}[%]", "Modules", 15, -5, 10) ;

  TH1F* h_deadstrip = bookTH1F("h_deadstrip", "", "Dead Strips", "Modules", 5, 0, 5) ;
  h_deadstrip->GetXaxis()->SetNdivisions(5) ;
  TH1F* h_deadstrip_good = bookTH1F("h_deadstrip_good", "", "Dead Strips", "Modules", 5, 0, 5) ;
  h_deadstrip_good->GetXaxis()->SetNdivisions(5) ;
  TH1F* h_deadstrip_medium = bookTH1F("h_deadstrip_medium", "", "Dead Strips", "Modules", 5, 0, 5) ;
  h_deadstrip_medium->GetXaxis()->SetNdivisions(5) ;

  TH1F* h_loweffstrip = bookTH1F("h_loweffstrip", "","Low Efficiency Strips", "Modules", 10, 0, 10) ;
  h_loweffstrip->GetXaxis()->SetNdivisions(10) ;
  TH1F* h_loweffstrip_good = bookTH1F("h_loweffstrip_good", "","Low Efficiency Strips", "Modules", 10, 0, 10) ;
  h_loweffstrip_good->GetXaxis()->SetNdivisions(10) ;
  TH1F* h_loweffstrip_medium = bookTH1F("h_loweffstrip_medium", "","Low Efficiency Strips", "Modules", 10, 0, 10) ;
  h_loweffstrip_medium->GetXaxis()->SetNdivisions(10) ;

  TH1F* h_noisystrip = bookTH1F("h_noisystrip", "", "Noisy Strips", "Modules", 11, 0, 11) ;
  h_noisystrip->GetXaxis()->SetNdivisions(11) ;
  TH1F* h_noisystrip_good = bookTH1F("h_noisystrip_good", "", "Noisy Strips", "Modules", 11, 0, 11) ;
  h_noisystrip_good->GetXaxis()->SetNdivisions(11) ;
  TH1F* h_noisystrip_medium = bookTH1F("h_noisystrip_medium", "", "Noisy Strips", "Modules", 11, 0, 11) ;
  h_noisystrip_medium->GetXaxis()->SetNdivisions(11) ;

  TH1F* h_xtalkstrip = bookTH1F("h_xtalkstrip", "", "Xtalk Strips", "Modules", 11, 0, 11) ;
  h_xtalkstrip->GetXaxis()->SetNdivisions(11) ;
  TH1F* h_xtalkstrip_good = bookTH1F("h_xtalkstrip_good", "", "Xtalk Strips", "Modules", 11, 0, 11) ;
  h_xtalkstrip_good->GetXaxis()->SetNdivisions(11) ;
  TH1F* h_xtalkstrip_medium = bookTH1F("h_xtalkstrip_medium", "", "Xtalk Strips", "Modules", 11, 0, 11) ;
  h_xtalkstrip_medium->GetXaxis()->SetNdivisions(11) ;

  TH1F* h_badstrip = bookTH1F("h_badstrip", "", "Bad Strips", "Modules", 15, 0, 15) ;
  h_badstrip->GetXaxis()->SetNdivisions(15) ;
  TH1F* h_badstrip_good = bookTH1F("h_badstrip_good", "", "Bad Strips", "Modules", 15, 0, 15) ;
  h_badstrip_good->GetXaxis()->SetNdivisions(15) ;
  TH1F* h_badstrip_medium = bookTH1F("h_badstrip_medium", "", "Bad Strips", "Modules", 15, 0, 15) ;
  h_badstrip_medium->GetXaxis()->SetNdivisions(15) ;

  tree->Draw("LeakageCurret>>h_leakagecurrent") ;
  tree->Draw("LeakageCurret>>h_leakagecurrent_good", "Quality==\"G\"") ;
  tree->Draw("LeakageCurret>>h_leakagecurrent_medium", "Quality==\"M\"") ;

  tree->Draw("NoiseNOTest>>h_noise_notest") ;
  tree->Draw("NoiseNOTest>>h_noise_notest_good", "Quality==\"G\"") ;
  tree->Draw("NoiseNOTest>>h_noise_notest_medium", "Quality==\"M\"") ;

  tree->Draw("Gain>>h_gain") ;
  tree->Draw("Gain>>h_gain_good", "Quality==\"G\"") ;
  tree->Draw("Gain>>h_gain_medium", "Quality==\"M\"") ;
  tree->Draw("UpperGain>>h_gain_upper") ;
  tree->Draw("UpperGain>>h_gain_upper_good", "Quality==\"G\"") ;
  tree->Draw("UpperGain>>h_gain_upper_medium", "Quality==\"M\"") ;
  tree->Draw("LowerGain>>h_gain_lower") ;
  tree->Draw("LowerGain>>h_gain_lower_good", "Quality==\"G\"") ;
  tree->Draw("LowerGain>>h_gain_lower_medium", "Quality==\"M\"") ;
  tree->Draw("(UpperGain-LowerGain)>>h_gain_diff") ;
  tree->Draw("(UpperGain-LowerGain)>>h_gain_diff_good", "Quality==\"G\"") ;
  tree->Draw("(UpperGain-LowerGain)>>h_gain_diff_medium", "Quality==\"M\"") ;

  tree->Draw("NoiseRCTest>>h_noise_rctest") ;
  tree->Draw("NoiseRCTest>>h_noise_rctest_good", "Quality==\"G\"") ;
  tree->Draw("NoiseRCTest>>h_noise_rctest_medium", "Quality==\"M\"") ;
  tree->Draw("(NoiseRCTest-NoiseNOTest)/NoiseRCTest*100>>h_noise_diff") ;
  tree->Draw("(NoiseRCTest-NoiseNOTest)/NoiseRCTest*100>>h_noise_diff_good", "Quality==\"G\"") ;
  tree->Draw("(NoiseRCTest-NoiseNOTest)/NoiseRCTest*100>>h_noise_diff_medium", "Quality==\"M\"") ;

  tree->Draw("(NDead+NLoweff+NNoisy+NXtalk)>>h_badstrip") ;
  tree->Draw("(NDead+NLoweff+NNoisy+NXtalk)>>h_badstrip_good", "Quality==\"G\"") ;
  tree->Draw("(NDead+NLoweff+NNoisy+NXtalk)>>h_badstrip_medium", "Quality==\"M\"") ;
  tree->Draw("NDead>>h_deadstrip") ;
  tree->Draw("NDead>>h_deadstrip_good", "Quality==\"G\"") ;
  tree->Draw("NDead>>h_deadstrip_medium", "Quality==\"M\"") ;
  tree->Draw("NLoweff>>h_loweffstrip") ;
  tree->Draw("NLoweff>>h_loweffstrip_good", "Quality==\"G\"") ;
  tree->Draw("NLoweff>>h_loweffstrip_medium", "Quality==\"M\"") ;
  tree->Draw("NNoisy>>h_noisystrip") ;
  tree->Draw("NNoisy>>h_noisystrip_good", "Quality==\"G\"") ;
  tree->Draw("NNoisy>>h_noisystrip_medium", "Quality==\"M\"") ;
  tree->Draw("NXtalk>>h_xtalkstrip") ;
  tree->Draw("NXtalk>>h_xtalkstrip_good", "Quality==\"G\"") ;
  tree->Draw("NXtalk>>h_xtalkstrip_medium", "Quality==\"M\"") ;

  // making plots
  makePlot(h_noise_notest, string(inPath)+"Noise_NOTest", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_noise_notest_good, string(inPath)+"Noise_NOTest_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_noise_notest_medium, string(inPath)+"Noise_NOTest_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_gain, string(inPath)+"Gain", "Hist", 0.7, 0.85, "", 0.7, 0.75, 1) ;
  makePlot(h_gain_good, string(inPath)+"Gain_Good", "Hist", 0.7, 0.85, "Good", 0.7, 0.75, 2) ;
  makePlot(h_gain_medium, string(inPath)+"Gain_Medium", "Hist", 0.7, 0.85, "Medium", 0.7, 0.75, 4) ;
  makePlot(h_gain_upper, string(inPath)+"Gain_Upper", "Hist", 0.7, 0.85, "", 0.7, 0.75, 1) ;
  makePlot(h_gain_upper_good, string(inPath)+"Gain_Upper_Good", "Hist", 0.7, 0.85, "Good", 0.7, 0.75, 2) ;
  makePlot(h_gain_upper_medium, string(inPath)+"Gain_Upper_Medium", "Hist", 0.7, 0.85, "Medium", 0.7, 0.75, 4) ;
  makePlot(h_gain_lower, string(inPath)+"Gain_Lower", "Hist", 0.7, 0.85, "", 0.7, 0.75, 1) ;
  makePlot(h_gain_lower_good, string(inPath)+"Gain_Lower_Good", "Hist", 0.6, 0.85, "Good", 0.7, 0.75, 2) ;
  makePlot(h_gain_lower_medium, string(inPath)+"Gain_Lower_Medium", "Hist", 0.7, 0.85, "Medium", 0.7, 0.75, 4) ;

  makePlot(h_gain_diff, string(inPath)+"Gain_Diff", "Hist", 0.7, 0.85, "", 0.7, 0.75, 1) ;
  makePlot(h_gain_diff_good, string(inPath)+"Gain_Diff_Good", "Hist", 0.7, 0.85, "Good", 0.7, 0.75, 2) ;
  makePlot(h_gain_diff_medium, string(inPath)+"Gain_Diff_Medium", "Hist", 0.7, 0.85, "Medium", 0.7, 0.75, 4) ;

  makePlot(h_leakagecurrent, string(inPath)+"LeakageCurrent", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_leakagecurrent_good, string(inPath)+"LeakageCurrent_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_leakagecurrent_medium, string(inPath)+"LeakageCurrent_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_noise_rctest, string(inPath)+"Noise_RCTest", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_noise_rctest_good, string(inPath)+"Noise_RCTest_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_noise_rctest_medium, string(inPath)+"Noise_RCTest_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_noise_diff, string(inPath)+"Noise_Diff", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_noise_diff_good, string(inPath)+"Noise_Diff_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_noise_diff_medium, string(inPath)+"Noise_Diff_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_badstrip, string(inPath)+"BadStrips", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_badstrip_good, string(inPath)+"BadStrips_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_badstrip_medium, string(inPath)+"BadStrips_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_deadstrip, string(inPath)+"DeadStrips", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_deadstrip_good, string(inPath)+"DeadStrips_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_deadstrip_medium, string(inPath)+"DeadStrips_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_loweffstrip, string(inPath)+"LowEffStrips", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_loweffstrip_good, string(inPath)+"LowEffStrips_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_loweffstrip_medium, string(inPath)+"LowEffStrips_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_noisystrip, string(inPath)+"NoisyStrips", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_noisystrip_good, string(inPath)+"NoisyStrips_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_noisystrip_medium, string(inPath)+"NoisyStrips_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  makePlot(h_xtalkstrip, string(inPath)+"Xtalkstrips", "Hist", 0.6, 0.85, "", 0.6, 0.75, 1) ;
  makePlot(h_xtalkstrip_good, string(inPath)+"Xtalkstrips_Good", "Hist", 0.6, 0.85, "Good", 0.6, 0.75, 2) ;
  makePlot(h_xtalkstrip_medium, string(inPath)+"Xtalkstrips_Medium", "Hist", 0.6, 0.85, "Medium", 0.6, 0.75, 4) ;

  outFile->Write() ;
  outFile->Close() ;

  return 0 ;
}

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

int makePlot(TH1F* h, string name, string drawoption, float xLabel, float yLabel, string text,  float xText, float yText, int textColor)
{
  TCanvas* c = new TCanvas("c", "", 800, 600) ;
  h->Draw(drawoption.c_str()) ;
  FASERLabel(xLabel, yLabel,"Internal", 1);
  myText(xText, yText, textColor, text.c_str()) ;
  name += ".pdf" ;
  c->Print(name.c_str()) ;
  delete c;
  return 0 ;
}
