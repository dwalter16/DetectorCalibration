#include "Calibrator.h"
#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TPolyMarker.h>
#include <TH2I.h>
#include <TDirectory.h>
#include <TRandom3.h>
#include <TMath.h>

using namespace std;
using namespace TMath;

Calibrator::Calibrator(string algorithm)
{
  peakFinder = shared_ptr<PeakFinder> (PeakFinder::Create(algorithm));
}

array<double,2> Calibrator::Calibrate(shared_ptr<TH1> spectrum, TF1 &func,
                                                       vector<double> energies)
{
  int nPeaks = energies.size();
  vector<array<double,2>> positions = peakFinder->Search(spectrum,nPeaks);
  
  TGraphErrors graph;
  for(int i=0; i<nPeaks; i++){
    graph.SetPoint(i,positions.at(i).at(0),energies.at(i));
    graph.SetPointError(i,0,positions.at(i).at(1));
  }
  graph.Fit(&func,"Q");
  
  array<double,2> result;
  result.at(0) = func.GetParameter(0);
  result.at(1) = func.GetParameter(1);
  
  if(logFile){
    //We plot the peak positions on the spectrum and save.
    vector<double> pos_vec;
    vector<double> heights;
    for(int i=0; i<nPeaks; i++){
      pos_vec.push_back(positions.at(i).at(0));
      heights.push_back(spectrum->GetBinContent(spectrum->FindBin(positions.at(i).at(0))));
    }
    //I tried creating the TPolyMarker on the stack, but then ROOT and memory management...
    TPolyMarker * pm = new TPolyMarker(nPeaks, pos_vec.data(), heights.data());
    spectrum->GetListOfFunctions()->Add(pm);
    pm->SetMarkerStyle(23);
    pm->SetMarkerColor(kRed);
    pm->SetMarkerSize(1.3);
    spectrum->Write("foundPeaks");
    
    //Store the linear fit.
    graph.SetMarkerStyle(20);
    graph.Write("calibrationFit");
  }
  return result;
} 

array<double,2> Calibrator::Calibrate(shared_ptr<TH1> spectrum,
                                      vector<double> energies, double pedestal)
{
  TF1 func("func","[0]*(x-[1])",0,4096);
  func.FixParameter(1,pedestal);
  return Calibrate(spectrum,func,energies);
}

array<double,2> Calibrator::Calibrate(shared_ptr<TH1> spectrum,
                                                       vector<double> energies)
{
  TF1 func("func","[0]*(x-[1])",0,4096);
  return Calibrate(spectrum,func,energies);                                                      
}

vector<array<double,2>> Calibrator::Calibrate(vector<shared_ptr<TH1>> spectra,
                     vector<vector<double>> energies, vector<double> pedestals)
{
  TDirectory *calDir;
  if(logFile) calDir = logFile->mkdir("calibrationResults");
  
  vector<array<double,2>> result;
  for(int i=0; i<spectra.size(); i++){
    if(logFile){
      char dirName[64];
      sprintf(dirName,"channel_%d",i);
      calDir->cd();
      TDirectory *chanDir = calDir->mkdir(dirName);
      chanDir->cd();
    }
    shared_ptr<TH1> si = spectra.at(i);
    vector<double> ei = energies.at(i);
    double pi = pedestals.at(i);
    result.push_back(Calibrate(si,ei,pi));
  }
  
  if(logFile){
    calDir->cd();
    EvalAndLog(spectra,result);
  }
  return result;
}

vector<array<double,2>> Calibrator::Calibrate(vector<shared_ptr<TH1>> spectra,
                                               vector<vector<double>> energies)
{
  TDirectory *calDir;
  if(logFile) calDir = logFile->mkdir("calibrationResults");
    
  vector<array<double,2>> result;
  for(int i=0; i<spectra.size(); i++){
    if(logFile){
      char dirName[64];
      sprintf(dirName,"channel_%d",i);
      calDir->cd();
      TDirectory *chanDir = calDir->mkdir(dirName);
      chanDir->cd();
    }
    shared_ptr<TH1> si = spectra.at(i);
    vector<double> ei = energies.at(i);
    result.push_back(Calibrate(si,ei));
  }
    
  if(logFile){
    calDir->cd();
    EvalAndLog(spectra,result);
  }
  return result; 
}

void Calibrator::EvalAndLog(vector<shared_ptr<TH1>> spectra,
                                                vector<array<double,2>> result)
{
  if(logFile){
    //Show all the gains and pedestals together.
    TGraph gGain, gPed;
    gGain.SetMarkerStyle(20);
    gPed.SetMarkerStyle(20);
    for(int i=0; i<result.size(); i++){
      gGain.SetPoint(i,i,result.at(i).at(0));
      gPed.SetPoint(i,i,result.at(i).at(1));
    }
    gGain.Write("gains");
    gPed.Write("pedestals");
    
    //Show the calibrated data in a 2D-histogram.
    //First, find the range.
    double Emax = -1;
    for(int i=0; i<spectra.size(); i++){
      int nBins = spectra.at(i)->GetNbinsX();
      double highBin = spectra.at(i)->GetBinCenter(nBins);
      double em = result.at(i).at(0) * (highBin - result.at(i).at(1));
      if(em > Emax) Emax = em;
    }
    //Create histogram.
    TH2I hist("channelComparison","",spectra.size(),-0.5,spectra.size()-0.5,1000,0,Emax);
    hist.SetOption("colz");
    for(int i=0; i<spectra.size(); i++){
      int nBins = spectra.at(i)->GetNbinsX();
      double gain = result.at(i).at(0);
      double pedestal = result.at(i).at(1);
      for(int j=1; j<nBins; j++){
        int content = Floor(spectra.at(i)->GetBinContent(j));
        for(int k=0; k<content; k++){
          TRandom3 rGen;
          double energy = gain * (j + rGen.Uniform(-0.5,0.5) - pedestal);
          hist.Fill(i,energy);
        }
      }
    }
    hist.Write();
  }
}

void Calibrator::SetPeakFinder(string type)
{
  peakFinder = shared_ptr<PeakFinder> (PeakFinder::Create(type));
}

void Calibrator::SetPeakFinder(shared_ptr<PeakFinder> peak_finder)
{
  peakFinder = peak_finder;
}

PeakFinder & Calibrator::GetPeakFinder()
{
  return *peakFinder;
}

void Calibrator::SetLogFile(string fileName)
{
  logFile = shared_ptr<TFile> (new TFile(fileName.c_str(),"RECREATE")); 
}
    
void Calibrator::SetLogFile(shared_ptr<TFile> log_file)
{
  logFile = log_file;
}

shared_ptr<TFile> Calibrator::GetLogFile()
{
  return logFile;
}
