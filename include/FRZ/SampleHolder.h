/** @file  SampleHolder.h
 *  @brief FRZ::SampleHolder class header
 *  @class FRZ::SampleHolder
 *  @brief holds map of Samples and access functions
 *
 *  This class holds an std::map
 *  of MC Samples for easy access to them
 *  along with functions to deal with them
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_SAMPLEHOLDER_H
#define FRZ_SAMPLEHOLDER_H

// C++
#include <map>
#include <iostream>

// FRZ
#include "FRZ/Sample.h"

namespace FRZ {

  class SampleHolder {

  private:

    std::map<int,FRZ::Sample> m_samples;

  public:

    /// main constructor
    SampleHolder();

    /// virtual destructor
    virtual ~SampleHolder();

    /// add a new entry in the samples map for a new Sample
    void addSample(const int id, const FRZ::Sample& samp) {
      m_samples[id] = samp;
    }

    /// prints all the Sample info
    void dumpSamples() const {
      for ( auto const& samp : m_samples ) {
	std::cout << samp.first << " ... ";
	samp.second.dumpVars();
      }
    }

    const FRZ::Sample& getSample(const int i) const { return m_samples.at(i); }

  };

}

#endif
