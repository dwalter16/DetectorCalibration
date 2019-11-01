#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
#include "Detector.h"
#include "PeakFinder.h"
#include "Source.h"
#include "Options.h"
#include <memory>
#include <string>
#include <fstream>

class ConfigParser {
  private:
    std::shared_ptr<Detector> detector;
    std::shared_ptr<PeakFinder> peakFinder;
    std::shared_ptr<Source> source;
    std::shared_ptr<Options> options;
    
    std::string GetOption(std::string &line);
    
    std::string GetValue(std::string &line);

    Detector * BuildDetector(std::ifstream &configFile);
    
    PeakFinder * BuildPeakFinder(std::ifstream &configFile);
    
    Source * BuildSource(std::ifstream &configFile);
    
    Options * BuildOptions(std::ifstream &configFile);
    
  public:
    ConfigParser(std::string fileName = "");
    ~ConfigParser() = default;
    
    void Parse(std::string fileName);
    
    void PrintConfiguration();
    
    std::shared_ptr<Detector> GetDetector();
    
    std::shared_ptr<PeakFinder> GetPeakFinder();
    
    std::shared_ptr<Source> GetSource();
    
    std::shared_ptr<Options> GetOptions();  
};
#endif