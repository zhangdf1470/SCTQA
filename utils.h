#include "./atlasrootstyle/AtlasStyle.C"
#include "./atlasrootstyle/AtlasLabels.C"
#include "./atlasrootstyle/AtlasUtils.C"

TH1F* bookTH1F(std::string name, std::string title,
               std::string xlabel,std::string ylabel,
               int xbins, double xlow, double xhigh,
               bool sumw2 = true, bool overflow=true)
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
