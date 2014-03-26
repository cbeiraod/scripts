#!/bin/bash

if (( $# == 0 ))
then
  echo "Wrong number of arguments"
  echo "Usage:"
  echo -e "\t"$0" dir"
  echo -e "\tdir - Directory that holds LHE files"
  exit 0
fi

STARTDIR=$PWD

if [ -d ./5E33 ];
then
  rm -Rf ./5E33
fi
mkdir ./5E33

if [ -d ./8E33 ];
then
  rm -Rf ./8E33
fi
mkdir ./8E33

numFiles=$(ls $1/*.lhe | wc -l)
echo "Found $numFiles LHE files"

for file in $1/*.lhe; do
  BASE_CMD="cmsDriver.py myGenFragment_cfi.py --mc --no_exec"
#--eventcontent AODSIM
#--pileup 2012_Summer_inTimeOnly
#--datatier AODSIM
#--conditions START53_V19::All
#--beamspot Realistic8TeVCollision
#--step GEN,FASTSIM,HLT:5E33v4
#--filein=file:/lustre/ncg.ingrid.pt/cmslocal/cbeiraod/TStauStau/TStauStau_50_0.lhe
#--fileout TStauStau_50_0_GEN_FASTSIM_HLT_PU.root
#--python_filename TStauStau_50_0_GEN_FASTSIM_HLT_PU.py
#-n 10
  BASE=$(basename $file)
  BASE=${BASE%.*}
  echo $BASE
done
