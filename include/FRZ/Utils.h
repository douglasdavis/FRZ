/** @file  Utils.h
 *  @brief FRZ::Utils header file
 *
 *  Some utility functions
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_UTILS_H
#define FRZ_UTILS_H

// C++
#include <algorithm>
#include <string>

// FRZ
#include "FRZ/Common.h"
#include "FRZ/Sample.h"

namespace FRZ {

  float getLuminosityData() {
    return LUM_DATA;
  }

  // This function returns a weight which corresponds to a calculation
  // described in the guide written by Dave B. here:
  // http://www.phy.duke.edu/~dmb60/the-guide/#sec-1-10-3
  // essentially it returns LUM_DATA divided by the MC luminosity
  float getWeight(const FRZ::Sample& samp, const bool pileup)
  {
    float nGen;
    float luminosity = LUM_DATA;
    if ( pileup )
      nGen = samp.nGeneratedPileup();
    else
      nGen = samp.nGenerated();
  
    return samp.crossSection()*samp.kFactor()*luminosity/nGen;
  }

  void notifyProgress(int current, int first, int last, std::string message) {
    int total = last - first + 1;
    float progress = ((float) 100 * (current - first)) / total;
    int notificationGap = std::max(1, (int) total / 10);
    if (current % notificationGap == 0) {
      printf((message + '\n').c_str(), progress, current, total);
    }
  }

}

#endif
