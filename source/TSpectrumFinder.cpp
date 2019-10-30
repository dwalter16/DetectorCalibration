#include <iostream>
#include <TMath.h>
#include "TSpectrumFinder.h"

using namespace std;

vector<array<double,2>> TSpectrumFinder::Search(shared_ptr<TH1> spectrum, int nPeaks, string option)
{
  //First, we limit the range of the histogram to be searched.
  spectrum->GetXaxis()->SetRangeUser(lowThreshold,highThreshold);
  
  //Then we let TSpectrum search for some peaks.
  tSpectrum.Search(spectrum.get(),sigma,option.c_str(),minHeight);
  
  //Check if something went wrong. More peaks than requested is ok, but not less.
  if(tSpectrum.GetNPeaks() < nPeaks){
    cout << "TSpectrumFinder::Search(): Warning: Only found " << tSpectrum.GetNPeaks();
    cout << " peaks, instead of the expected " << nPeaks << "!" << endl;
    vector<array<double,2>> result(nPeaks);
    return result;
  }
  
  //TSpectrum orders the found peaks according to their height, but we want them
  //sorted according to position.
  vector<int> indices (nPeaks);
  TMath::Sort(nPeaks,tSpectrum.GetPositionX(),indices.data(),false);
  
  //Then we fill the output vector with the peak positions.
  vector<array<double,2>> result;
  for(int i=0; i<nPeaks; i++){
    array<double,2> result_i;
    result_i.at(0) = tSpectrum.GetPositionX()[indices.at(i)];
    result.push_back(result_i);
  }
  
  //We also make an attempt to estimate the uncertainty of the result.
  //Here we take delta ~ sigma / sqrt(N).
  for(int i=0; i<nPeaks; i++){
    double binWidth = spectrum->GetBinWidth(0);
    double mu = result.at(i).at(0);
    int lowerBin = spectrum->FindBin(mu) - sigma;
    int upperBin = spectrum->FindBin(mu) + sigma;
    double counts = 1.5 * spectrum->Integral(lowerBin,upperBin);
    double error = sigma * binWidth / TMath::Sqrt(counts);
    error = error > binWidth ? error : binWidth; //TSpectrum can never do better than one bin width.
    result.at(i).at(1) = error;
    //cout << "mu = " << mu << ", lowerBin = " << lowerBin << ", upperBin = " ;
    //cout << upperBin << ", counts = " << counts << ", error = " << error << endl;
  }
  
  return result;
}

vector<array<double,2>> TSpectrumFinder::Search(shared_ptr<TH1> spectrum, int nPeaks)
{
  return Search(spectrum,nPeaks,"goff,nodraw");
}

TSpectrum & TSpectrumFinder::GetTSpectrum()
{
  return tSpectrum;
}
