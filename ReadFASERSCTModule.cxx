#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TString.h"

using namespace std ;
int ReadFASERSCTModule() 
{
  TString outPath = gSystem->pwd() ;
  outPath.ReplaceAll("user", "work") ;
  outPath += "/plotting/" ;
  if(gSystem->AccessPathName(outPath))
    gSystem->Exec("mkdir -p "+outPath) ;
  cout<<outPath<<endl ;

  TFile* f = TFile::Open(outPath+"FASERSCTModuleTest.root","RECREATE");
  TTree* tree = new TTree("tree", "");
  tree->ReadFile("FASER_SCTmodule_QA.txt") ;
  f->Write();
  f->Close() ;
  delete f ;

  return 0 ;
}
