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
TH2F* bookTH2F(std::string name, std::string title,
               std::string xlabel,std::string ylabel,
               int xbins, double xlow, double xhigh,
               int ybins, double ylow, double yhigh,
               bool sumw2 = true, bool overflow=true)
{
  TH2F* tmp = new TH2F( name.c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh);
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

int makeLongerPlot(TH1F* h, string name, string drawoption)
{
  TCanvas* c = new TCanvas("c", "", 1200, 600) ;
  h->Draw(drawoption.c_str()) ;
  //FASERLabel(xLabel, yLabel,"Internal", 1);
  //myText(xText, yText, textColor, text.c_str()) ;
  name += ".pdf" ;
  c->Print(name.c_str()) ;
  delete c;
  return 0 ;
}

int draw3LongHists(string name, TH1F* h1, TH1F* h2, TH1F* h3, string drawoptions[3], string header, string legendentries[3])
{
  TCanvas* c = new TCanvas("c", "", 1200, 600) ;
  h1->Draw(drawoptions[0].c_str()) ;
  h2->Draw((drawoptions[1]+"same").c_str()) ;
  h3->Draw((drawoptions[2]+"same").c_str()) ;
  TLegend* legend = new TLegend(0.2, 0.8, 0.8, 0.9);
  legend->SetHeader(header.c_str(), "c") ;
  //legend->SetBorderSize(0) ;
  legend->SetFillStyle(4000);
  legend->SetShadowColor(0) ;
  legend->SetNColumns(3);
  legend->AddEntry(h1, legendentries[0].c_str(), "p") ;
  legend->AddEntry(h2, legendentries[1].c_str(), "p") ;
  legend->AddEntry(h3, legendentries[2].c_str(), "p") ;
  legend->Draw() ;
  name += ".pdf" ;
  c->Print(name.c_str()) ;
  delete legend ;
  delete c;
  return 0 ;
}
