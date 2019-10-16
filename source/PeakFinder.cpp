#include <iostream>
#include <cstdlib>
#include "PeakFinder.h"
#include "TSpectrumFinder.h"

using namespace std;

PeakFinder * PeakFinder::Create(string type)
{
  if(type == "TSpectrum"){
    return new TSpectrumFinder();
  }
  else{
    cout << "PeakFinder::Create(): Unknown peak finding algorithm: " << type << endl;
    exit(EXIT_FAILURE);
  }
}

void PeakFinder::SetSigma(double sig)
{
  sigma = sig;
}

double PeakFinder::GetSigma()
{
  return sigma;
}

