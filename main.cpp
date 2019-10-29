#include <iostream>
#include <memory>
#include <cstdlib>
#include <vector>
#include <TF1.h>
#include <TH1I.h>
#include <TVector3.h>
#include "TSpectrumFinder.h"
#include "YY1Detector.h"
#include "Calibrator.h"


using namespace std;

int main(int argc , char *argv[])
{
  TF1 f("f","TMath::Gaus(x,1000,10)+TMath::Gaus(x,2000,15)",0,4096);
  vector<shared_ptr<TH1>> spectra;
  vector<vector<double>> energies;
  for(int i=0; i<100; i++){
    char name[32];
    sprintf(name,"h_%d",i);
    shared_ptr<TH1I> h(new TH1I(name,"",4096,0,4096));
    h->FillRandom("f",500);
    spectra.push_back(h);
    vector<double> e = {0.8, 1.5};
    energies.push_back(e);
  }

  Calibrator c;
  c.SetLogFile("test.root");
  c.GetPeakFinder().SetSigma(10);
  c.Calibrate(spectra,energies);
  
  /*
  YY1Detector Yu1;
  TVector3 source(0.,0.,0.);
  Yu1.SetPosition(0.,0.,-90.);
  for(int i=0; i<16; i++){
    cout << Yu1.GetSegSolidAngle(i,source) << "\t" << Yu1.GetEffectiveThickness(i,source) << endl;
  }
  */
  return 0;
}
  
