#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
#include "Detector.h"
#include "PeakFinder.h"
#include "Source.h"
#include "Options.h"
#include <memory>
#include <string>
#include <fstream>

/**
 * A class that opens and parses the configuration file provided to the
 * calibration program. Based on the result it constructs the various objects
 * needed to perform the calibration. The format of the configuration file is
 * described elsewhere. If any of the 'getters' are called before Parse() has
 * been called, they will return null pointers.
 */ 
class ConfigParser {
  private:
    std::shared_ptr<Detector> detector;
    std::shared_ptr<PeakFinder> peakFinder;
    std::shared_ptr<Source> source;
    std::shared_ptr<Options> options;
    
    /**
     * Get the 'option' part of the given string. First it is checked if the
     * string begins with two spaces. The first word following the spaces is
     * interpreted as the 'option'. If the string doesn't match this format an
     * empty string is returned.
     * @param line A text string to be parsed.
     */
    std::string GetOption(std::string &line);
    
    /**
     * Get the 'value' part of the given string. No matter how the string is
     * formatted this is understood to be the last word of the string.
     * @param line A text string to be parsed.
     */
    std::string GetValue(std::string &line);

    Detector * BuildDetector(std::ifstream &configFile);
    
    PeakFinder * BuildPeakFinder(std::ifstream &configFile);
    
    Source * BuildSource(std::ifstream &configFile);
    
    Options * BuildOptions(std::ifstream &configFile);
    
  public:
    /**
     * Default constructor.
     * @param fileName It is optional to pass the configuration file name to the
     *                 constructor. If no argument is given, the Parse() method
     *                 must be called before calling the 'getters'.
     */     
    ConfigParser(std::string fileName = "");
    ~ConfigParser() = default;
    
    /**
     * Parse the configuration file for this calibration.
     * Below is an example of a configuration file:
     * \include [] configExample.txt
     * Each of the four classes is specified with non-indented capital letters.
     * The following options must be indented by exactly two spaces, and the
     * end of each class is signalled by an empty line. The first option for the
     * DETECTOR, SOURCE and ALGORITHM classes must be the TYPE option.
     * @param fileName Path to the configuration file.
     */
    void Parse(std::string fileName);
    
    /**
     * Print the current configuration. Useful to check if the config-file was
     * interpreted correctly.
     */
    void PrintConfiguration();
    
    /**
     * Get a pointer to the instance of the Detector class which was constructed
     * by the Parse() method.
     */     
    std::shared_ptr<Detector> GetDetector();

    /**
     * Get a pointer to the instance of the PeakFinder class which was 
     * constructed by the Parse() method.
     */     
    std::shared_ptr<PeakFinder> GetPeakFinder();

    /**
     * Get a pointer to the instance of the Source class which was constructed
     * by the Parse() method.
     */     
    std::shared_ptr<Source> GetSource();

    /**
     * Get a pointer to the instance of the Options class which was constructed
     * by the Parse() method.
     */     
    std::shared_ptr<Options> GetOptions();  
};
#endif
