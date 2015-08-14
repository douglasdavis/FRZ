#!/usr/bin/env python2

import sys
import os
import errno
import plot_utils
plot_utils.looks_atlas()

usage = 'Usage: FRZ_plot.py [in-file] [out-folder] [eps OR pdf]'
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
import ROOT
ROOT.gSystem.Load(FRZ_BASE+'/lib/libFRZ')

in_file = ROOT.TFile(in_file,'read')

hists     = ['MET','HT','tlpt','tlptv','njets','lpim']
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
            cur_hist.SetFillColorAlpha(colors[p],.7)
            cur_hist.SetLineColorAlpha(colors[p],.7)
            plot_utils.hist_formatting(cur_hist,'not_data')
            stacks[h].Add(cur_hist)
        else:
            plot_utils.hist_formatting(cur_hist,'data')
            cur_hist.GetYaxis().SetTitle('Events/'+str(cur_hist.GetXaxis().GetBinWidth(0)))
        if p == 'zjets':
            cur_hist.SetLineColor(ROOT.kBlack)
            cur_hist.SetLineWidth(2)

label_height = 0.85
            
c_tlptv  = ROOT.TCanvas('c_tlptv','c_tlptv', 600,450)
p1_tlptv = ROOT.TPad('p1_tlptv',  'p1_tlptv',0.0,0.31,0.95,0.95)
p2_tlptv = ROOT.TPad('p2_tlptv',  'p2_tlptv',0.0,0.00,0.95,0.3)
p1_tlptv.cd()
in_file.Get('tlptv_data').Draw('e')
stacks['tlptv'].Draw('same')
plot_utils.legend.Draw('same')
in_file.Get('tlptv_data').SetMinimum(0)
in_file.Get('tlptv_data').Draw('same,e')
plot_utils.atlaslabel.DrawLatex(.2,label_height,'ATLAS')
plot_utils.atlaslabelText.DrawLatex(.2+0.110,label_height,'Work in Progress, #sqrt{s} = 8 TeV, 20.3 fb^{-1}')
p2_tlptv.cd()
plot_utils.hist_ratio_formatting(ratios['tlptv'])
ratios['tlptv'].Draw('e')
plot_utils.line_at_one.Draw('same')
plot_utils.pad_margining(p1_tlptv,p2_tlptv)
c_tlptv.cd()
p1_tlptv.Draw()
p2_tlptv.Draw()
p1_tlptv.RedrawAxis()
p2_tlptv.RedrawAxis()
c_tlptv.SaveAs(out_folder+'/plot_tlptv.'+file_ext)

c_tlpt  = ROOT.TCanvas('c_tlpt','c_tlpt', 600,450)
p1_tlpt = ROOT.TPad('p1_tlpt',  'p1_tlpt',0.0,0.31,0.95,0.95)
p2_tlpt = ROOT.TPad('p2_tlpt',  'p2_tlpt',0.0,0.00,0.95,0.3)
p1_tlpt.cd()
in_file.Get('tlpt_data').Draw('e')
stacks['tlpt'].Draw('same')
plot_utils.legend.Draw('same')
in_file.Get('tlpt_data').Draw('same,e')
plot_utils.atlaslabel.DrawLatex(.2,label_height,'ATLAS')
plot_utils.atlaslabelText.DrawLatex(.2+0.110,label_height,'Work in Progress, #sqrt{s} = 8 TeV, 20.3 fb^{-1}')
p2_tlpt.cd()
plot_utils.hist_ratio_formatting(ratios['tlpt'])
ratios['tlpt'].Draw('e')
plot_utils.line_at_one.Draw('same')
plot_utils.pad_margining(p1_tlpt,p2_tlpt)
c_tlpt.cd()
p1_tlpt.Draw()
p2_tlpt.Draw()
p1_tlpt.RedrawAxis()
p2_tlpt.RedrawAxis()
c_tlpt.SaveAs(out_folder+'/plot_tlpt.'+file_ext)

c_MET  = ROOT.TCanvas('c_MET','c_MET', 600,450)
p1_MET = ROOT.TPad('p1_MET',  'p1_MET',0.0,0.31,0.95,0.95)
p2_MET = ROOT.TPad('p2_MET',  'p2_MET',0.0,0.00,0.95,0.3)
p1_MET.cd()
in_file.Get('MET_data').Draw('e')
stacks['MET'].Draw('same')
plot_utils.legend.Draw('same')
in_file.Get('MET_data').Draw('same,e')
plot_utils.atlaslabel.DrawLatex(.2,label_height,'ATLAS')
plot_utils.atlaslabelText.DrawLatex(.2+0.110,label_height,'Work in Progress, #sqrt{s} = 8 TeV, 20.3 fb^{-1}')
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
c_MET.SaveAs(out_folder+'/plot_MET.'+file_ext)

c_HT  = ROOT.TCanvas('c_HT','c_HT', 600,450)
p1_HT = ROOT.TPad('p1_HT',  'p1_HT',0.0,0.31,0.95,0.95)
p2_HT = ROOT.TPad('p2_HT',  'p2_HT',0.0,0.00,0.95,0.3)
p1_HT.cd()
in_file.Get('HT_data').Draw('e')
stacks['HT'].Draw('same')
plot_utils.legend.Draw('same')
in_file.Get('HT_data').Draw('same,e')
plot_utils.atlaslabel.DrawLatex(.2,label_height,'ATLAS')
plot_utils.atlaslabelText.DrawLatex(.2+0.110,label_height,'Work in Progress, #sqrt{s} = 8 TeV, 20.3 fb^{-1}')
p2_HT.cd()
plot_utils.hist_ratio_formatting(ratios['HT'])
ratios['HT'].Draw('e')
plot_utils.line_at_one.Draw('same')
plot_utils.pad_margining(p1_HT,p2_HT)
c_HT.cd()
p1_HT.Draw()
p2_HT.Draw()
p1_HT.RedrawAxis()
p2_HT.RedrawAxis()
c_HT.SaveAs(out_folder+'/plot_HT.'+file_ext)

c_lpim  = ROOT.TCanvas('c_lpim','c_lpim', 600,450)
p1_lpim = ROOT.TPad('p1_lpim',  'p1_lpim',0.0,0.31,0.95,0.95)
p2_lpim = ROOT.TPad('p2_lpim',  'p2_lpim',0.0,0.00,0.95,0.3)
p1_lpim.cd()
in_file.Get('lpim_data').Draw('e')
stacks['lpim'].Draw('same')
plot_utils.legend.Draw('same')
in_file.Get('lpim_data').Draw('same,e')
plot_utils.atlaslabel.DrawLatex(.2,label_height,'ATLAS')
plot_utils.atlaslabelText.DrawLatex(.2+0.110,label_height,'Work in Progress, #sqrt{s} = 8 TeV, 20.3 fb^{-1}')
p2_lpim.cd()
plot_utils.hist_ratio_formatting(ratios['lpim'])
ratios['lpim'].Draw('e')
plot_utils.line_at_one.Draw('same')
plot_utils.pad_margining(p1_lpim,p2_lpim)
c_lpim.cd()
p1_lpim.Draw()
p2_lpim.Draw()
p1_lpim.RedrawAxis()
p2_lpim.RedrawAxis()
c_lpim.SaveAs(out_folder+'/plot_lpim.'+file_ext)

c_njets  = ROOT.TCanvas('c_njets','c_njets', 600,450)
p1_njets = ROOT.TPad('p1_njets',  'p1_njets',0.0,0.31,0.95,0.95)
p2_njets = ROOT.TPad('p2_njets',  'p2_njets',0.0,0.00,0.95,0.3)
p1_njets.cd()
in_file.Get('njets_data').Draw('e')
stacks['njets'].Draw('same')
plot_utils.legend.Draw('same')
in_file.Get('njets_data').Draw('same,e')
plot_utils.atlaslabel.DrawLatex(.2,label_height,'ATLAS')
plot_utils.atlaslabelText.DrawLatex(.2+0.110,label_height,'Work in Progress, #sqrt{s} = 8 TeV, 20.3 fb^{-1}')
p2_njets.cd()
plot_utils.hist_ratio_formatting(ratios['njets'])
ratios['njets'].GetXaxis().SetLabelSize(0.175)
ratios['njets'].Draw('e')
plot_utils.line_at_one.Draw('same')
plot_utils.pad_margining(p1_njets,p2_njets)
c_njets.cd()
p1_njets.Draw()
p2_njets.Draw()
p1_njets.RedrawAxis()
p2_njets.RedrawAxis()
c_njets.SaveAs(out_folder+'/plot_njets.'+file_ext)
