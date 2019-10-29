#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include "PeakFinder.h"
#include <memory>
#include <array>
#include <string>
#include <vector>
#include <TH1.h>
#include <TFile.h>


/**
 * This class provides methods to find the calibration coefficients from the
 * given calibration data, source information and, optionally, pedestal.
 */
class Calibrator {
  private:
    std::shared_ptr<TFile> logFile;
    std::shared_ptr<PeakFinder> peakFinder;
    
    std::array<double,2> Calibrate(std::shared_ptr<TH1> spectrum, TF1 &func,
                                                  std::vector<double> energies);
                                                  
    void EvalAndLog(std::vector<std::shared_ptr<TH1>> spectra,
                                      std::vector<std::array<double,2>> result);
    
  public:
    /**
     * Default constructor.
     * @param algorithm Specify which peak finding algorithm to use. Available
     *                  options can be found in the documentation for the
     *                  PeakFinder-class.
     */
    Calibrator(std::string algorithm = "TSpectrum");
    ~Calibrator() = default;
    
    /**
     * Calibrate a single spectrum. Providing a pedestal is optional.
     * @param spectrum A pointer to the histogram containing the calibration
     *                 data.
     * @param energies Array of peak energies to use.
     * @param pedestal Pedestal or zero-energy channel.
     * @return An array containing the two calibration parameters, a and b, such
     *         that the energy calibration is obtained from E = a*(x-b);
     */
    std::array<double,2> Calibrate(std::shared_ptr<TH1> spectrum,
                                 std::vector<double> energies, double pedestal);
                                 
    std::array<double,2> Calibrate(std::shared_ptr<TH1> spectrum,
                                                  std::vector<double> energies);

    /**
     * Calibrate a collection of spectra in a single function call.
     */                                 
    std::vector<std::array<double,2>> Calibrate
                       (std::vector<std::shared_ptr<TH1>> spectra,
                        std::vector<std::vector<double>> energies,
                        std::vector<double> pedestals);

    std::vector<std::array<double,2>> Calibrate
                       (std::vector<std::shared_ptr<TH1>> spectra,
                        std::vector<std::vector<double>> energies);
                        
    /**
     * Here you can change the peak finding method after you construction
     * the calibrator.
     */
    void SetPeakFinder(std::string type);
    void SetPeakFinder(std::shared_ptr<PeakFinder> peak_finder);
    PeakFinder & GetPeakFinder();

    /**
     * Tell the calibrator to store some logging information in a ROOT file.
     * This function creates a new TFile instance, which can later be accessed
     * through the GetLogFile-method.
     * @param fileName Name of the output file.
     */
    void SetLogFile(std::string fileName);
    
    /**
     * Tell the calibrator to store logging information in an existing instance
     * af a TFile.
     */     
    void SetLogFile(std::shared_ptr<TFile> log_file);

    /**
     * Obtain a pointer to the calibrator's log file. By default the log file
     * is not automatically created, so this function might return a null
     * pointer.
     * @return Shared pointer to the log file.
     */     
    std::shared_ptr<TFile> GetLogFile();
};
#endif
