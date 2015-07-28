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

    TLorentzVector m_p;
    T              m_obj;
    
  public:

    PhysicsObject() {}
    virtual ~PhysicsObject() {}

    T&       obj()       { return m_obj; }
    T const& obj() const { return m_obj; }
    
    TLorentzVector&       p()       { return m_p; }
    TLorentzVector const& p() const { return m_p; }
    
    double pT()  const { return m_p.Pt();  }
    double eta() const { return m_p.Eta(); }
    double phi() const { return m_p.Phi(); }
    double E()   const { return m_p.E();   }
    double m()   const { return m_p.M();   }
    
  };

}

#endif
