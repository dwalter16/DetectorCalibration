#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <vector>
#include <array>
#include <fstream>
#include <TH1.h>
#include <TH2I.h>
#include <TFile.h>
#include <TTree.h>
#include "Options.h"
#include "Source.h"
#include "PeakFinder.h"
#include "Detector.h"
#include "Calibrator.h"
#include "Options.h"
#include "ConfigParser.h"

using namespace std;

void PrintHelp(char *argv[]);

int main(int argc , char *argv[])
{
  //Parse the input.
  string configFile, logFile, outputFile, dataFile, pedFile;
  int c;
  while((c = getopt(argc, argv,"c:o:l:hp:")) != -1){
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
      case 'p':
        pedFile = optarg;
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
  ConfigParser config(configFile);
  shared_ptr<Source> source = config.GetSource();
  shared_ptr<Detector> detector = config.GetDetector();
  shared_ptr<PeakFinder> peakFinder = config.GetPeakFinder();
  shared_ptr<Options> options = config.GetOptions();
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
  TTree *tree = (TTree*)data.Get(options->tree.c_str());
  TH2I hist("hist","",nChannels,-0.5,nChannels-0.5,options->nBins,options->min,options->max);
  string drawCmd = options->energyBranch + ":" + options->channelBranch + ">>hist";
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
  
  //If pedestal file is provided, load...
  vector<double> pedestals;
  if(!pedFile.empty()){
    ifstream pedestalFile(pedFile);
    string line;
    getline(pedestalFile,line); //First line is for comments.
    while(getline(pedestalFile,line)){
      stringstream ss(line);
      int channel; double pedestal;
      ss >> channel >> pedestal ;
      pedestals.push_back(pedestal);
    }
  }
  if(pedestals.size() != nChannels){
    cout << "Pedestal file does not contain the expected number of values.";
    return 1;
  }
    
  //Then we are ready to run the calibration routine.
  vector<array<double,2>> result;
  if(!pedestals.empty()) result = calibrator.Calibrate(spectra,energies,pedestals);
  else result = calibrator.Calibrate(spectra,energies);
  
  //..and store the output in a plain text file.
  if(!outputFile.empty()){
    ofstream output(outputFile);
    output << "ch  offset  gain" << endl;
    for(int i=0; i<nChannels; i++){
      int channel = i + options->offset;
      output << channel << "  " << result.at(i).at(1) << "  " << result.at(i).at(0) << endl;
    }
    output.close();
  }
    
  return 0;
}

void PrintHelp(char *argv[])
{
  cout << "Usage: " << argv[0] << " [OPTIONS] ... [DATAFILE]" << endl;
  cout << "Detector energy calibration based on the data in [DATAFILE]." << endl;
  cout << "Available options:" << endl;
  cout << endl;
  cout << "  -c [FILE]      Load configuration from [FILE] (required)." << endl;
  cout << "  -h             Print this message." << endl;
  cout << "  -l [FILE]      Store logging information in [FILE] (optional)." << endl;
  cout << "  -o [FILE]      Store calibration coefficients in [FILE] (optional)." << endl;
  cout << endl;
  cout << "Example:" << endl;
  cout << argv[0] << " -c /config/file -o /output/file /data/file" << endl;
}
