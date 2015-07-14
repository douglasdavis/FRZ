import math
import ROOT
from ROOT import gStyle, gROOT, TStyle

def looks_atlas():

    atlasStyle = TStyle("ATLAS","Atlas style");
    icol=0
    atlasStyle.SetFrameBorderMode(icol)
    atlasStyle.SetFrameFillColor(icol)
    atlasStyle.SetCanvasBorderMode(icol)
    atlasStyle.SetCanvasColor(icol)
    atlasStyle.SetPadBorderMode(icol)
    atlasStyle.SetPadColor(icol)
    atlasStyle.SetStatColor(icol)
    atlasStyle.SetPaperSize(20,26)

    atlasStyle.SetPadTopMargin(0.05)
    atlasStyle.SetPadRightMargin(0.05)
    atlasStyle.SetPadBottomMargin(0.16)
    atlasStyle.SetPadLeftMargin(0.16)
    
    atlasStyle.SetTitleXOffset(1.4)
    atlasStyle.SetTitleYOffset(1.4)

    font=42
    tsize=0.05
    atlasStyle.SetTextFont(font)

    atlasStyle.SetTextSize(tsize)
    atlasStyle.SetLabelFont(font,'x')
    atlasStyle.SetTitleFont(font,'x')
    atlasStyle.SetLabelFont(font,'y')
    atlasStyle.SetTitleFont(font,'y')
    atlasStyle.SetLabelFont(font,'z')
    atlasStyle.SetTitleFont(font,'z')

    atlasStyle.SetLabelSize(tsize,'x')
    atlasStyle.SetTitleSize(tsize,'x')
    atlasStyle.SetLabelSize(tsize,'y')
    atlasStyle.SetTitleSize(tsize,'y')
    atlasStyle.SetLabelSize(tsize,'z')
    atlasStyle.SetTitleSize(tsize,'z')

    atlasStyle.SetMarkerStyle(20)
    atlasStyle.SetMarkerSize(1.2)
    atlasStyle.SetHistLineWidth(2)
    atlasStyle.SetLineStyleString(2,'[12 12]')

    atlasStyle.SetEndErrorSize(0.)
    

    atlasStyle.SetOptTitle(0)
    atlasStyle.SetOptStat(0)
    atlasStyle.SetOptFit(0)

    atlasStyle.SetPadTickX(1)
    atlasStyle.SetPadTickY(1)

    atlasStyle.cd()
    gROOT.ForceStyle()

class plot_props:
    def __init__(self,nbins,xmin,xmax):
        self.nbins   = nbins
        self.xmin    = xmin
        self.xmax    = xmax
        self.binsize = (float(xmax) - float(xmin))/float(nbins) 
            
def hist_formatting(chist,ptype):
    chist.SetLineWidth(0)
    chist.SetMarkerStyle(8)
    chist.SetMarkerSize(1)
    chist.GetXaxis().SetLabelOffset(99)
    chist.GetXaxis().SetTitleOffset(99)
    chist.GetYaxis().SetTitleOffset(.9)
    chist.GetYaxis().SetTitleSize(.065)
    chist.GetYaxis().SetLabelSize(.0625)
    
    if ptype == 'data':
        chist.SetLineWidth(1)
        chist.SetMaximum(chist.GetMaximum()*1.275)
            
def hist_ratio_formatting(hist):
    hist.SetMarkerStyle(8)
    hist.SetMarkerSize(1)
    hist.SetLineWidth(1)
    hist.SetLineColor(1)
    hist.GetYaxis().SetNdivisions(5)
    hist.GetXaxis().SetLabelSize(0.12)
    hist.GetXaxis().SetTitleSize(0.13)
    hist.GetYaxis().SetLabelSize(0.12)
    hist.GetYaxis().SetTitleSize(0.13)
    hist.GetXaxis().SetTitleOffset(1.4)
    hist.GetYaxis().SetTitleOffset(.45)
    hist.GetYaxis().CenterTitle()
    hist.SetMinimum(0)
    
def format_legend(legend):
    legend.SetTextFont(42)
    legend.SetTextSize(0.0575)
    legend.SetFillStyle(0)
    legend.SetBorderSize(0)

def pad_margining(pad1,pad2):
    pad1.SetBottomMargin(0.03)
    pad2.SetTopMargin(0.045)
    #pad1.SetLeftMargin(.1)
    #pad2.SetLeftMargin(.1)
    pad1.SetRightMargin(.01)
    pad2.SetRightMargin(.01)
    pad2.SetBottomMargin(0.475)
    pad2.SetFrameFillColor(0)
    pad2.SetFrameBorderMode(0)

legend = ROOT.TLegend(.815,.55,1.025,.92)
format_legend(legend)
leg_data = ROOT.TH1D('leg_data','leg_data',100,1,0)
leg_data.SetLineColor(ROOT.kBlack)
leg_data.SetMarkerStyle(8)
#leg_data.SetMarkerSize(.5)
leg_zjets = ROOT.TH1D('leg_zjets','leg_zjets',100,1,0)
leg_zjets.SetLineWidth(2)
leg_zjets.SetLineColor(ROOT.kBlack)
leg_zjets.SetFillColor(0)
leg_diboson = ROOT.TH1D('leg_diboson','leg_diboson',100,1,0)
leg_diboson.SetLineWidth(0)
leg_diboson.SetFillColor(ROOT.kAzure+2)
leg_diboson.SetLineColor(ROOT.kAzure+2)
leg_ttbar = ROOT.TH1D('leg_ttbar','leg_ttbar',100,1,0)
leg_ttbar.SetLineWidth(0)
leg_ttbar.SetFillColor(ROOT.kGreen-2)
leg_ttbar.SetLineColor(ROOT.kGreen-2)
leg_ttbarV = ROOT.TH1D('leg_ttbarV','leg_ttbarV',100,1,0)
leg_ttbarV.SetLineWidth(0)
leg_ttbarV.SetFillColor(ROOT.kOrange+1)
leg_ttbarV.SetLineColor(ROOT.kOrange+1)

legend.AddEntry(leg_data,   'Data',      'p')
legend.AddEntry(leg_zjets,  'Z+jets',    'f')
legend.AddEntry(leg_diboson,'ZZ/WZ',     'f')
legend.AddEntry(leg_ttbar,  't#bar{t}',  'f')
legend.AddEntry(leg_ttbarV, 't#bar{t}V', 'f')

atlaslabel = ROOT.TLatex()
atlaslabel.SetNDC()
atlaslabel.SetTextFont(72)
atlaslabel.SetTextSize(0.065)
atlaslabelText = ROOT.TLatex()
atlaslabelText.SetNDC()
atlaslabelText.SetTextFont(42)
atlaslabelText.SetTextSize(0.065)

line_at_one = ROOT.TF1('line_at_one','1',-1e5,1e5)
line_at_one.SetLineColor(1)
line_at_one.SetLineStyle(7)
line_at_one.SetLineWidth(1)
