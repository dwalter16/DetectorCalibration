#include "CalibrationConfig.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void CalibrationConfig::Parse(string fileName)
{
  string line;
  ifstream configFile(fileName);
  while(getline(configFile,line)){
    if(line == "DETECTOR"){
      detector = shared_ptr<Detector>(BuildDetector(configFile));
    }
    else if(line == "ALGORITHM"){
      peakFinder = shared_ptr<PeakFinder>(BuildPeakFinder(configFile));
    }
    else if(line == "SOURCE"){
      source = shared_ptr<Source>(BuildSource(configFile));
    }
    else{
      cout << "CalibrationConfig::Parse(): Unknown option: " ;
      cout << "\"" << line << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }   
}

Detector * CalibrationConfig::BuildDetector(ifstream &configFile)
{
  string line, option;
  //First, we make sure the first option specifies a detector type. Then we construct it.
  getline(configFile,line);
  option = GetOption(line);
  if(option != "TYPE"){
    cout << "CalibrationConfig::BuildDetector(): The first option must specify a \"TYPE\"." ;
    cout << endl;
    exit(EXIT_FAILURE);
  }
  Detector *d = Detector::Create(GetValue(line));
  
  //Next we set the detector properties according to the input.
  while(getline(configFile,line)){
    option = GetOption(line);
    if(option.empty()) break; //An empty line means the configuration is complete.
    else if(option == "POSX") d->SetX(stod(GetValue(line)));
    else if(option == "POSY") d->SetY(stod(GetValue(line)));
    else if(option == "POSZ") d->SetZ(stod(GetValue(line)));
    else if(option == "ROTX") d->RotateX(stod(GetValue(line)));
    else if(option == "ROTY") d->RotateY(stod(GetValue(line)));
    else if(option == "ROTZ") d->RotateZ(stod(GetValue(line)));
    else if(option == "DEADLAYER") d->SetDeadLayer(stod(GetValue(line)));
    else if(option == "ENERGY") d->SetEnergyBranch(GetValue(line));
    else if(option == "CHANNEL") d->SetChannelBranch(GetValue(line));
    else{
      cout << "CalibrationConfig::BuildDetector(): Unknown option: \"" ;
      cout << option << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return d;
}
    
PeakFinder * CalibrationConfig::BuildPeakFinder(ifstream &configFile)
{
  string line, option;
  //First, we make sure the first option specifies an algorithm type. Then we construct it.
  getline(configFile,line);
  option = GetOption(line);
  if(option != "TYPE"){
    cout << "CalibrationConfig::BuildPeakFinder(): The first option must specify a \"TYPE\"." ;
    cout << endl;
    exit(EXIT_FAILURE);
  }
  PeakFinder *p = PeakFinder::Create(GetValue(line));
  
  //Next we configure the peak finder according to the input.
  while(getline(configFile,line)){
    option = GetOption(line);
    if(option.empty()) break; //An empty line means the configuration is complete.
    else if(option == "SIGMA") p->SetSigma(stod(GetValue(line)));
    else if(option == "LOWTHRES") p->SetLowThreshold(stod(GetValue(line)));
    else if(option == "HIGHTHRES") p->SetHighThreshold(stod(GetValue(line)));
    else if(option == "MINHEIGHT") p->SetMinHeight(stod(GetValue(line)));
    else{
      cout << "CalibrationConfig::BuildPeakFinder(): Unknown option: \"" ;
      cout << option << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return p;
}

Source * CalibrationConfig::BuildSource(ifstream &configFile)
{
  string line, option;
  //First, we make sure the first option specifies a particle type. Then we construct it.
  getline(configFile,line);
  option = GetOption(line);
  if(option != "TYPE"){
    cout << "CalibrationConfig::BuildSource(): The first option must specify a \"TYPE\"." ;
    cout << endl;
    exit(EXIT_FAILURE);
  }
  Source *s = new Source();
  s->SetParticle(GetValue(line));
   
  //Next we configure the source according to the input.
  while(getline(configFile,line)){
    option = GetOption(line);
    if(option.empty()) break; //An empty line means the configuration is complete.
    else if(option == "POSX") s->SetX(stod(GetValue(line)));
    else if(option == "POSY") s->SetY(stod(GetValue(line)));
    else if(option == "POSZ") s->SetZ(stod(GetValue(line)));
    else if(option == "PEAK") s->AddPeak(stod(GetValue(line)));
    else{
      cout << "CalibrationConfig::BuildSource(): Unknown option: \"" ;
      cout << option << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return s;
}

string CalibrationConfig::GetOption(string &line)
{
  size_t pos = line.find_first_not_of(" ");
  if(pos != 2){
    cout << "CalibrationConfig()::GetOption(): This line does not specify an option: \"";
    cout << line << "\"" << endl;
    return "";
  }
  else{
    return line.substr(pos,line.find_first_of(" ",pos)-pos);
  }
}

string CalibrationConfig::GetValue(string &line)
{
  return line.substr(line.find_last_of(" ")+1,line.length());
}
    
shared_ptr<Detector> CalibrationConfig::GetDetector()
{
  return detector;
}

shared_ptr<PeakFinder> CalibrationConfig::GetPeakFinder()
{
  return peakFinder;
}