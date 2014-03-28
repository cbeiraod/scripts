#!/bin/bash

. /exper-sw/cmst3/cmssw/users/cbeiraod/setup.sh
cd /exper-sw/cmst3/cmssw/users/cbeiraod/Stau_Gen/slc5/CMSSW_5_3_14/src/
#cmsenv
eval `scramv1 runtime -sh`
cd /exper-sw/cmst3/cmssw/users/cbeiraod/Stau_Gen/slc5/CMSSW_5_3_14/src/Stau_Gen
./createCRABcfg.sh /lustre/ncg.ingrid.pt/cmslocal/cbeiraod/TStauStau/ > createCRABcfg.log
