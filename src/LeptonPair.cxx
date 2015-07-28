/** @file LeptonPair.cxx
 *  @brief FRZ::LeptonPair class implementation
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

// C++
#include <iostream>

// FRZ
#include "FRZ/Lepton.h"
#include "FRZ/LeptonPair.h"
#include "FRZ/Common.h"

FRZ::LeptonPair::LeptonPair() :
  m_mass(-9e9),
  m_lepton1idx(10),
  m_lepton2idx(10),
  m_mumu(false),
  m_elel(false),
  m_OF(false),
  m_SS(false),
  m_OS(false),
  m_Zcandidate(false),
  m_thirdLeptonIdx(10)
{}

FRZ::LeptonPair::~LeptonPair() {}

void FRZ::LeptonPair::setLeptons(const FRZ::PhysicsObject<FRZ::Lepton> lep1,
				 const FRZ::PhysicsObject<FRZ::Lepton> lep2,
				 const unsigned int idx1,
				 const unsigned int idx2,
				 const unsigned int idx3)
{
  m_mass         = (lep1.p() + lep2.p()).M();
  m_lepton1idx   = idx1;
  m_lepton2idx   = idx2;

  int pdg_id_sum = lep1.obj().pdgId()  + lep2.obj().pdgId();
  int charge_sum = lep1.obj().charge() + lep2.obj().charge();

  switch ( charge_sum ) {
  case -2:
    m_SS = true;
    break;
  case 0:
    m_OS = true;
    break;
  case 2:
    m_SS = true;
    break;
  default:
    std::cout << "bad charge_sum in LeptonPair::set_Leptons" << std::endl;
    break;
  }

  switch ( pdg_id_sum ) {
  case 22:
    m_elel = true;
    m_mumu = false;
    m_OF   = false;
    break;
  case 26:
    m_mumu = true;
    m_elel = false;
    m_OF   = false;
    break;
  case 24:
    m_OF   = true;
    m_mumu = false;
    m_elel = false;
    break;
  default:
    std::cout << "bad pdg_id_sum in LeptonPair::set_Leptons" << std::endl;
    break;
  }

  if ( m_mass > Z_MINUS_15 && m_mass < Z_PLUS_15 ) {
    if ( m_elel || m_mumu ) {
      if ( m_OS ) {
	m_Zcandidate = true;
      }
    }
  }

  m_thirdLeptonIdx = idx3; 
}
