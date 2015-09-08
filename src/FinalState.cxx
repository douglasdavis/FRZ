/** @file FinalState.cxx
 *  @brief FRZ::FinalState class implementation
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

// FRZ
#include "FRZ/FinalState.h"
#include "FRZ/MET.h"

FRZ::FinalState::FinalState() :
  m_leptons(),
  m_jets(),
  m_leptonPairs(),
  m_MET(),
  m_nZcandidates(0),
  m_Ht(-9e9),
  m_ZcandidateIdx(10),
  m_thirdLeptonIdx(10)
{
  m_leptons.reserve(3);
  m_leptonPairs.reserve(3);
}

FRZ::FinalState::~FinalState() {}

void FRZ::FinalState::evaluateZcandidates() {
  m_nZcandidates = 0;
  for ( auto const& lp : m_leptonPairs )
    if ( lp.obj().Zcandidate() )
      ++m_nZcandidates;

  if ( m_nZcandidates == 1 ) {
    for ( unsigned int i = 0; i < 3; ++i ) {
      if ( m_leptonPairs.at(i).obj().Zcandidate() ) {
	m_ZcandidateIdx  = i;
	m_thirdLeptonIdx = m_leptonPairs.at(i).obj().thirdLeptonIdx();
	break;
      }
    }
  }
}

void FRZ::FinalState::evaluateSelf() {
  m_Ht = 0;
  for ( auto const& lep : m_leptons )
    m_Ht += lep.pT();
  for ( auto const& jet : m_jets )
    m_Ht += jet.pT();

  m_thirdLoose = false; m_pairLoose = false; m_allLoose = false;
  
  if ( m_leptons.at(m_thirdLeptonIdx).obj().isLoose() ) m_thirdLoose = true;
  if ( m_leptons.at(m_leptonPairs.at(m_ZcandidateIdx).obj().lepton1idx()).obj().isLoose() &&
       m_leptons.at(m_leptonPairs.at(m_ZcandidateIdx).obj().lepton2idx()).obj().isLoose() ) m_pairLoose = true;
  if ( m_thirdLoose && m_pairLoose ) m_allLoose = true;  

  double current_dR = 2000.0;
  double min_dR     = 1000.0;
  for ( auto& lep : m_leptons ) {
    min_dR = 1000.0;
    if ( m_jets.empty() )
      lep.obj().set_dR_closestJet(-1.0);
    else {
      for ( auto const& jet : m_jets ) {
	current_dR = lep.p().DeltaR(jet.p());
	if ( current_dR < min_dR )
	  min_dR = current_dR;
      }
      lep.obj().set_dR_closestJet(min_dR);
    }
    lep.obj().set_dphi_MET(lep.p().DeltaPhi(m_MET.p()));
  }
      
}
