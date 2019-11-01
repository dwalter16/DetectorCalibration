#ifndef OPTIONS_H
#define OPTIONS_H
#include <string>

struct Options {
  int nBins;    /**< Number of bins in the signal spectra.*/
  double min;   /**< Minimum of signal range to be considered.*/
  double max;   /**< Maximum of signal range to be considered.*/
  int offset;   /**< The channel numbers in the output file is offset by this amount.*/
  std::string tree;  /**< Name of the ROOT tree containing the data.*/
  std::string energyBranch;  /**< Name of the branch containing the energy signals.*/
  std::string channelBranch; /**< Name of the branch containing the channel indices.*/
  
  Options(): nBins(4096),
             min(-0.5),
             max(4095.5),
             offset(0),
             tree("Iris"),
             energyBranch(""),
             channelBranch("") {}
};
#endif
