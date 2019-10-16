#ifndef PEAK_FINDER_H
#define PEAK_FINDER_H
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <TH1.h>

/**
 * Abstract base-class for peak finding classes. It defines the interface and
 * provides a factory method 'Create' to create instances of the various child
 * classes.
 */
class PeakFinder {
  protected:
    double sigma; /**< The expected width of the peaks to be searched for.*/
    
  public:
    PeakFinder() {sigma = 3.;};
    virtual ~PeakFinder() = default;

    /**
     * Factory method for instantiating peak finders.
     * @param type Specifies the peak finding algorithm. Valid algorithms are:
     *               "TSpectrum".
     * @return A pointer to the newly created instance.
     */     
    static PeakFinder * Create(std::string type);
    
    /**
     * This function searches the provided spectrum for peaks.
     * @param spectrum Contains the spectrum to be analysed.
     * @param nPeaks Specifies the number of peaks to be found.
     * @return A vector containing the results of the peak search. Each entry 
     *         is an array with the first entry holding the peak position and
     *         the second entry holding the uncertainty on the position. The
     *         peaks are ordered from lowest to highest position.
     */
    virtual std::vector<std::array<double,2>> 
                          Search(std::shared_ptr<TH1> spectrum, int nPeaks) = 0;
            
    /**
     * Set the approximate width of the peaks to be searched for.
     * @param sig The expected width in no. of bins.
     */
    void SetSigma(double sig);

    double GetSigma();
};
#endif
