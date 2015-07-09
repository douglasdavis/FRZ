/** @file  PhysicsObject.h
 *  @brief FRZ::PhysicsObject class header
 *  @class FRZ::PhysicsObject
 *  @brief Container of PhysicsObject information
 *
 *  This template class holds information
 *  about reconstructed physics objects,
 *  leptons, jets, MET.
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_PHYSICSOBJECT_H
#define FRZ_PHYSICSOBJECT_H

#include "TLorentzVector.h"

namespace FRZ {
  
  template <class T> class PhysicsObject {

  private:

    TLorentzVector m_P;
    T              m_obj;
    
  public:

    PhysicsObject() {}
    virtual ~PhysicsObject() {}

    T&       obj()       { return m_obj; }
    T const& obj() const { return m_obj; }
    
    TLorentzVector&       P()       { return m_P; }
    TLorentzVector const& P() const { return m_P; }
    
    double pT()  const { return m_P.Pt();  }
    double eta() const { return m_P.Eta(); }
    double phi() const { return m_P.Phi(); }
    double E()   const { return m_P.E();   }
    double m()   const { return m_P.M();   }
    
  };

}

#endif
