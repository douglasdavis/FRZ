/** @file Swizzler.cxx
 *  @brief FRZ::Swizzler class implementation
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

// C++
#include <iostream>

// ROOT
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

// FRZ
#include "FRZ/SampleHolder.h"
#include "FRZ/Swizzler.h"
#include "FRZ/FinalState.h"
#include "FRZ/Utils.h"

FRZ::Swizzler::Swizzler()
{
  std::cout << "default FRZ::Swizzler constructor does nothing!" << std::endl;
}

FRZ::Swizzler::Swizzler(const bool is_data, const bool is_pileup)
{
  _is_data    = is_data;
  _is_pileup  = is_pileup;
  _read_chain = new TChain("mini");

  _JVF_weight     = 0; _read_chain->SetBranchAddress("scaleFactor_JVFSF",  &_JVF_weight);
  _el_sf_weight   = 0; _read_chain->SetBranchAddress("scaleFactor_ELE",    &_el_sf_weight);
  _mu_sf_weight   = 0; _read_chain->SetBranchAddress("scaleFactor_MUON",   &_mu_sf_weight);
  _pileup_weight  = 0; _read_chain->SetBranchAddress("scaleFactor_PILEUP", &_pileup_weight);
  _btag_weight    = 0; _read_chain->SetBranchAddress("scaleFactor_BTAG",   &_btag_weight);

  // MET data:
  _MET_RefFinal_em_tightpp_et	 = 0;
  _MET_RefFinal_em_tightpp_phi	 = 0;
  _MET_RefFinal_em_tightpp_sumet = 0;
  _read_chain->SetBranchAddress("met_et",    &_MET_RefFinal_em_tightpp_et);
  _read_chain->SetBranchAddress("met_phi",   &_MET_RefFinal_em_tightpp_phi);
  _read_chain->SetBranchAddress("met_sumet", &_MET_RefFinal_em_tightpp_sumet);

  // Event metadata:
  _read_chain->SetBranchAddress("runNumber",   &_RunNumber);
  _read_chain->SetBranchAddress("eventNumber", &_EventNumber);
  _read_chain->SetBranchAddress("hfor",        &_hfor_type);

  // Event classification predicates:
  _flag_DL     = 0; _read_chain->SetBranchAddress("flag_DL",     &_flag_DL);
  _channel_DL  = 0; _read_chain->SetBranchAddress("channel_DL",  &_channel_DL);
  _flag_TTZ    = 0; _read_chain->SetBranchAddress("flag_TTZ",    &_flag_TTZ);
  _channel_TTZ = 0; _read_chain->SetBranchAddress("channel_TTZ", &_channel_TTZ);

  // Lepton data:
  _lep_n = 0;
  _read_chain->SetBranchAddress("lep_n",                    &_lep_n);
  _read_chain->SetBranchAddress("lep_flag",                 &_lep_flag);
  _read_chain->SetBranchAddress("lep_trigMatched",          &_lep_inTrigger);
  _read_chain->SetBranchAddress("lep_trackd0pvunbiased",    &_lep_trackd0pvunbiased);
  _read_chain->SetBranchAddress("lep_tracksigd0pvunbiased", &_lep_tracksigd0pvunbiased);
  _read_chain->SetBranchAddress("lep_z0",                   &_lep_z0);
  _read_chain->SetBranchAddress("lep_pt",                   &_lep_pt);
  _read_chain->SetBranchAddress("lep_eta",                  &_lep_eta);
  _read_chain->SetBranchAddress("lep_phi",                  &_lep_phi);
  _read_chain->SetBranchAddress("lep_charge",               &_lep_charge);
  _read_chain->SetBranchAddress("lep_LHflag",               &_lep_LHflag);
  _read_chain->SetBranchAddress("lep_type",                 &_lep_type);

  // Jet data:
  _read_chain->SetBranchAddress("jet_eta", &_jet_eta);
  _read_chain->SetBranchAddress("jet_MV1", &_jet_flavor_weight_MV1);
  _read_chain->SetBranchAddress("jet_phi", &_jet_phi);
  _read_chain->SetBranchAddress("jet_pt",  &_jet_pt);
  _read_chain->SetBranchAddress("jet_jvf", &_jet_jvf);

  _nJets    = 0; _read_chain->SetBranchAddress("jet_n",    &_nJets);
  _nAllJets = 0; _read_chain->SetBranchAddress("alljet_n", &_nAllJets);

  _event_weight_PRETAG = 0; _read_chain->SetBranchAddress("eventWeight_PRETAG", &_event_weight_PRETAG);
  _event_weight_BTAG   = 0; _read_chain->SetBranchAddress("eventWeight_BTAG",   &_event_weight_BTAG);

  // Set some variables to hold MC-specific branches if applicable:
  if ( !_is_data ) {
    // Event metadata:
    _read_chain->SetBranchAddress("channelNumber",   &_mc_channel_number);

    // Electron truth data:
    _read_chain->SetBranchAddress("lep_truthMatched",&_lep_truth_matched);
  }

}

FRZ::Swizzler::~Swizzler() {}

void FRZ::Swizzler::addFile(const char* fname)
{
  _read_chain->Add(fname);
}

void FRZ::Swizzler::loopToFile(const char* out_file_name, const FRZ::SampleHolder& sh)
{
  // Set up the output ROOT file and tree (FRZ will hold final state info
  // sample will hold information about the sample and weighting)
  TFile *out_file    = new TFile(out_file_name,"RECREATE");
  TTree *FRZ_tree   = new TTree("FRZ_tree","FRZ_tree");
  TTree *sample_tree = new TTree("sample_tree","sample_tree");

  // Set up and write sample and sample weight information to tree & file
  if ( !_is_data ) {
    _read_chain->GetEntry(0);
    auto sample = sh.getSample(_mc_channel_number);
    auto weight = FRZ::getWeight(sample,_is_pileup);
    sample_tree->Branch("sample",&sample);
    sample_tree->Branch("weight",&weight,"weight/F");
    sample_tree->Fill();
    sample_tree->Write();
  }
  else {
    FRZ::Sample data_samp;
    data_samp.set_processType("data");
    data_samp.set_processGenerator("data");
    float weight = 1.0;
    sample_tree->Branch("sample",&data_samp);
    sample_tree->Branch("weight",&weight,"weight/F");
    sample_tree->Fill();
    sample_tree->Write();
  }

  
  // Set up the final state branch in the tree
  FRZ::FinalState FinalState;
  FRZ_tree->Branch("FinalState",&FinalState);

  // Loop over all events in the tree
  for ( auto iev = 0; iev < _read_chain->GetEntries(); ++iev ) {

    // Print little percentages
    FRZ::notifyProgress(iev,0,_read_chain->GetEntries(),"[%3.0f%%] processing event #%d");

    _read_chain->GetEntry(iev);

    // Clear all FinalState containers for each new event
    FinalState.clear();
    
    // if less than 3 leptons, who cares
    if ( _lep_n < 3 ) continue;

    // loop over all leptons in the event
    for ( unsigned int ilep = 0; ilep < _lep_n; ++ilep ) {
      if ( _lep_type[ilep] == 11 ) {
	FRZ::PhysicsObject<FRZ::Lepton> lep;
	lep.obj().set_pdgId(11);
	lep.obj().set_charge(_lep_charge[ilep]);
	lep.obj().set_z0(_lep_z0[ilep]);
	lep.obj().set_d0(_lep_trackd0pvunbiased[ilep]);
	lep.p().SetPtEtaPhiM(_lep_pt[ilep],_lep_eta[ilep],_lep_phi[ilep],ELECTRON_MASS*GeV);
	lep.obj().set_trackPt(_lep_pt[ilep]);
	lep.obj().set_isLoose( (_lep_flag[ilep] & GOOD_ELEC) != GOOD_ELEC || !passIPcuts(_lep_trackd0pvunbiased[ilep],
										_lep_tracksigd0pvunbiased[ilep],
										_lep_z0[ilep],
										_lep_eta[ilep]));
	lep.obj().set_trigger( (_lep_inTrigger[ilep] & 1) || (_lep_inTrigger[ilep] & 2) );
	lep.obj().set_d0cut(_lep_trackd0pvunbiased[ilep] / _lep_tracksigd0pvunbiased[ilep]);
	lep.obj().set_z0cut(_lep_z0[ilep]*std::sin(lep.p().Theta()));
	if ( !_is_data ) {
	  lep.obj().set_truthMatched(_lep_truth_matched[ilep]);
	}
	FinalState.leptons().push_back(lep);
      } // electron
      if ( _lep_type[ilep] == 13 ) {
	FRZ::PhysicsObject<FRZ::Lepton> lep;
	lep.obj().set_pdgId(13);
	lep.obj().set_charge(_lep_charge[ilep]);
	lep.obj().set_z0(_lep_z0[ilep]);
	lep.obj().set_d0(_lep_trackd0pvunbiased[ilep]);
	lep.p().SetPtEtaPhiM(_lep_pt[ilep],_lep_eta[ilep],_lep_phi[ilep],MUON_MASS*GeV);
	lep.obj().set_trackPt(_lep_pt[ilep]);
	lep.obj().set_isLoose( (_lep_flag[ilep] & GOOD_MUON) != GOOD_MUON || !passIPcuts(_lep_trackd0pvunbiased[ilep],
										   _lep_tracksigd0pvunbiased[ilep],
										   _lep_z0[ilep],
										   _lep_eta[ilep]));
	lep.obj().set_trigger( (_lep_inTrigger[ilep] & 1) || (_lep_inTrigger[ilep] & 2) );
	lep.obj().set_d0cut(_lep_trackd0pvunbiased[ilep] / _lep_tracksigd0pvunbiased[ilep]);
	lep.obj().set_z0cut(_lep_z0[ilep]*std::sin(lep.p().Theta()));
	if ( !_is_data ) {
	  lep.obj().set_truthMatched(_lep_truth_matched[ilep]);
	}
	FinalState.leptons().push_back(lep);
      } // muon
    } // all leptons

    if ( FinalState.leptons().size() < 3 ) continue;

    // make LeptonPairs
    FRZ::PhysicsObject<FRZ::LeptonPair> pair01;
    FRZ::PhysicsObject<FRZ::LeptonPair> pair02;
    FRZ::PhysicsObject<FRZ::LeptonPair> pair12;
    pair01.obj().setLeptons(FinalState.leptons().at(0),FinalState.leptons().at(1),0,1,2);
    pair02.obj().setLeptons(FinalState.leptons().at(0),FinalState.leptons().at(2),0,2,1);
    pair12.obj().setLeptons(FinalState.leptons().at(1),FinalState.leptons().at(2),1,2,0);
    FinalState.leptonPairs().push_back(pair01);
    FinalState.leptonPairs().push_back(pair02);
    FinalState.leptonPairs().push_back(pair12);

    // check the total number of Z candidates,
    // if it's not 1, go to the next event
    // without wasting anymore time.
    FinalState.evaluateZcandidates();
    if ( FinalState.nZcandidates() != 1 ) continue;

    // loop over all jets in the event
    for ( unsigned int ijet = 0; ijet < _nAllJets && ijet < JETSIZE; ++ijet ) {
      if ( _jet_pt[ijet] >= 25000.0 ) {
	if ( std::fabs(_jet_eta[ijet]) < 2.5 ) {
	  if ( std::fabs(_jet_jvf[ijet]) > 0.5 || _jet_pt[ijet] > 50000.0 || std::fabs(_jet_eta[ijet]) > 2.4 ) {
	    FRZ::PhysicsObject<FRZ::Jet> jet;
	    jet.p().SetPtEtaPhiM(_jet_pt[ijet],_jet_eta[ijet],_jet_phi[ijet],0.0);
	    jet.obj().set_MV1(_jet_flavor_weight_MV1[ijet]);
	    jet.obj().set_JVF(_jet_jvf[ijet]);
	    FinalState.jets().push_back(jet);
	  } // jet jvf
	} // jet eta
      } // jet pt
    } // all jets

    // set the missing Et
    FRZ::PhysicsObject<FRZ::MET> MET;
    MET.p().SetPtEtaPhiM(_MET_RefFinal_em_tightpp_et,0.0,_MET_RefFinal_em_tightpp_phi,0.0);
    MET.obj().set_Et(_MET_RefFinal_em_tightpp_et);
    MET.obj().set_sumEt(_MET_RefFinal_em_tightpp_sumet);
    FinalState.setMET(MET);

    // final stuff before filling event into tree
    FinalState.evaluateSelf();
    if ( FinalState.leptons().at(FinalState.thirdLeptonIdx()).pT() < 25.0e3 ) continue;
    
    FRZ_tree->Fill();

  } // all events

  FRZ_tree->Write();
  out_file->Close();
}
