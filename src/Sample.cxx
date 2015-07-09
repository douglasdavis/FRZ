/** @file Sample.cxx
 *  @brief FRZ::Sample class implementation
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

// C++
#include <iostream>

// FRZ
#include "FRZ/Sample.h"

FRZ::Sample::Sample() :
  m_processType(""),
  m_processGenerator(""),
  m_mc_channel(0),
  m_ID(0),
  m_crossSection(0),
  m_kFactor(0),
  m_nGenerated(0),
  m_nGeneratedPileup(0),
  m_nFiducial_elel(0),
  m_nFiducial_elmu(0),
  m_nFiducial_mumu(0),
  m_nGenerated_elel(0),
  m_nGenerated_elmu(0),
  m_nGenerated_mumu(0),
  m_nReconstructed_elel(0),
  m_nReconstructed_elmu(0),
  m_nReconstructed_mumu(0),
  m_BR_elel(0),
  m_BR_elmu(0),
  m_BR_mumu(0)
{}

FRZ::Sample::~Sample() {}

void FRZ::Sample::dumpVars() const
{
  std::cout << m_processType
	    << " " << m_processGenerator
	    << " " << m_mc_channel
	    << " " << m_ID
	    << " " << m_crossSection
	    << " " << m_kFactor
	    << " " << m_nGenerated
	    << " " << m_nGeneratedPileup
	    << " " << m_nFiducial_elel
	    << " " << m_nFiducial_elmu
	    << " " << m_nFiducial_mumu
	    << " " << m_nGenerated_elel
	    << " " << m_nGenerated_elmu
	    << " " << m_nGenerated_mumu
	    << " " << m_nReconstructed_elel
	    << " " << m_nReconstructed_elmu
	    << " " << m_nReconstructed_mumu
	    << " " << m_BR_elel
	    << " " << m_BR_elmu
	    << " " << m_BR_mumu
	    << std::endl;
}
