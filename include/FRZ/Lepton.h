/** @file  Lepton.h
 *  @brief FRZ::Lepton class header
 *  @class FRZ::Lepton
 *  @brief Container of lepton information
 *
 *  This class holds information about
 *  reconstructed  leptons (muon or electron)
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_LEPTON_H
#define FRZ_LEPTON_H

namespace FRZ {

  class Lepton {

  private:

    int            m_pdgId;
    double         m_charge;
    double         m_d0;
    double         m_z0;
    double         m_d0cut;
    double         m_z0cut;
    double         m_dphi_MET;
    double         m_dR_closestJet;
    bool           m_truthMatched;
    bool           m_isLoose;
    bool           m_trigger;
    unsigned short m_triggerBit;
    double         m_trackPt;

  public:

    Lepton();
    virtual ~Lepton();

    void set_pdgId(const int i)                 { m_pdgId         = i; }
    void set_charge(const double i)             { m_charge        = i; }
    void set_d0(const double i)                 { m_d0            = i; }
    void set_z0(const double i)                 { m_z0            = i; }
    void set_d0cut(const double i)              { m_d0cut         = i; }
    void set_z0cut(const double i)              { m_z0cut         = i; }
    void set_dphi_MET(const double i)           { m_dphi_MET      = i; }
    void set_dR_closestJet(const double i)      { m_dR_closestJet = i; }
    void set_truthMatched(const bool i)         { m_truthMatched  = i; }
    void set_isLoose(const bool i)              { m_isLoose       = i; }
    void set_trigger(const bool i)              { m_trigger       = i; }
    void set_triggerBit(const unsigned short i) { m_triggerBit    = i; }
    void set_trackPt(const double i)            { m_trackPt       = i; }

    int            pdgId()         const { return m_pdgId;         }
    double         charge()        const { return m_charge;        }
    double         d0()            const { return m_d0;            }
    double         z0()            const { return m_z0;            }
    double         d0cut()         const { return m_d0cut;         }
    double         z0cut()         const { return m_z0cut;         }
    double         dphi_MET()      const { return m_dphi_MET;      }
    double         dR_closestJet() const { return m_dR_closestJet; }
    bool           truthMatched()  const { return m_truthMatched;  }
    bool           isLoose()       const { return m_isLoose;       }
    bool           trigger()       const { return m_trigger;       }
    unsigned short triggerBit()    const { return m_triggerBit;    }
    double         trackPt()       const { return m_trackPt;       }
    
  };

}

#endif
