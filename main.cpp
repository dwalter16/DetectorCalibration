#include <unistd.h>
#include <string>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <vector>
#include <array>
#include <fstream>
#include <TF1.h>
#include <TH1.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TVector3.h>
#include <TFile.h>
#include <TTree.h>
#include "Source.h"
#include "PeakFinder.h"
#include "Detector.h"
#include "Calibrator.h"
#include "CalibrationConfig.h"

using namespace std;

void PrintHelp(char *argv[]);

int main(int argc , char *argv[])
{
  //Parse the input.
  string configFile, logFile, outputFile, dataFile;
  int c;
  while((c = getopt(argc, argv,"c:o:l:h")) != -1){
    switch (c){
      case 'c':
        configFile = optarg;
        break;
      case 'l':
        logFile = optarg;
        break;
      case 'o':
        outputFile = optarg;
        break;
      case 'h':
        PrintHelp(argv);
        return 0;
      case '?':
        PrintHelp(argv);
        return 1;
      default:
        return 1;
    }
  }

  if(optind == argc-1){
    dataFile = argv[optind];
  }
  else{
    cout << "Exactly one data file must be provided." << endl;
    PrintHelp(argv);
    return 1;
  }

  //Then we load the configuration file and construct the necessary objects.
  if(configFile.empty()){
    cout << "No configuration file provided." << endl;
    PrintHelp(argv);
    return 1;
  }
  CalibrationConfig config;
  config.Parse(configFile);
  shared_ptr<Source> source = config.GetSource();
  shared_ptr<Detector> detector = config.GetDetector();
  shared_ptr<PeakFinder> peakFinder = config.GetPeakFinder();
  Calibrator calibrator;
  calibrator.SetPeakFinder(peakFinder);
  if(!logFile.empty()) calibrator.SetLogFile(logFile);
  
  //Here, a sophisticated calculation of the energy loss for each segment follows...
  int nChannels = detector->GetNSegments();
  vector<vector<double>> energies;
  for(int i=0; i<nChannels; i++){
    vector<double> energies_i;
    int nPeaks = source->GetNPeaks();
    for(int j=0; j<nPeaks; j++){
      energies_i.push_back(source->GetPeak(j));
    }
    energies.push_back(energies_i);
  }
  
  //Then we produce the histograms from the data file.
  TFile data(dataFile.c_str(),"READ");
  TTree *tree = (TTree*)data.Get("Iris");
  TH2I hist("hist","",nChannels,-0.5,nChannels-0.5,4096,-0.5,4095.5);
  string drawCmd = detector->GetEnergyBranch() + ":" + detector->GetChannelBranch() + ">>hist";
  tree->Draw(drawCmd.c_str());
  hist.SetDirectory(0);
  data.Close();
  vector<shared_ptr<TH1>> spectra;
  for(int i=1; i<=nChannels; i++){
    string histName = "channel_" + to_string(i-1);
    shared_ptr<TH1> channelSpectrum =
         shared_ptr<TH1>((TH1*)hist.ProjectionY(histName.c_str(),i,i)->Clone());
    spectra.push_back(channelSpectrum);
  }
  
  //Then we are ready to run the calibration routine.
  vector<array<double,2>> result = calibrator.Calibrate(spectra,energies);
  if(!outputFile.empty()){
    ofstream output(outputFile);
    output << "ch  offset  gain" << endl;
    for(int i=0; i<nChannels; i++){
      output << i << "  " << result.at(i).at(1) << "  " << result.at(i).at(0) << endl;
    }
    output.close();
  }
    
  
  /*
  TF1 f("f","TMath::Gaus(x,1000,10)+TMath::Gaus(x,2000,15)",0,4096);
  vector<shared_ptr<TH1>> spectra;
  vector<vector<double>> energies;
  for(int i=0; i<10; i++){
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
  */
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

void PrintHelp(char *argv[])
{
  cout << "Usage: " << argv[0] << " [OPTIONS] ... [DATAFILE]" << endl;
  cout << "Detector energy calibration based on the data in [DATAFILE]." << endl;
  cout << endl;
  cout << "  -c [FILE]      Load configuration from [FILE] (required)." << endl;
  cout << "  -h             Print this message." << endl;
  cout << "  -l [FILE]      Store logging information in [FILE] (optional)." << endl;
  cout << "  -o [FILE]      Store calibration coefficients in [FILE] (optional)." << endl;
  cout << endl;
  cout << "Example:" << endl;
  cout << argv[0] << " -c /config/file -o /output/file /data/file" << endl;
}
