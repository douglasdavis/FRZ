#!/usr/bin/env python2

import sys
in_file = sys.argv[1]

import ROOT
import root2py

in_file = ROOT.TFile(in_file,'read')

samples = ['data',  'zjets', 'diboson', 'ttbar','ttbarV']
colors  = ['black', 'white', 'orange',  'blue', 'green' ]
labels  = ['Data', r'$Z+\mathrm{jets}$',r'$WW/WZ$',r'$t\overline{t}$',r'$t\overline{t}V$']
def plot_factory(dist,xt='xtitle',yt='ytitle',unit=' ',xlim=False,ymax=False,save=None):
    rh = { s : in_file.Get(dist+'_'+s) for s in samples }
    if xlim != False:
        xr = xlim
    else:
        xr = [rh['data'].GetBinLowEdge(1),
              rh['data'].GetBinLowEdge(rh['data'].GetNbinsX()+1)]
    if ymax != False:
        yr = [0,ymax]
    else:
        yr = False
    ra = in_file.Get(dist+'_ratio')
    mh = root2py.multi_hist([rh['ttbarV'],rh['ttbar'],rh['diboson'],rh['zjets']],
                            colors=colors[::-1][:-1],histlabels=labels[::-1][:-1],
                            stacked=True,data=rh['data'],ratio=ra,xlim=xr,ylim=yr,
                            titles=[xt,yt])
    #mh.text(.02,.825,
    #        r'$\sqrt{s}=8\,\mathrm{TeV}\,\int\,\mathcal{L}\,dt = 20.3\,\,\mathrm{fb}^{-1}$',
    #        size=14)
    mh.text(.02,.825,
            r'$\sqrt{s}=8\,\mathrm{TeV},\,\, 20.3\,\,\mathrm{fb}^{-1}$',
            size=14)
    mh.draw(awip=True,save=save)
    
plot_factory('MET',save='wow.eps',xt=r'$E_T^{\mathrm{miss}}\,\,\mathrm{[GeV]}$',
             yt='Events/bin',ymax=150)
plot_factory('tlptv',ymax=400)
