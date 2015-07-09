/** @file Lepton.cxx
 *  @brief FRZ::Lepton class implementation
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

// FRZ
#include "FRZ/Lepton.h"

FRZ::Lepton::Lepton() :
  m_pdgId(0),
  m_charge(-9e9),
  m_d0(-9e9),
  m_z0(-9e9),
  m_d0cut(-9e9),
  m_z0cut(-9e9),
  m_dphi_MET(-9e9),
  m_dR_closestJet(-9e9),
  m_truthMatched(false),
  m_isLoose(false),
  m_trigger(false),
  m_triggerBit(0),
  m_trackPt(-9e9)
{}

FRZ::Lepton::~Lepton() {}
