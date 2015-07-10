/** @file  Swizzler.h
 *  @brief FRZ::Swizzler class header
 *  @class FRZ::Swizzler
 *  @brief Class to swizzle ntuple into EDM
 *
 *  The Swizzler class takes the top mini ntuple
 *  and forms a concise EDM using the FRZ library
 *  containers (Lepton, LeptonPair, Jet, MET, FinalState).
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_SWIZZLER_H
#define FRZ_SWIZZLER_H

// C++
#include <cmath>

// ROOT
class TChain;
class TFile;

// FRZ
#include "FRZ/Common.h"
#include "FRZ/SampleHolder.h"

namespace FRZ {

  class Swizzler {

  private:

    TChain *_read_chain;
    TFile  *_out_file;
    bool    _is_data;
    bool    _is_pileup;
    
  public:

    float _event_weight_PRETAG;
    float _event_weight_BTAG;
    float _JVF_weight;
    float _el_sf_weight;
    float _mu_sf_weight;
    float _pileup_weight;
    float _btag_weight;

    // MET data:
    float _MET_RefFinal_em_tightpp_et;
    float _MET_RefFinal_em_tightpp_phi;
    float _MET_RefFinal_em_tightpp_sumet;

    // Event metadata:
    Int_t _RunNumber;
    Int_t _EventNumber;
    Int_t _hfor_type;
    Int_t _mc_channel_number;

    // Event classification predicates:
    unsigned int _flag_DL;
    unsigned int _channel_DL;
    unsigned int _flag_TTZ;
    unsigned int _channel_TTZ;

    // Lepton data:
    unsigned int _lep_n;
    unsigned int _lep_flag[LEPSIZE];
    unsigned short _lep_inTrigger[LEPSIZE];
    float _lep_trackd0pvunbiased[LEPSIZE];
    float _lep_tracksigd0pvunbiased[LEPSIZE];
    float _lep_z0[LEPSIZE];
    float _lep_pt[LEPSIZE];
    float _lep_eta[LEPSIZE];
    float _lep_phi[LEPSIZE];
    float _lep_charge[LEPSIZE];
    int _lep_LHflag[LEPSIZE];
    unsigned int _lep_type[LEPSIZE];

    // Jet Data:
    float _jet_eta[JETSIZE];
    float _jet_flavor_weight_MV1[JETSIZE];
    float _jet_phi[JETSIZE];
    float _jet_pt[JETSIZE];
    float _jet_jvf[JETSIZE];
    unsigned int _nJets;
    unsigned int _nAllJets;

    // b-Jet Data: (unused, for future ref)
    static const unsigned int BJETSIZE = 3;

    /// MC-specific branches:
    bool _lep_truth_matched[LEPSIZE];

    /// Default constructor which is not used
    Swizzler();

    /// Constructor which is used to start a swizzle session
    Swizzler(const bool is_data, const bool is_pileup);

    /// virtual destructor
    virtual ~Swizzler();

    /// adds a file to the flat ntuple TChain which will be analyzed
    void addFile(const char* fname);

    /// does the real swizzling, reads flat ntuple and makes EDM
    void loopToFile(const char* out_file_name, const FRZ::SampleHolder& sh);

    /// some impact parameter cuts checker from AIDA
    bool passIPcuts(const double trackd0pvun, const double tracksigd0,
		    const double z0,          const double eta) const {
      if ( (trackd0pvun/tracksigd0) >  3.0 ) return false;
      if ( (z0/std::cosh(eta))      >  0.4 ) return false;
      if ( (trackd0pvun/tracksigd0) < -3.0 ) return false;
      if ( (z0/std::cosh(eta))      < -0.4 ) return false;
      return true;
    }

  };

}

#endif
