#!/usr/bin/env python2

import argparse
import sys
import json
import os
FRZ_BASE = str(os.environ['FRZ_BASE'])

parser = argparse.ArgumentParser(description='FRZ python controller')
parser.add_argument('-s','--swizzle',     help='flag to run Swizzler on a top mini ntuple',  required=False,action='store_true')
parser.add_argument('-d','--data',        help='flag to tell job if it is data',             required=False,action='store_true')
parser.add_argument(     '--pileup',      help='flag to tell job to consider pileup',        required=False,action='store_true')
parser.add_argument('-i','--in-file',     help='input file name',                            required=False)
parser.add_argument(     '--make-hists',  help='run the histogram maker',                    required=False,action='store_true')
parser.add_argument('-p','--third-pdg',   help='third lepton pdg, 11/13 (for e/mu)',         required=False,type=int,choices=[11,13],default=11)
parser.add_argument('-o','--out-file',    help='output file for histogram maker',            required=False)
parser.add_argument(     '--json-to-root',help='convert sample list from json to ROOT',      required=False,action='store_true')
parser.add_argument(     '--third-loose', help='third lepton loose flag (0 no,1 yes,2 both)',required=False,type=int,choices=[0,1,2],default=2)
parser.add_argument(     '--third-ratios',help='print ratio for the 3 third lepton pT bins', required=False,type=str)
parser.add_argument(     '--gen-plots',   help='executes plotting script on hist file',      required=False,type=str)
parser.add_argument(     '--mpl',         help='generate plots with matplotlib',             required=False,action='store_true')

args    = vars(parser.parse_args())
args_tf = parser.parse_args()

if len(sys.argv) < 2:
    parser.print_usage()
    sys.exit(1)

import ROOT
ROOT.gSystem.Load(FRZ_BASE+'/lib/libFRZ')
from ROOT import FRZ

if args_tf.swizzle or args_tf.json_to_root:
    sample_holder = FRZ.SampleHolder()
    data = json.load(open(FRZ_BASE+'/config/samples_mc12a_nominal.json','r'))
    for key1,val1 in data.iteritems():
        for key2,val2 in val1.iteritems():
            for key3,val3 in val2.iteritems():
                a_sample = FRZ.Sample()
                a_sample.set_processType(str(key1))
                a_sample.set_processGenerator(str(key2))
                a_sample.set_ID(int(key3))
                a_sample.set_mc_channel(int(key3))
                a_sample.set_crossSection(val3['crossSection'])
                a_sample.set_kFactor(val3['kFactor'])
                a_sample.set_nGenerated(val3['nGenerated'])
                a_sample.set_nGeneratedPileup(val3['nGeneratedPileup'])
                a_sample.set_nFiducial_elel(val3['nFiducial_elel'])
                a_sample.set_nFiducial_elmu(val3['nFiducial_elmu'])
                a_sample.set_nFiducial_mumu(val3['nFiducial_mumu'])
                a_sample.set_nGenerated_elel(val3['nGenerated_elel'])
                a_sample.set_nGenerated_elmu(val3['nGenerated_elmu'])
                a_sample.set_nGenerated_mumu(val3['nGenerated_mumu'])
                a_sample.set_nReconstructed_elel(val3['nReconstructed_elel'])
                a_sample.set_nReconstructed_elmu(val3['nReconstructed_elmu'])
                a_sample.set_nReconstructed_mumu(val3['nReconstructed_mumu'])
                a_sample.set_BR_elel(val3['BR_elel'])
                a_sample.set_BR_elmu(val3['BR_elmu'])
                a_sample.set_BR_mumu(val3['BR_mumu'])
                sample_holder.addSample(a_sample.ID(),a_sample)

    if args_tf.json_to_root:
        out_file    = ROOT.TFile('samples_mc12a_nominal.root','RECREATE')
        sample_tree = ROOT.TTree('sample_tree','sample_tree')
        sample_tree.Branch('sample_holder',sample_holder)
        sample_tree.Fill()
        sample_tree.Write()
        out_file.Close()

    if args_tf.swizzle:
        if args_tf.data:
            split_me   = args['in_file']
            parts_full = split_me.split('/')
            out_file   = split_me[len(split_me)-1]
            out_file   = 'swizzled.data.ML_LOOSEELE_LOOSEMU_3FF.root'
            out_file   = 'swizzled_files/'+out_file
            a_swizz    = FRZ.Swizzler(True,args['pileup'])
            a_swizz.addFile(args['in_file'])
            a_swizz.loopToFile(out_file,sample_holder)
        else:
            split_me   = args['in_file']
            parts_full = split_me.split('mc12_8TeV')
            out_file   = 'swizzled.mc12_8TeV'+parts_full[len(parts_full)-1]
            out_file   = out_file.replace('/','.')
            out_file   = 'swizzled_files/'+out_file
            a_swizz    = FRZ.Swizzler(False,args['pileup'])
            a_swizz.addFile(args['in_file'])
            a_swizz.loopToFile(out_file,sample_holder)

if args_tf.make_hists:
    file_names = os.listdir(FRZ_BASE+'/swizzled_files')
    hist_maker = FRZ.HistMaker()
    for f in file_names: hist_maker.addFileName(FRZ_BASE+'/swizzled_files/'+f)
    tl = int(args['third_pdg'])
    hist_maker.setThirdLepOpt(tl)
    of = 'hists.pdg_'+str(tl)+'.looseopt_'+str(args['third_loose'])+'.root'
    if hist_maker.run(of,args['third_loose']) == False:
        print 'Something bad happened in HistMaker::run, it returned false'

if args_tf.third_ratios:
    in_file    = ROOT.TFile(args['third_ratios'])
    ratio      = in_file.Get('tlptv_ratio')
    data       = in_file.Get('tlptv_data')
    stack_last = in_file.Get('stack_tlptv').GetStack().Last()
    nbins      = data.GetNbinsX()
    bin_left   = [ratio.GetXaxis().GetBinLowEdge(i+1) for i in xrange(nbins)]
    bin_right  = [ratio.GetXaxis().GetBinUpEdge(i+1)  for i in xrange(nbins)]
    bin_widths = [y-x for x,y in zip(bin_left,bin_right)]
    bin_mc     = [data.GetBinContent(i+1)/ratio.GetBinContent(i+1) for i in xrange(nbins)]
    bin_data   = [data.GetBinContent(i+1) for i in xrange(nbins)]
    bin_mc_err = [stack_last.GetBinError(i+1) for i in xrange(nbins)]
    bin_data_e = [data.GetBinError(i+1) for i in xrange(nbins)]
    bin_cont   = [ratio.GetBinContent(i+1) for i in xrange(nbins)]
    bin_err    = [ratio.GetBinError(i+1) for i in xrange(nbins)]
        
    print '\\begin{table}'
    print '\\caption{Trilepton events with a $Z\\rightarrow\\ell^+\\ell^-$ candidate. Third lepton (the lepton not in the pair forming the $Z$ candidate)',
    print '$p_T$ ranges corresponding to $\\sqrt{s}~=~8$~TeV and $\\int\\mathcal{L}\\,dt~=~20.3~\\text{ fb}^{-1}$.}'
    print '\\begin{tabular*}{\\textwidth}{@{\\extracolsep{\\fill}} l | c | c | c | c }'
    print '\\hline\hline'
    print '& $p_T$ range (GeV) & $N_{\\text{events}}$ Data & $N_{\\text{events}}$ MC & Ratio \\\ '
    print '\\hline'

    for i in xrange(nbins):
        print ' Bin '+str(i+1)+' &\t',bin_left[i],'--',bin_right[i],'\t& $', round(bin_data[i],2),'\\pm',round(bin_data_e[i],2),'$ \t& $',
        print round(bin_mc[i],2),'\\pm',round(bin_mc_err[i]),'$ \t& $',round(bin_cont[i],2),'\\pm',round(bin_err[i],2),'$\t \\\ '
    
    print '\\hline\hline'
    print '\\end{tabular*}'
    print '\\end{table}'

if args_tf.gen_plots:
    if args_tf.mpl:
        com = 'FRZ_plot_mpl.py '+args['gen_plots']
        os.system('mkdir -p '+str(args['gen_plots'])+'.plots.mpl')
        os.system(com)
    else:
        arg = args['gen_plots']+'.plots'
        com = 'FRZ_plot.py '+args['gen_plots']+' '+arg+' pdf'
        os.system(com)
