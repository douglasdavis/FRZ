/** @file  Jet.h
 *  @brief FRZ::Jet class header
 *  @class FRZ::Jet
 *  @brief Container of Jet information
 *
 *  This class holds information about
 *  reconstructed Jets
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_JET_H
#define FRZ_JET_H

namespace FRZ {

  class Jet {

  private:

    double         m_MV1;
    double         m_JVF;

  public:

    Jet();
    virtual ~Jet();

    void set_MV1(const double i) { m_MV1 = i; }
    void set_JVF(const double i) { m_JVF = i; }

    double MV1() const { return m_MV1; }
    double JVF() const { return m_JVF; }
    
  };

}

#endif
