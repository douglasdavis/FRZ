/** @file HistMaker.cxx
 *  @brief FRZ::HistMaker class implementation
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

// C++
#include <array>
#include <cstdlib>
#include <fstream>

// ROOT
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

// FRZ
#include "FRZ/HistMaker.h"
#include "FRZ/FinalState.h"
#include "FRZ/Sample.h"
#include "FRZ/Common.h"

FRZ::HistMaker::HistMaker() :
  m_thirdLepOpt(0)
{}

FRZ::HistMaker::~HistMaker() {}

void FRZ::HistMaker::addFileName(const std::string& fn)
{
  m_fileNames.push_back(fn);
  m_files.push_back(new TFile((fn).c_str(),"READ"));
}

bool FRZ::HistMaker::run(const std::string& out_name, const int third_loose)
{
  if ( m_files.empty() ) return false;

  for ( auto const& f : m_files ) {
    m_FRZtrees.push_back((TTree*)f->Get("FRZ_tree"));
    m_sampleTrees.push_back((TTree*)f->Get("Sample_tree"));
  }

  std::string thirdLepXtitle;
  if      ( m_thirdLepOpt == 11 ) thirdLepXtitle = "(e)";
  else if ( m_thirdLepOpt == 13 ) thirdLepXtitle = "(#mu)";
  else                            return false;

  bool third_loose_b = false;
  if ( third_loose == 1 ) third_loose_b = true;
  if ( third_loose == 2 ) third_loose_b = false;
  
  std::array<std::string,5> procs { { "zjets" , "diboson" , "ttbarV" , "ttbar" , "data" } };

  plot_props MET_plot_props(30,0,150);
  std::string MET_title       = ";E^{miss}_{T} [GeV];Events/"+std::to_string(MET_plot_props.binsize)+" GeV";
  std::string MET_ratio_title = ";E^{miss}_{T} [GeV];Data/MC";
  std::map<std::string,TH1D*> MET;
  auto MET_ratio = new TH1D("MET_ratio",MET_ratio_title.c_str(),
			    MET_plot_props.nbins,
			    MET_plot_props.xmin,
			    MET_plot_props.xmax);

  plot_props HT_plot_props(25,75,450);
  std::string HT_title       = ";H_{T} [GeV];Events/"+std::to_string(HT_plot_props.binsize)+" GeV";
  std::string HT_ratio_title = ";H_{T} [GeV];Data/MC";
  std::map<std::string,TH1D*> HT;
  auto HT_ratio = new TH1D("HT_ratio",HT_ratio_title.c_str(),
			   HT_plot_props.nbins,
			   HT_plot_props.xmin,
			   HT_plot_props.xmax);

  plot_props lpim_plot_props(19,Z_MINUS_15/1.0e3,Z_PLUS_15/1.0e3);
  std::string lpim_title       = ";m_{ll} [GeV];Events/"+std::to_string(lpim_plot_props.binsize)+" GeV";
  std::string lpim_ratio_title = ";m_{ll} [GeV];Data/MC";
  std::map<std::string,TH1D*> lpim;
  auto lpim_ratio = new TH1D("lpim_ratio",lpim_ratio_title.c_str(),
			     lpim_plot_props.nbins,
			     lpim_plot_props.xmin,
			     lpim_plot_props.xmax);

  plot_props njets_plot_props(6,0,6);
  std::string njets_title       = ";N_{jets};Events/"+std::to_string(njets_plot_props.binsize)+" GeV";
  std::string njets_ratio_title = ";N_{jets};Data/MC";
  std::map<std::string,TH1D*> njets;
  auto njets_ratio = new TH1D("njets_ratio",njets_ratio_title.c_str(),
			      njets_plot_props.nbins,
			      njets_plot_props.xmin,
			      njets_plot_props.xmax);

  for ( int i = 1; i < 6; ++i )
    njets_ratio->GetXaxis()->SetBinLabel(i,std::to_string(i).c_str());
  njets_ratio->GetXaxis()->SetBinLabel(6,"#geq 6");
  
  plot_props tlpt_plot_props(20,12,120);
  std::string tlpt_title        = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Events/"
    +std::to_string(tlpt_plot_props.binsize)+" GeV";
  std::string tlpt_ratio_title  = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Data/MC";
  std::map<std::string,TH1D*> tlpt;
  auto tlpt_ratio = new TH1D("tlpt_ratio",tlpt_ratio_title.c_str(),
			     tlpt_plot_props.nbins,
			     tlpt_plot_props.xmin,
			     tlpt_plot_props.xmax);

  std::string FRZ_BASE = std::getenv("FRZ_BASE");
  std::string bin_lim_file_name = FRZ_BASE+"/config/bin_limits";
  std::ifstream bin_lim_file;
  bin_lim_file.open(bin_lim_file_name.c_str());
  std::vector<double> bin_lims;
  double cur_lim;
  while ( bin_lim_file >> cur_lim ) bin_lims.push_back(cur_lim);
  bin_lim_file.close();
  
  std::string tlptv_title        = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Events";
  std::string tlptv_ratio_title  = ";3rd lepton "+thirdLepXtitle+" p_{T} [GeV];Data/MC";
  std::map<std::string,TH1D*> tlptv;
  int arrayn = bin_lims.size();
  double var_bins[arrayn];
  for ( int i = 0; i < arrayn; ++ i ) var_bins[i] = bin_lims.at(i);
  auto tlptv_ratio = new TH1D("tlptv_ratio",tlptv_ratio_title.c_str(),arrayn-1,var_bins);
  
  for ( auto const& entry : procs ) {
    MET.emplace(entry,new TH1D(("MET_"+entry).c_str(),MET_title.c_str(),
			       MET_plot_props.nbins,MET_plot_props.xmin,MET_plot_props.xmax));
    
    HT.emplace(entry,new TH1D(("HT_"+entry).c_str(),HT_title.c_str(),
			      HT_plot_props.nbins,HT_plot_props.xmin,HT_plot_props.xmax));

    lpim.emplace(entry,new TH1D(("lpim_"+entry).c_str(),lpim_title.c_str(),
				lpim_plot_props.nbins,lpim_plot_props.xmin,lpim_plot_props.xmax));

    njets.emplace(entry,new TH1D(("njets_"+entry).c_str(),njets_title.c_str(),
				 njets_plot_props.nbins,njets_plot_props.xmin,njets_plot_props.xmax));
    
    tlpt.emplace(entry,new TH1D(("tlpt_"+entry).c_str(),tlpt_title.c_str(),
				tlpt_plot_props.nbins,tlpt_plot_props.xmin,tlpt_plot_props.xmax));

    tlptv.emplace(entry,new TH1D(("tlptv_"+entry).c_str(),tlptv_title.c_str(),4,var_bins));
  }

  // loop over all trees, make temporary histograms,
  // add them to the correct total histograms.
  for ( auto const& file_itr : m_files ) {
    auto temp_hist_MET = new TH1D("temp_hist_MET","nothing",
				  MET_plot_props.nbins,
				  MET_plot_props.xmin,
				  MET_plot_props.xmax);

    auto temp_hist_HT = new TH1D("temp_hist_HT","nothing",
				  HT_plot_props.nbins,
				  HT_plot_props.xmin,
				  HT_plot_props.xmax);

    auto temp_hist_lpim = new TH1D("temp_hist_lpim","nothing",
				   lpim_plot_props.nbins,
				   lpim_plot_props.xmin,
				   lpim_plot_props.xmax);

    auto temp_hist_njets = new TH1D("temp_hist_njets","nothing",
				    njets_plot_props.nbins,
				    njets_plot_props.xmin,
				    njets_plot_props.xmax);

    auto temp_hist_tlpt = new TH1D("temp_hist_tlpt","nothing",
				   tlpt_plot_props.nbins,
				   tlpt_plot_props.xmin,
				   tlpt_plot_props.xmax);

    auto temp_hist_tlptv = new TH1D("temp_hist_tlptv","nothing",4,var_bins);

    TTreeReader readerFRZ_tree("FRZ_tree",file_itr);
    TTreeReader readerSample_tree("Sample_tree",file_itr);

    TTreeReaderValue<FRZ::Sample>        samp(readerSample_tree,"sample");
    TTreeReaderValue<float>            weight(readerSample_tree,"weight");
    TTreeReaderValue<FRZ::FinalState>      fs(readerFRZ_tree,   "FinalState");
        
    while ( readerFRZ_tree.Next() ) {
      auto third_lep_idx = fs->thirdLeptonIdx();
      auto z_cand_idx    = fs->ZcandidateIdx();
      if ( fs->leptons().at(third_lep_idx).obj().pdgId() == m_thirdLepOpt ) {
	if ( ( third_loose == 0 || third_loose == 1 ) && third_loose_b == fs->thirdLoose() ) {
	  temp_hist_MET->Fill(fs->MET().obj().Et()/1.0e3);
	  temp_hist_tlpt->Fill(fs->leptons().at(third_lep_idx).pT()/1.0e3);
	  temp_hist_tlptv->Fill(fs->leptons().at(third_lep_idx).pT()/1.0e3);
	  temp_hist_HT->Fill(fs->Ht()/1.0e3);
	  temp_hist_lpim->Fill(fs->leptonPairs().at(z_cand_idx).obj().mass()/1.0e3);
	  if ( fs->jets().size() < 6 )
	    temp_hist_njets->Fill(fs->jets().size());
	  else
	    temp_hist_njets->Fill(6);
	}
	if ( third_loose == 2 ) {
	  temp_hist_MET->Fill(fs->MET().obj().Et()/1.0e3);
	  temp_hist_tlpt->Fill(fs->leptons().at(third_lep_idx).pT()/1.0e3);
	  temp_hist_tlptv->Fill(fs->leptons().at(third_lep_idx).pT()/1.0e3);
	  temp_hist_HT->Fill(fs->Ht()/1.0e3);
	  temp_hist_lpim->Fill(fs->leptonPairs().at(z_cand_idx).obj().mass()/1.0e3);
	  if ( fs->jets().size() < 6 )
	    temp_hist_njets->Fill(fs->jets().size());
	  else
	    temp_hist_njets->Fill(6);
	}
      } // if third lepton is requestions e or mu
    } // for all in current tree


    readerSample_tree.SetEntry(0);
    auto ptype = samp->processType();

    temp_hist_MET->Scale(*weight);
    temp_hist_HT->Scale(*weight);
    temp_hist_lpim->Scale(*weight);
    temp_hist_tlpt->Scale(*weight);
    temp_hist_tlptv->Scale(*weight);
    temp_hist_njets->Scale(*weight);
    
    if ( ptype == "zlf" || ptype == "zhf" ) {
      MET.at("zjets")->Add(temp_hist_MET);
      HT.at("zjets")->Add(temp_hist_HT);
      lpim.at("zjets")->Add(temp_hist_lpim);
      tlpt.at("zjets")->Add(temp_hist_tlpt);
      tlptv.at("zjets")->Add(temp_hist_tlptv);
      njets.at("zjets")->Add(temp_hist_njets);
    }
    else if ( ptype == "ww" || ptype == "wz" || ptype == "zz" ) {
      MET.at("diboson")->Add(temp_hist_MET);
      HT.at("diboson")->Add(temp_hist_HT);
      lpim.at("diboson")->Add(temp_hist_lpim);
      tlpt.at("diboson")->Add(temp_hist_tlpt);
      tlptv.at("diboson")->Add(temp_hist_tlptv);
      njets.at("diboson")->Add(temp_hist_njets);
    }
    else if ( ptype == "ttbarW" || ptype == "ttbarZ" ) {
      MET.at("ttbarV")->Add(temp_hist_MET);
      HT.at("ttbarV")->Add(temp_hist_HT);
      lpim.at("ttbarV")->Add(temp_hist_lpim);
      tlpt.at("ttbarV")->Add(temp_hist_tlpt);
      tlptv.at("ttbarV")->Add(temp_hist_tlptv);
      njets.at("ttbarV")->Add(temp_hist_njets);
    }
    else if ( ptype == "data" ) {
      MET.at("data")->Add(temp_hist_MET);
      HT.at("data")->Add(temp_hist_HT);
      lpim.at("data")->Add(temp_hist_lpim);
      tlpt.at("data")->Add(temp_hist_tlpt);
      tlptv.at("data")->Add(temp_hist_tlptv);
      njets.at("data")->Add(temp_hist_njets);
    }
    else {
      MET.at(ptype)->Add(temp_hist_MET);
      HT.at(ptype)->Add(temp_hist_HT);
      lpim.at(ptype)->Add(temp_hist_lpim);
      tlpt.at(ptype)->Add(temp_hist_tlpt);
      tlptv.at(ptype)->Add(temp_hist_tlptv);
      njets.at(ptype)->Add(temp_hist_njets);
    }
    
    delete temp_hist_MET;
    delete temp_hist_HT;
    delete temp_hist_lpim;
    delete temp_hist_tlpt;
    delete temp_hist_tlptv;
    delete temp_hist_njets;
    
  } // for all trees
  
  // make the stacked histograms, add histograms accordingly,
  // calculate the ratio histograms.
  auto stack_MET = new THStack("stack_MET",MET_title.c_str());
  stacker(MET,stack_MET);
  makeRatio(MET_ratio,(TH1D*)stack_MET->GetStack()->Last(),MET.at("data"));

  auto stack_HT = new THStack("stack_HT",HT_title.c_str());
  stacker(HT,stack_HT);
  makeRatio(HT_ratio,(TH1D*)stack_HT->GetStack()->Last(),HT.at("data"));

  auto stack_lpim = new THStack("stack_lpim",lpim_title.c_str());
  stacker(lpim,stack_lpim);
  makeRatio(lpim_ratio,(TH1D*)stack_lpim->GetStack()->Last(),lpim.at("data"));

  auto stack_tlpt = new THStack("stack_tlpt",tlpt_title.c_str());
  stacker(tlpt,stack_tlpt);
  makeRatio(tlpt_ratio,(TH1D*)stack_tlpt->GetStack()->Last(),tlpt.at("data"));

  auto stack_tlptv = new THStack("stack_tlptv",tlptv_title.c_str());
  stacker(tlptv,stack_tlptv);
  makeRatio(tlptv_ratio,(TH1D*)stack_tlptv->GetStack()->Last(),tlptv.at("data"));

  auto stack_njets = new THStack("stack_njets",njets_title.c_str());
  stacker(njets,stack_njets);
  makeRatio(njets_ratio,(TH1D*)stack_njets->GetStack()->Last(),njets.at("data"));

  TFile *out_file = new TFile(out_name.c_str(),"RECREATE");
  for ( auto const& k : procs ) {
    MET.at(k)->Write();
    HT.at(k)->Write();
    lpim.at(k)->Write();
    tlpt.at(k)->Write();
    tlptv.at(k)->Write();
    njets.at(k)->Write();
  }
  MET_ratio->Write();
  HT_ratio->Write();
  lpim_ratio->Write();
  tlpt_ratio->Write();
  tlptv_ratio->Write();
  njets_ratio->Write();

  stack_tlptv->Write();
  
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
