#ifndef TSPECTRUM_FINDER_H
#define TSPECTRUM_FINDER_H
#include "PeakFinder.h"
#include <string>
#include <vector>
#include <array>
#include <TSpectrum.h>

/**
 * Implementation of a peak finder using the TSpectrum class from ROOT.
 */
class TSpectrumFinder : public PeakFinder {
  private:
    TSpectrum tSpectrum;
    
  public:
    TSpectrumFinder() = default;
    ~TSpectrumFinder() = default;

    virtual std::vector<std::array<double,2>> 
                              Search(std::shared_ptr<TH1> spectrum, int nPeaks);
    
    /**
     * This method allows you to run the TSpectrum search algorithm with one of
     * the standard options. See the TSpectrum documentation.
     */     
    std::vector<std::array<double,2>> 
          Search(std::shared_ptr<TH1> spectrum, int nPeaks, std::string option);

    /**
     * This method makes it possible for the user to directly interact with the
     * instance of TSpectrum owned by this class.
     */            
    TSpectrum & GetTSpectrum();
    
    /**
     * Set a threshold on the amplitude of the peaks returned by the algorithm.
     * @param threshold The algorithm excludes peaks with height less than
     *                  threshold * (amplitude of highest peak). Default value
     *                  is 0.1.
     */     
    void SetThreshold(double thres);

    double GetThreshold();
};
#endif
