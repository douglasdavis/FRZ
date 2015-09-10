#!/usr/bin/env python2

import sys
in_file_name = sys.argv[1]

import ROOT
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gsc
from pylab import setp

in_file = ROOT.TFile(in_file_name)

def get_content(hist):
    """ the height of each bin """
    return np.array([hist.GetBinContent(i+1)
                     for i in xrange(hist.GetNbinsX())])

def get_error(hist):
    """ the error bar for each bin """
    return np.array([hist.GetBinError(i+1)
                     for i in xrange(hist.GetNbinsX())])

def get_bin_edges(hist):
    """ the bin edges |____|____|____|___..
                     [0]  [1]  [2]  [3]       """
    return np.array([hist.GetXaxis().GetBinLowEdge(i+1)
                     for i in xrange(hist.GetNbinsX()+1)])

def get_bin_centers(hist):
    """ the centers of each bin |_____|_____|_____|__...
                                  [0]   [1]   [2]          """
    return np.array([(hist.GetXaxis().GetBinWidth(i+1)*0.5 +
                      hist.GetXaxis().GetBinLowEdge(i+1))
                     for i in xrange(hist.GetNbinsX())])

def make_it(*args, **kwargs):
    ## first we set up some beginning arguments
    xtitle = kwargs.get('xtitle')
    if not 'yunit' in kwargs: yunit = ''
    else: yunit  = kwargs.get('yunit')
    in_f   = kwargs.get('infile')
    htype  = kwargs.get('htype')

    ## declare list of the difference processes
    ## and generate lists of histograms for each process
    procs      = ['data','zjets','diboson','ttbar','ttbarV']
    curh_hists = { ty : in_file.Get(htype+'_'+ty) for ty in procs }
    curh_ratio = in_file.Get(htype+'_ratio')

    ## use the above functions to get numpy arrays for parts of the histogram
    ## we want the bin edges, the bin centers, the error bars, and the bin content
    curh_bins    = { ty : get_bin_edges(curh_hists[ty])   for ty in procs }
    curh_vals    = { ty : get_bin_centers(curh_hists[ty]) for ty in procs }
    curh_vals_e  = { ty : get_error(curh_hists[ty])       for ty in procs }
    curh_weights = { ty : get_content(curh_hists[ty])     for ty in procs }
    bin_xerr     = [curh_hists['data'].GetXaxis().GetBinWidth(i+1)/2.0
                    for i in xrange(curh_hists['data'].GetNbinsX())]

    ## get characteristics for the ratio histogram
    curh_ratio_x = curh_vals['data']
    curh_ratio_y = [curh_ratio.GetBinContent(i+1)
                    for i in xrange(curh_ratio.GetNbinsX())]
    curh_ratio_yerr = [curh_ratio.GetBinError(i+1) for i in xrange(curh_ratio.GetNbinsX())]

    ## set the edges of the plot axes to it looks nice
    ## only want the min bin left edge to the max bin right edge
    ## *remember* ROOT histograms have first bin as underflow last bin as overflow
    real_x_min = curh_hists['data'].GetXaxis().GetBinUpEdge(0)
    real_x_max = curh_hists['data'].GetXaxis().GetBinUpEdge(curh_hists['data'].GetNbinsX())

    ## put get each process np.array into a list for histogramming, and declare some labels
    curh_MC_vals    = [curh_vals['ttbarV'],curh_vals['ttbar'],curh_vals['diboson'],curh_vals['zjets']]
    curh_MC_weights = [curh_weights['ttbarV'],curh_weights['ttbar'],curh_weights['diboson'],curh_weights['zjets']]
    labels          = [r'$t\bar{t}V$',r'$t\bar{t}$',r'$WW/WZ$',r'$Z$+jets']
    h_type          = 'stepfilled'
    h_alpha         = 1.0

    ## set up the plotting canvas
    fig = plt.figure(figsize=(9,6))
    gs  = gsc.GridSpec(2,1,height_ratios=[3,1])
    gs.update(hspace=0.075)
    ax0 = plt.subplot(gs[0])
    ax1 = plt.subplot(gs[1],sharex=ax0)
    setp(ax0.get_xticklabels(),visible=False)

    ## setup all the MC histograms (the stack)
    ax0.hist(curh_MC_vals,
             bins=curh_bins['data'],
             weights=curh_MC_weights,
             label=labels,stacked=True,
             histtype=h_type,alpha=h_alpha,
             color=['green','orange','blue','white'],
             linewidth=1.25)

    ## plot the data
    ax0.errorbar(curh_vals['data'],
                 curh_weights['data'],
                 fmt='ko',label=r'Data',
                 yerr=curh_vals_e['data'],
                 xerr=bin_xerr)

    ## now just make the plots look pretty
    if 'ytitle' in kwargs:
        ax0.set_ylabel(kwargs.get('ytitle'),size=16)
    else:
        ax0.set_ylabel(r'Events/'+str(round(curh_hists['data'].GetXaxis().GetBinWidth(0),2))+' '+yunit,size=16)
    ax0.set_ylim([0,ax0.get_ylim()[1]*1.25])
    if kwargs.get('legend') == 'on':
        ax0.legend(loc='best',numpoints=1,prop={'size':14})
    else: pass

    ## plot the Data/MC ratio on the lower subplot
    ax1.errorbar(curh_ratio_x,curh_ratio_y,fmt='ko',yerr=curh_ratio_yerr,xerr=bin_xerr)

    ## make sure to make it pretty too
    if ax1.get_ylim()[1] > 10:
        ax1.set_ylim([0,10])
    else:
        ax1.set_ylim([0,ax1.get_ylim()[1]])
    one_line_x = np.linspace(real_x_min,real_x_max,1000)
    ax1.plot(one_line_x,[1 for _ in xrange(len(one_line_x))],'k--')
    ax1.set_ylabel(r'Data/MC',size=16)
    ax1.set_xlabel(xtitle,size=16)

    ## declaring those limits using the bin edges discussed above
    ax1.set_xlim([real_x_min,real_x_max])
    ax0.set_xlim([real_x_min,real_x_max])

    ## give that nice ATLAS text
    ax0.text(.05,.92,'ATLAS',style='italic',transform=ax0.transAxes,size=16)
    ax0.text(.175,.92,'Work In Progress',transform=ax0.transAxes,size=16)
    ax0.text(.05,.805,
             r'$\sqrt{s}\,=\,8 \mathrm{\,TeV},\,\int\,\mathcal{L}\,dt\,=\,20.3 \mathrm{\,fb}^{-1}$',
             transform=ax0.transAxes,size=16)

    ## make sure not too many ticks on the ratio plot
    max_yticks = 4
    yloc = plt.MaxNLocator(max_yticks)
    ax1.yaxis.set_major_locator(yloc)

    ## and finally save that sucker
    fig.savefig(str(in_file_name)+'.plots.mpl/plot_'+str(htype)+'.pdf')

make_it(htype='MET',   xtitle=r'$E_T^{\mathrm{miss}}$ [GeV]',yunit='GeV',infile=in_file,legend='on')
make_it(htype='njets', xtitle=r'$N_{\mathrm{jets}}$',infile=in_file,legend='on')
make_it(htype='HT',    xtitle=r'$H_T$ [GeV]',yunit='GeV',infile=in_file,legend='on')
make_it(htype='lpim',  xtitle=r'$m_{\ell\ell}$ [GeV]',yunit='GeV',infile=in_file,legend='on')
make_it(htype='tlptv', xtitle=r'$p_{T,\ell 3}$ [GeV]',yunit='GeV',infile=in_file,legend='on',ytitle=r'Events/bin')
