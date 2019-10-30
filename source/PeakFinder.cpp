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

void PeakFinder::SetLowThreshold(double threshold)
{
  lowThreshold = threshold;
}
    
void PeakFinder::SetHighThreshold(double threshold)
{
  highThreshold = threshold;
}

void PeakFinder::SetMinHeight(double height)
{
  minHeight = height;
}
    
double PeakFinder::GetLowThreshold()
{
  return lowThreshold;
}
    
double PeakFinder::GetHighThreshold()
{
  return highThreshold;
}

double PeakFinder::GetMinHeight()
{
  return minHeight;
}
