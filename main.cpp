#include <iostream>
#include <memory>
#include <cstdlib>
#include <TF1.h>
#include <TH1I.h>
#include <TVector3.h>
#include "TSpectrumFinder.h"
#include "YY1Detector.h"


using namespace std;

int main(int argc , char *argv[])
{
  /*
  double sigma = atof(argv[1]);
  TF1 *f = new TF1("f","TMath::Gaus(x,15,1)",0,30);
  shared_ptr<TH1I> h(new TH1I("h","",300,0,30));
  h->FillRandom("f",1000);
  
  TSpectrumFinder finder;
  finder.SetSigma(sigma);
  vector<array<double,2>> peaks = finder.Search(h,1);
  cout << peaks.at(0).at(0) << "  " << peaks.at(0).at(1) << endl;
  */

  YY1Detector Yu1;
  TVector3 source(0.,0.,0.);
  Yu1.SetPosition(0.,0.,-90.);
  for(int i=0; i<16; i++){
    cout << Yu1.GetSegSolidAngle(i,source) << "\t" << Yu1.GetEffectiveThickness(i,source) << endl;
  }

  return 0;
}
  
