#!/usr/bin/env python

print "Parsing options"
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-d","--datfile",dest="datfile")
parser.add_option("--dryRun",dest="dryRun",action="store_true",default=False)
(options,args)=parser.parse_args()

import ROOT

print "Loading file and setting branches"
oldfile = ROOT.TFile("eos/cms/store/group/phys_higgs/cmshgg/reduced/moriond2013_reduction_v2/data/Photon_Run2012A-13Jul2012-v1/Photon_Run2012A-13Jul2012-v1_0.root", "READ")
oldtree = oldfile.Get("event")

oldtree.SetBranchStatus("*", 0)

df = open(options.datfile)
for line in df.readlines():
  #line = line.encode("ASCII", 'ignore')
  oldtree.SetBranchStatus(line[:-1], 1)
#oldtree.SetBranchStatus("bx", 1)

print "Copying to new file"
newfile = ROOT.TFile("test.root", "RECREATE")
newtree = oldtree.CloneTree()

newfile.Write()
