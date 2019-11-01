#include "ConfigParser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

ConfigParser::ConfigParser(string fileName)
{
  if(!fileName.empty()) Parse(fileName);
}

void ConfigParser::Parse(string fileName)
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
    else if(line == "OPTIONS"){
      options = shared_ptr<Options>(BuildOptions(configFile));
    }
    else{
      cout << "ConfigParser::Parse(): Unknown option: " ;
      cout << "\"" << line << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }   
}

Detector * ConfigParser::BuildDetector(ifstream &configFile)
{
  string line, option;
  //First, we make sure the first option specifies a detector type. Then we construct it.
  getline(configFile,line);
  option = GetOption(line);
  if(option != "TYPE"){
    cout << "ConfigParser::BuildDetector(): The first option must specify a \"TYPE\"." ;
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
    else{
      cout << "ConfigParser::BuildDetector(): Unknown option: \"" ;
      cout << option << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return d;
}
    
PeakFinder * ConfigParser::BuildPeakFinder(ifstream &configFile)
{
  string line, option;
  //First, we make sure the first option specifies an algorithm type. Then we construct it.
  getline(configFile,line);
  option = GetOption(line);
  if(option != "TYPE"){
    cout << "ConfigParser::BuildPeakFinder(): The first option must specify a \"TYPE\"." ;
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
      cout << "ConfigParser::BuildPeakFinder(): Unknown option: \"" ;
      cout << option << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return p;
}

Source * ConfigParser::BuildSource(ifstream &configFile)
{
  string line, option;
  //First, we make sure the first option specifies a particle type. Then we construct it.
  getline(configFile,line);
  option = GetOption(line);
  if(option != "TYPE"){
    cout << "ConfigParser::BuildSource(): The first option must specify a \"TYPE\"." ;
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
      cout << "ConfigParser::BuildSource(): Unknown option: \"" ;
      cout << option << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return s;
}

Options * ConfigParser::BuildOptions(ifstream &configFile)
{
  string line, option;
  Options *o = new Options();
  while(getline(configFile,line)){
    option = GetOption(line);
    if(option.empty()) break; //An empty line means the configuration is complete.
    else if(option == "NBINS") o->nBins = stoi(GetValue(line));
    else if(option == "MIN") o->min = stod(GetValue(line));
    else if(option == "MAX") o->max = stod(GetValue(line));
    else if(option == "TREE") o->tree = GetValue(line);
    else if(option == "ENERGY") o->energyBranch = GetValue(line);
    else if(option == "CHANNEL") o->channelBranch = GetValue(line);
    else if(option == "OFFSET") o->offset = stoi(GetValue(line));
    else{
      cout << "ConfigParser::BuildOptions(): Unknown option: \"" ;
      cout << option << "\"" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return o;
}

string ConfigParser::GetOption(string &line)
{
  size_t pos = line.find_first_not_of(" ");
  if(pos != 2){
    //cout << "ConfigParser()::GetOption(): This line does not specify an option: \"";
    //cout << line << "\"" << endl;
    return "";
  }
  else{
    return line.substr(pos,line.find_first_of(" ",pos)-pos);
  }
}

string ConfigParser::GetValue(string &line)
{
  return line.substr(line.find_last_of(" ")+1,line.length());
}
    
shared_ptr<Detector> ConfigParser::GetDetector()
{
  return detector;
}

shared_ptr<PeakFinder> ConfigParser::GetPeakFinder()
{
  return peakFinder;
}

shared_ptr<Source> ConfigParser::GetSource()
{
  return source;
}

shared_ptr<Options> ConfigParser::GetOptions()
{
  return options;
}

void ConfigParser::PrintConfiguration()
{
  cout << "Calibration configuration:" << endl;
  cout << "Detector:" << endl ;
  cout << "  position = (" ;
  cout << detector->GetPosition().x() << "," ;
  cout << detector->GetPosition().y() << "," ;
  cout << detector->GetPosition().z() << ")" << endl;
  cout << "  normal = (" ;
  cout << detector->GetNormal().x() << "," ;
  cout << detector->GetNormal().y() << "," ;
  cout << detector->GetNormal().z() << ")" << endl;
  cout << "  orientation = (" ;
  cout << detector->GetOrientation().x() << "," ;
  cout << detector->GetOrientation().y() << "," ;
  cout << detector->GetOrientation().z() << ")" << endl;
  
  cout << "Source:" << endl;
  cout << "  type = " << source->GetParticle() << endl;
  cout << "  position = (" ;
  cout << source->GetPosition().x() << "," ;
  cout << source->GetPosition().y() << "," ;
  cout << source->GetPosition().z() << ")" << endl;
  for(int i=0; i<source->GetNPeaks(); i++){
    cout << "  peak " << i+1 << " = " << source->GetPeak(i) << endl;
  }
  
  cout << "Algorithm:" << endl;
  cout << "  sigma = " << peakFinder->GetSigma() << endl;
  cout << "  min. threshold = " << peakFinder->GetLowThreshold() << endl;
  cout << "  max. threshold = " << peakFinder->GetHighThreshold() << endl;
  cout << "  min. height = " << peakFinder->GetMinHeight() << endl;
  
  cout << "Options:" << endl;
  cout << "  tree = " << options->tree << endl;
  cout << "  energy = " << options->energyBranch << endl;
  cout << "  channel = " << options->channelBranch << endl;
  cout << "  offset = " << options->offset << endl;
  cout << "  no. bins = " << options->nBins << endl;
  cout << "  range min. = " << options->min << endl;
  cout << "  range max. = " << options->max << endl;
}
