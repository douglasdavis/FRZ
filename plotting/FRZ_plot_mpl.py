#!/usr/bin/env python2

import sys
in_file_name = sys.argv[1]

import ROOT
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gsc
from pylab import setp

in_file = ROOT.TFile(in_file_name)

def make_it(*args,**kwargs):
    xtitle = kwargs.get('xtitle')
    if not 'yunit' in kwargs: yunit = ''
    else: yunit  = kwargs.get('yunit')
    in_f   = kwargs.get('infile')
    htype  = kwargs.get('htype')
    
    
    procs     = ['data','zjets','diboson','ttbar','ttbarV']

    curh_hists = { ty : in_file.Get(htype+'_'+ty) for ty in procs }
    curh_ratio = in_file.Get(htype+'_ratio')

    curh_bins  = { ty :
                   [curh_hists[ty].GetXaxis().GetBinLowEdge(i+1)
                    for i in xrange(curh_hists[ty].GetNbinsX()+1)]
                   for ty in procs }
    curh_vals    = { ty :
                     [curh_bins[ty][i]+.5*curh_hists[ty].GetXaxis().GetBinWidth(i+1)
                      for i in xrange(curh_hists[ty].GetNbinsX())]
                     for ty in procs }
    curh_vals_e  = { ty :
                     [curh_hists[ty].GetBinError(i+1)
                      for i in xrange(curh_hists[ty].GetNbinsX())]
                     for ty in procs }

    curh_weights = { ty :
                     [curh_hists[ty].GetBinContent(i+1)
                      for i in xrange(curh_hists[ty].GetNbinsX())]
                     for ty in procs }

    curh_ratio_x = curh_vals['data']
    curh_ratio_y = [curh_ratio.GetBinContent(i+1)
                    for i in xrange(curh_ratio.GetNbinsX())]
    curh_ratio_yerr = [curh_ratio.GetBinError(i+1) for i in xrange(curh_ratio.GetNbinsX())]

    curh_MC_vals    = [curh_vals['ttbarV'],curh_vals['ttbar'],curh_vals['diboson'],curh_vals['zjets']]
    curh_MC_weights = [curh_weights['ttbarV'],curh_weights['ttbar'],curh_weights['diboson'],curh_weights['zjets']]
    labels          = [r'$t\bar{t}V$',r'$t\bar{t}$',r'$WW/WZ$',r'$Z$+jets']
    types           = 'stepfilled' #['stepfilled','stepfilled','stepfilled','step']
    alphas          = .65 #[.5,.5,.5,1]

    real_x_min = curh_hists['data'].GetXaxis().GetBinUpEdge(0)
    real_x_max = curh_hists['data'].GetXaxis().GetBinUpEdge(curh_hists['data'].GetNbinsX())
    bin_xerr   = [curh_hists['data'].GetXaxis().GetBinWidth(i+1)/2.0 for i in xrange(curh_hists['data'].GetNbinsX())]

    fig = plt.figure()
    gs  = gsc.GridSpec(2,1,height_ratios=[3,1])
    gs.update(hspace=0.075)
    ax0 = plt.subplot(gs[0])
    ax1 = plt.subplot(gs[1],sharex=ax0)
    setp(ax0.get_xticklabels(),visible=False)

    ax0.errorbar(curh_vals['data'],
                 curh_weights['data'],
                 fmt='ko',label=r'Data',
                 yerr=curh_vals_e['data'],
                 xerr=bin_xerr)
    ax0.hist(curh_MC_vals,
             bins=curh_bins['data'],
             weights=curh_MC_weights,
             label=labels,stacked=True,
             histtype=types,alpha=alphas,
             color=['orange','green','blue','white'],
             linewidth=1.5)
    
    ax1.set_xlabel(xtitle)
    if 'ytitle' in kwargs:
        ax0.set_ylabel(kwargs.get('ytitle'))
    else:
        ax0.set_ylabel(r'Events/'+str(curh_hists['data'].GetXaxis().GetBinWidth(0))+' '+yunit)
    ax1.set_ylabel(r'Data/MC')
    ax0.set_ylim([0,ax0.get_ylim()[1]*1.2])
    if kwargs.get('legend') == 'on':
        ax0.legend(loc='best',numpoints=1,prop={'size':10})
    else: pass

    ax1.errorbar(curh_ratio_x,curh_ratio_y,fmt='ko',yerr=curh_ratio_yerr,xerr=bin_xerr)
    if ax1.get_ylim()[1] > 10:
        ax1.set_ylim([0,ax1.get_ylim()[1]/2.0])
    else:
        ax1.set_ylim([0,ax1.get_ylim()[1]])

    one_line_x = np.linspace(real_x_min,real_x_max,1000)
    ax1.plot(one_line_x,[1 for _ in xrange(len(one_line_x))],'k--')

    ax1.set_xlim([real_x_min,real_x_max])
    ax0.set_xlim([real_x_min,real_x_max])

    ax0.text(.05,.93,'ATLAS',style='italic',transform=ax0.transAxes)
    ax0.text(.15,.93,'Work In Progress',transform=ax0.transAxes)
    ax0.text(.05,.85,r'$\sqrt{s} = 8 \mathrm{\,TeV},\,\int\,\mathcal{L}\,dt = 20.3 \mathrm{\,fb}^{-1}$',transform=ax0.transAxes)

    max_yticks = 4
    yloc = plt.MaxNLocator(max_yticks)
    ax1.yaxis.set_major_locator(yloc)
    
    fig.savefig(str(in_file_name)+'.plots.mpl/plot_'+str(htype)+'.pdf')

make_it(htype='MET',   xtitle=r'$E_T^{\mathrm{miss}}$ [GeV]',yunit='GeV',infile=in_file,legend='on')
make_it(htype='njets', xtitle=r'$N_{\mathrm{jets}}$',infile=in_file,legend='on')
make_it(htype='HT',    xtitle=r'$H_T$ [GeV]',yunit='GeV',infile=in_file,legend='on')
make_it(htype='lpim',  xtitle=r'$m_{\ell\ell}$ [GeV]',yunit='GeV',infile=in_file,legend='on')
make_it(htype='tlptv', xtitle=r'$p_{T,\ell 3}$ [GeV]',yunit='GeV',infile=in_file,legend='on',ytitle=r'Events/bin')
