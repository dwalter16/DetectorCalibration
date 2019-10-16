#include <iostream>
#include <memory>
#include <cstdlib>
#include <TF1.h>
#include <TH1I.h>
#include "TSpectrumFinder.h"

using namespace std;

int main(int argc , char *argv[])
{
  double sigma = atof(argv[1]);
  TF1 *f = new TF1("f","TMath::Gaus(x,15,1)",0,30);
  shared_ptr<TH1I> h(new TH1I("h","",300,0,30));
  h->FillRandom("f",1000);
  
  TSpectrumFinder finder;
  finder.SetSigma(sigma);
  vector<array<double,2>> peaks = finder.Search(h,1);
  cout << peaks.at(0).at(0) << "  " << peaks.at(0).at(1) << endl;
  return 0;
}
  
