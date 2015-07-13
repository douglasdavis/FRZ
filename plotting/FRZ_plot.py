#!/usr/bin/env python2

import ROOT
import sys
import os
import errno
import plot_utils
plot_utils.looks_atlas()

usage = 'Usage: FRZ_plot2.py [in-file] [out-folder] [eps OR pdf]'
if len(sys.argv) != 4:
    sys.exit(usage)
if not os.path.exists(sys.argv[1]):
    sys.exit('ERROR: file %s not found.' % sys.argv[1])
try:
    os.makedirs(sys.argv[2])
    print 'Making directory',sys.argv[2]
except:
    pass
if str(sys.argv[3]) != 'eps' and str(sys.argv[3]) != 'pdf':
    sys.exit(usage)

in_file    = sys.argv[1]
out_folder = sys.argv[2]
file_ext   = sys.argv[3]

FRZ_BASE = os.environ['FRZ_BASE']
ROOT.gSystem.Load(FRZ_BASE+'/lib/libFRZ')

in_file = ROOT.TFile(in_file,'read')

hists     = ['MET','tlpt','tlpts']
processes = ['ttbarV','ttbar','diboson','zjets','data']
colors    = { 'zjets'  : ROOT.kWhite    , 'diboson': ROOT.kAzure+2 ,
              'ttbarV' : ROOT.kOrange+1 , 'ttbar'  : ROOT.kGreen-2 ,
              'data'   : ROOT.kBlack                               }

stacks = { h : ROOT.THStack(h+'_stack','holder') for h in hists }
ratios = { h : in_file.Get(h+'_ratio')           for h in hists }

for h in hists:
    for p in processes:
        cur_hist = in_file.Get(h+'_'+p)
        if not p == 'data':
            cur_hist.SetFillColor(colors[p])
            cur_hist.SetLineColor(colors[p])
            plot_utils.hist_formatting(cur_hist,'not_data')
            stacks[h].Add(cur_hist)
        else:
            plot_utils.hist_formatting(cur_hist,'data')
        if p == 'zjets':
            cur_hist.SetLineColor(ROOT.kBlack)
            cur_hist.SetLineWidth(2)

c_MET  = ROOT.TCanvas('c_MET','c_MET', 600,450)
p1_MET = ROOT.TPad('p1_MET',  'p1_MET',0.0,0.31,0.95,0.95)
p2_MET = ROOT.TPad('p2_MET',  'p2_MET',0.0,0.00,0.95,0.3)
p1_MET.cd()
in_file.Get('MET_data').Draw('e')
stacks['MET'].Draw('same')
plot_utils.legend.Draw('same')
in_file.Get('MET_data').Draw('same,e')
plot_utils.atlaslabel.DrawLatex(.2,.83,'ATLAS')
plot_utils.atlaslabelText.DrawLatex(.2+0.110,.83,'Work in Progress, #sqrt{s} = 8 TeV, 20.3 fb^{-1}')
p2_MET.cd()
plot_utils.hist_ratio_formatting(ratios['MET'])
ratios['MET'].Draw('e')
plot_utils.line_at_one.Draw('same')
plot_utils.pad_margining(p1_MET,p2_MET)
c_MET.cd()
p1_MET.Draw()
p2_MET.Draw()
p1_MET.RedrawAxis()
p2_MET.RedrawAxis()
c_MET.SaveAs(out_folder+'/MET.'+file_ext)
