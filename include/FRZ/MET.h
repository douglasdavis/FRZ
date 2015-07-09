/** @file  MET.h
 *  @brief FRZ::MET class header
 *  @class FRZ::MET
 *  @brief Container of MET information
 *
 *  This class holds information about
 *  reconstructed  missing tranvserse energy
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_MET_H
#define FRZ_MET_H

namespace FRZ {

  class MET {

  private:

    double         m_Et;
    double         m_sumEt;

  public:

    MET();
    virtual ~MET();
    
    void set_Et(const double i)    { m_Et    = i; }
    void set_sumEt(const double i) { m_sumEt = i; }

    double Et()    const { return m_Et;    }
    double sumEt() const { return m_sumEt; }

  };
  
}

#endif
