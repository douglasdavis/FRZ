/** @file HistMaker.cxx
 *  @brief FRZ::HistMaker class implementation
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

// C++
#include <array>

// ROOT
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"

// FRZ
#include "FRZ/HistMaker.h"
#include "FRZ/FinalState.h"
#include "FRZ/Sample.h"

FRZ::HistMaker::HistMaker() :
  m_thirdLepOpt(0)
{}

FRZ::HistMaker::~HistMaker() {}

void FRZ::HistMaker::addFileName(const std::string& fn)
{
  m_fileNames.push_back(fn);
  m_files.push_back(new TFile((fn).c_str(),"READ"));
}

bool FRZ::HistMaker::run(const std::string& out_name)
{
  if ( m_files.empty() ) return false;

  for ( auto const& f : m_files ) {
    m_FRZtrees.push_back((TTree*)f->Get("FRZ_tree"));
    m_sampleTrees.push_back((TTree*)f->Get("sample_tree"));
  }

  std::string thirdLepXtitle;
  if      ( m_thirdLepOpt == 11 ) thirdLepXtitle = "(e)";
  else if ( m_thirdLepOpt == 13 ) thirdLepXtitle = "(#mu)";
  else                            return false;
			         
  std::array<std::string,5> procs { { "zjets" , "diboson" , "ttbarV" , "ttbar" , "data" } };

  plot_props MET_plot_props(30,0,150);
  std::string MET_title       = ";E^{miss}_{T} [GeV];Events/"+std::to_string((int)MET_plot_props.binsize)+" GeV";
  std::string MET_ratio_title = ";E^{miss}_{T} [GeV];Data/MC";
  std::map<std::string,TH1D*> MET;
  auto MET_ratio = new TH1D("MET_ratio",MET_ratio_title.c_str(),
			    MET_plot_props.nbins,
			    MET_plot_props.xmin,
			    MET_plot_props.xmax);

  plot_props tlpts_plot_props(3,20,230);
  std::string tlpts_title        = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Events/"+std::to_string((int)tlpts_plot_props.binsize)+" GeV";
  std::string tlpts_ratio_title  = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Data/MC";
  std::map<std::string,TH1D*> tlpts;
  auto tlpts_ratio = new TH1D("tlpts_ratio",tlpts_ratio_title.c_str(),
			      tlpts_plot_props.nbins,
			      tlpts_plot_props.xmin,
			      tlpts_plot_props.xmax);

  plot_props tlpt_plot_props(20,20,220);
  std::string tlpt_title        = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Events/"+std::to_string((int)tlpt_plot_props.binsize)+" GeV";
  std::string tlpt_ratio_title  = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Data/MC";
  std::map<std::string,TH1D*> tlpt;
  auto tlpt_ratio = new TH1D("tlpt_ratio",tlpt_ratio_title.c_str(),
			     tlpt_plot_props.nbins,
			     tlpt_plot_props.xmin,
			     tlpt_plot_props.xmax);

  for ( auto const& entry : procs ) {
    MET[entry] = new TH1D(("MET_"+entry).c_str(),MET_title.c_str(),
			  MET_plot_props.nbins,MET_plot_props.xmin,MET_plot_props.xmax);

    tlpt[entry] = new TH1D(("tlpt_"+entry).c_str(),tlpt_title.c_str(),
			   tlpt_plot_props.nbins,tlpt_plot_props.xmin,tlpt_plot_props.xmax);

    tlpts[entry] = new TH1D(("tlpts_"+entry).c_str(),tlpts_title.c_str(),
			    tlpts_plot_props.nbins,tlpts_plot_props.xmin,tlpts_plot_props.xmax);
  }

  // loop over all trees, make temporary histograms,
  // add them to the correct total histograms.
  for ( unsigned int i = 0; i < m_FRZtrees.size(); ++i ) {
    auto temp_hist_MET = new TH1D(("temp_hist_MET"+std::to_string(i)).c_str(),"nothing",
				  MET_plot_props.nbins,
				  MET_plot_props.xmin,
				  MET_plot_props.xmax);

    auto temp_hist_tlpt = new TH1D(("temp_hist_tlpt"+std::to_string(i)).c_str(),"nothing",
				   tlpt_plot_props.nbins,
				   tlpt_plot_props.xmin,
				   tlpt_plot_props.xmax);

    auto temp_hist_tlpts = new TH1D(("temp_hist_tlpts"+std::to_string(i)).c_str(),"nothing",
				    tlpts_plot_props.nbins,
				    tlpts_plot_props.xmin,
				    tlpts_plot_props.xmax);

    FRZ::FinalState *fs     = 0;
    FRZ::Sample     *samp   = 0;
    float             weight = 0;
    m_FRZtrees.at(i)->SetBranchAddress("FinalState",&fs);
    m_sampleTrees.at(i)->SetBranchAddress("sample",&samp);
    m_sampleTrees.at(i)->SetBranchAddress("weight",&weight);
    m_sampleTrees.at(i)->GetEntry(0);
    auto ptype = samp->processType();

    for ( Long64_t iev = 0; iev < m_FRZtrees.at(i)->GetEntries(); ++iev ) {
      m_FRZtrees.at(i)->GetEntry(iev);
      auto third_lep_idx = fs->thirdLeptonIdx();
      //auto z_cand_idx    = fs->z_candidate_idx();
      if ( fs->leptons().at(third_lep_idx).obj().pdgId() == m_thirdLepOpt ) {
	temp_hist_MET->Fill(fs->MET().obj().Et()/1.0e3);
	temp_hist_tlpt->Fill(fs->leptons().at(third_lep_idx).P().Pt()/1.0e3);
	temp_hist_tlpts->Fill(fs->leptons().at(third_lep_idx).P().Pt()/1.0e3);
      } // if third lepton is requestions e or mu
    } // for all in current tree

    temp_hist_MET->Scale(weight);
    temp_hist_tlpt->Scale(weight);
    temp_hist_tlpts->Scale(weight);

    if ( ptype == "zlf" || ptype == "zhf" ) {
      MET.at("zjets")->Add(temp_hist_MET);
      tlpt.at("zjets")->Add(temp_hist_tlpt);
      tlpts.at("zjets")->Add(temp_hist_tlpts);
    }
    else if ( ptype == "ww" || ptype == "wz" || ptype == "zz" ) {
      MET.at("diboson")->Add(temp_hist_MET);
      tlpt.at("diboson")->Add(temp_hist_tlpt);
      tlpts.at("diboson")->Add(temp_hist_tlpts);
    }
    else if ( ptype == "ttbarW" || ptype == "ttbarZ" ) {
      MET.at("ttbarV")->Add(temp_hist_MET);
      tlpt.at("ttbarV")->Add(temp_hist_tlpt);
      tlpts.at("ttbarV")->Add(temp_hist_tlpts);
    }
    else if ( ptype == "data" ) {
      MET.at("data")->Add(temp_hist_MET);
      tlpt.at("data")->Add(temp_hist_tlpt);
      tlpts.at("data")->Add(temp_hist_tlpts);
    }
    else {
      MET.at(ptype)->Add(temp_hist_MET);
      tlpt.at(ptype)->Add(temp_hist_tlpt);
      tlpts.at(ptype)->Add(temp_hist_tlpts);
    }
    
  } // for all trees
  
  // make the stacked histograms, add histograms accordingly,
  // calculate the ratio histograms.
  auto stack_MET = new THStack("stack_MET",MET_title.c_str());
  stacker(MET,stack_MET);
  makeRatio(MET_ratio,(TH1D*)stack_MET->GetStack()->Last(),MET.at("data"));

  auto stack_tlpt = new THStack("stack_tlpt",tlpt_title.c_str());
  stacker(tlpt,stack_tlpt);
  makeRatio(tlpt_ratio,(TH1D*)stack_tlpt->GetStack()->Last(),tlpt.at("data"));

  auto stack_tlpts = new THStack("stack_tlpts",tlpts_title.c_str());
  stacker(tlpts,stack_tlpts);
  makeRatio(tlpts_ratio,(TH1D*)stack_tlpts->GetStack()->Last(),tlpts.at("data"));

  TFile *out_file = new TFile(out_name.c_str(),"RECREATE");
  for ( auto const& k : procs ) {
    MET.at(k)->Write();
    tlpt.at(k)->Write();
    tlpts.at(k)->Write();
  }
  MET_ratio->Write();
  tlpt_ratio->Write();
  tlpts_ratio->Write();

  out_file->Close();      

  return true;
}


void FRZ::HistMaker::makeRatio(TH1D* ratio, const TH1D* mc, const TH1D* data) {
  for ( int ibin = 0; ibin < data->GetNbinsX(); ++ibin ) {
    auto mc_val = mc->GetBinContent(ibin+1);
    auto mc_err = mc->GetBinError(ibin+1);
    auto da_val = data->GetBinContent(ibin+1);
    auto da_err = data->GetBinError(ibin+1);
    double ra_err = 0.0;
    if ( mc_val == 0 ) {
      ratio->SetBinContent(ibin+1,0);
      ratio->SetBinError(ibin+1,0);
    }
    else {
      ra_err = std::sqrt(std::pow(da_err/mc_val,2)+std::pow((da_val*mc_err)/std::pow(mc_val,2),2));
      ratio->SetBinContent(ibin+1,da_val/mc_val);
      ratio->SetBinError(ibin+1,ra_err);
    }
  }
}

void FRZ::HistMaker::stacker(const std::map<std::string,TH1D*>& hm, THStack* stack) {
  stack->Add(hm.at("ttbarV"));
  stack->Add(hm.at("ttbar"));
  stack->Add(hm.at("diboson"));
  stack->Add(hm.at("zjets"));
}
