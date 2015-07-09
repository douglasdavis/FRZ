/** @file  LeptonPair.h
 *  @brief FRZ::LeptonPair class header
 *  @class FRZ::LeptonPair
 *  @brief Container of LeptonPair information
 *
 *  This class holds information about
 *  pairs on reconstructed Leptons
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_LEPTONPAIR_H
#define FRZ_LEPTONPAIR_H

#include "FRZ/PhysicsObject.h"
#include "FRZ/Lepton.h"

namespace FRZ {

  class LeptonPair {

  private:

    double         m_mass;
    unsigned int   m_lepton1idx;
    unsigned int   m_lepton2idx;
    bool           m_mumu;
    bool           m_elel;
    bool           m_OF;
    bool           m_SS;
    bool           m_OS;
    bool           m_Zcandidate;
    unsigned int   m_thirdLeptonIdx;
    
  public:

    /// default constructor (only one used)
    LeptonPair();

    /// virtual destructor
    virtual ~LeptonPair();

    /** assigns 2 Leptons to the pair with
     *  corresponding index information, sets
     *  the pair four vector information
     */
    void setLeptons(const FRZ::PhysicsObject<FRZ::Lepton> lep1,
		    const FRZ::PhysicsObject<FRZ::Lepton> lep2,
		    const unsigned int idx1,
		    const unsigned int idx2,
		    const unsigned int idx3);

    double        mass()           const { return m_mass;           }
    unsigned int  lepton1idx()     const { return m_lepton1idx;     }
    unsigned int  lepton2idx()     const { return m_lepton2idx;     }
    bool          mumu()           const { return m_mumu;           }
    bool          elel()           const { return m_elel;           }
    bool          OF()             const { return m_OF;             }
    bool          SS()             const { return m_SS;             }
    bool          OS()             const { return m_OS;             }
    bool          Zcandidate()     const { return m_Zcandidate;     }
    unsigned int  thirdLeptonIdx() const { return m_thirdLeptonIdx; }
    
  };

}

#endif
