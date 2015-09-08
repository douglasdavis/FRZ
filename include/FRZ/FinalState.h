/** @file  FinalState.h
 *  @brief FRZ::FinalState class header
 *  @class FRZ::FinalState
 *  @brief Container of FinalState information
 *
 *  This class holds information about
 *  each event's final state
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_FINALSTATE_H
#define FRZ_FINALSTATE_H

// C++
#include <vector>

// FRZ
#include "FRZ/Lepton.h"
#include "FRZ/Jet.h"
#include "FRZ/MET.h"
#include "FRZ/LeptonPair.h"

namespace FRZ {

  class FinalState {

  private:

    std::vector<FRZ::PhysicsObject<FRZ::Lepton> >     m_leptons;         //< Lepton container
    std::vector<FRZ::PhysicsObject<FRZ::Jet> >        m_jets;            //< Jet container
    std::vector<FRZ::PhysicsObject<FRZ::LeptonPair> > m_leptonPairs;     //< Lepton pair conatiner
    FRZ::PhysicsObject<FRZ::MET>                      m_MET;             //< missing tranvserse energy object
    int                                               m_nZcandidates;    //< total number of z candidate pairs
    double                                            m_Ht;              //< scalar sum of jet, Lepton pT
    unsigned int                                      m_ZcandidateIdx;   //< index of the z candidate pair in the container
    unsigned int                                      m_thirdLeptonIdx;  //< third Lepton idx corresponding to the Lepton pair
    bool                                              m_allLoose;        //< if all leptons in the event are loose
    bool                                              m_thirdLoose;      //< if the third lepton is loose;
    bool                                              m_pairLoose;       //< if the leptons in the pair are loose
    
  public:

    FinalState();
    virtual ~FinalState();
    void setMET(const FRZ::PhysicsObject<FRZ::MET> i )                 { m_MET = i;                   }
    void addLepton(const FRZ::PhysicsObject<FRZ::Lepton>& lep)         { m_leptons.push_back(lep);    }
    void addJet(const FRZ::PhysicsObject<FRZ::Jet>& j)                 { m_jets.push_back(j);         }
    void addLeptonPair(const FRZ::PhysicsObject<FRZ::LeptonPair>& lp)  { m_leptonPairs.push_back(lp); }

    std::vector<FRZ::PhysicsObject<FRZ::Lepton> >&     leptons()     { return m_leptons;     }
    std::vector<FRZ::PhysicsObject<FRZ::Jet> >&        jets()        { return m_jets;        }
    std::vector<FRZ::PhysicsObject<FRZ::LeptonPair> >& leptonPairs() { return m_leptonPairs; }
    FRZ::PhysicsObject<FRZ::MET>&                      MET()         { return m_MET;         }

    const std::vector<FRZ::PhysicsObject<FRZ::Lepton> >&     leptons()     const { return m_leptons;     }
    const std::vector<FRZ::PhysicsObject<FRZ::Jet> >&        jets()        const { return m_jets;        }
    const std::vector<FRZ::PhysicsObject<FRZ::LeptonPair> >& leptonPairs() const { return m_leptonPairs; }
    const FRZ::PhysicsObject<FRZ::MET>&                      MET()         const { return m_MET;         }
    
    int          nZcandidates()   const { return m_nZcandidates;   }
    double       Ht()             const { return m_Ht;             }
    unsigned int ZcandidateIdx()  const { return m_ZcandidateIdx;  }
    unsigned int thirdLeptonIdx() const { return m_thirdLeptonIdx; }

    bool         allLoose()       const { return m_allLoose;       }
    bool         thirdLoose()     const { return m_thirdLoose;     }
    bool         pairLoose()      const { return m_pairLoose;      }

    /// clear all of the containers
    void clear() {
      m_leptons.clear(); m_jets.clear(); m_leptonPairs.clear();
    }
    
    /// loop through pairs to get number of Z candidates
    void evaluateZcandidates();
    
    /// sets the indices for certain objects and sets the ht
    void evaluateSelf();

  };  

}

#endif
