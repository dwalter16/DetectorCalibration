#ifndef CALIBRATION_CONFIG_H
#define CALIBRATION_CONFIG_H
#include "Detector.h"
#include "PeakFinder.h"
#include "Source.h"
#include <memory>
#include <string>
#include <fstream>

class CalibrationConfig {
  private:
    std::shared_ptr<Detector> detector;
    std::shared_ptr<PeakFinder> peakFinder;
    std::shared_ptr<Source> source;
    
    std::string GetOption(std::string &line);
    
    std::string GetValue(std::string &line);

    Detector * BuildDetector(std::ifstream &configFile);
    
    PeakFinder * BuildPeakFinder(std::ifstream &configFile);
    
    Source * BuildSource(std::ifstream &configFile);
    
  public:
    CalibrationConfig() = default;
    ~CalibrationConfig() = default;
    
    void Parse(std::string fileName);
    
    std::shared_ptr<Detector> GetDetector();
    
    std::shared_ptr<PeakFinder> GetPeakFinder();
    
};
#endif
