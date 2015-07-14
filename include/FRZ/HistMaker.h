/** @file  HistMaker.h
 *  @brief FRZ::HistMaker class header
 *  @class FRZ::HistMaker
 *  @brief generate histograms from swizzled file
 *
 *  This class handles the generation
 *  of histograms from the swizzled files
 *  (including scaling)
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_HISTMAKER_H
#define FRZ_HISTMAKER_H

// C++
#include <vector>
#include <map>

class TFile;
class TTree;
class TH1D;
class THStack;

namespace FRZ {

  class HistMaker {

  private:

    std::vector<std::string> m_fileNames;
    std::vector<TFile*>      m_files;
    std::vector<TTree*>      m_FRZtrees;
    std::vector<TTree*>      m_sampleTrees;
    int                      m_thirdLepOpt;

    struct plot_props {
      int    nbins;
      double xmin;
      double xmax;
      double binsize;
      plot_props(int nb, double min, double max) {
	nbins = nb;
	xmin  = min;
	xmax  = max;
	binsize = (xmax - xmin)/(double)nbins;
      }
      virtual ~plot_props() {}
    };

  public:

    HistMaker();
    virtual ~HistMaker();

    void setThirdLepOpt(const int i) { m_thirdLepOpt = i; }
    
    void addFileName(const std::string& fn);
    bool run(const std::string& out_name, const int third_tight);
    void makeRatio(TH1D* ratio, const TH1D *mc, const TH1D* data);
    void stacker(const std::map<std::string,TH1D*>& hm, THStack* stack);

  };

}

#endif
