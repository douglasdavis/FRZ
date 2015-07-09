/** @file  Sample.h
 *  @brief FRZ::Sample class header
 *  @class FRZ::Sample
 *  @brief Container of Sample information for scaling
 *
 *  This class holds information about
 *  the MC Sample for scaling purposes
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_SAMPLE_H
#define FRZ_SAMPLE_H

namespace FRZ {

  class Sample {

  private:

    std::string m_processType;
    std::string m_processGenerator;
    int         m_mc_channel;
    int         m_ID;
    float       m_crossSection;
    float       m_kFactor;
    float       m_nGenerated;
    float       m_nGeneratedPileup;
    float       m_nFiducial_elel;
    float       m_nFiducial_elmu;
    float       m_nFiducial_mumu;
    float       m_nGenerated_elel;
    float       m_nGenerated_elmu;
    float       m_nGenerated_mumu;
    float       m_nReconstructed_elel;
    float       m_nReconstructed_elmu;
    float       m_nReconstructed_mumu;
    float       m_BR_elel;
    float       m_BR_elmu;
    float       m_BR_mumu;

  public:

    Sample();
    virtual ~Sample();

    void dumpVars() const;

    void set_processType(const std::string& i)       { m_processType         = i;  }
    void set_processGenerator(const std::string& i)  { m_processGenerator    = i;  }
    void set_mc_channel(const int i)                 { m_mc_channel          = i;  }
    void set_ID(const int i)                         { m_ID                  = i;  }
    void set_crossSection(const float i)             { m_crossSection        = i;  }
    void set_kFactor(const float i)                  { m_kFactor             = i;  }
    void set_nGenerated(const float i)               { m_nGenerated          = i;  }
    void set_nGeneratedPileup(const float i)         { m_nGeneratedPileup    = i;  }
    void set_nFiducial_elel(const float i)           { m_nFiducial_elel      = i;  }
    void set_nFiducial_elmu(const float i)           { m_nFiducial_elmu      = i;  }
    void set_nFiducial_mumu(const float i)           { m_nFiducial_mumu      = i;  }
    void set_nGenerated_elel(const float i)          { m_nGenerated_elel     = i;  }
    void set_nGenerated_elmu(const float i)          { m_nGenerated_elmu     = i;  }
    void set_nGenerated_mumu(const float i)          { m_nGenerated_mumu     = i;  }
    void set_nReconstructed_elel(const float i)      { m_nReconstructed_elel = i;  }
    void set_nReconstructed_elmu(const float i)      { m_nReconstructed_elmu = i;  }
    void set_nReconstructed_mumu(const float i)      { m_nReconstructed_mumu = i;  }
    void set_BR_elel(const float i)                  { m_BR_elel             = i;  }
    void set_BR_elmu(const float i)                  { m_BR_elmu             = i;  }
    void set_BR_mumu(const float i)                  { m_BR_mumu             = i;  }

    const std::string& processType()      const { return m_processType;      }
    const std::string& processGenerator() const { return m_processGenerator; }

    int   mc_channel()          const { return m_mc_channel;          }
    int   ID()                  const { return m_ID;                  }
    float crossSection()        const { return m_crossSection;        }
    float kFactor()             const { return m_kFactor;             }
    float nGenerated()          const { return m_nGenerated;          }
    float nGeneratedPileup()    const { return m_nGeneratedPileup;    }
    float nFiducial_elel()      const { return m_nFiducial_elel;      }
    float nFiducial_elmu()      const { return m_nFiducial_elmu;      }
    float nFiducial_mumu()      const { return m_nFiducial_mumu;      }
    float nGenerated_elel()     const { return m_nGenerated_elel;     }
    float nGenerated_elmu()     const { return m_nGenerated_elmu;     }
    float nGenerated_mumu()     const { return m_nGenerated_mumu;     }
    float nReconstructed_elel() const { return m_nReconstructed_elel; }
    float nReconstructed_elmu() const { return m_nReconstructed_elmu; }
    float nReconstructed_mumu() const { return m_nReconstructed_mumu; }
    float BR_elel()             const { return m_BR_elel;             }
    float BR_elmu()             const { return m_BR_elmu;             }
    float BR_mumu()             const { return m_BR_mumu;             }
    
  };

}

#endif
