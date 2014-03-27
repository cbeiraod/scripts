#!/bin/bash

if (( $# == 0 ))
then
  echo "Wrong number of arguments"
  echo "Usage:"
  echo -e "\t"$0" dir"
  echo -e "\tdir - Directory that holds LHE files"
  exit 1
fi

: ${CMSSW_BASE?"Must be in a CMSSW environment"}

if [ ! -d $CMSSW_BASE/src/Configuration/Generator/ ];
then
  echo "You did not checkout the Configuration/Generator module."
  echo "Please check it out and try again"
  exit 1
fi

STARTDIR=$PWD

cp myGenFragment_cfi.py $CMSSW_BASE/src/Configuration/Generator/python/

cd $CMSSW_BASE/src
scram b clean
scram b -j 8
cd $STARTDIR

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

BASE_CMD="cmsDriver.py myGenFragment_cfi.py --mc --no_exec"
CMSDRIVER_OPT="--eventcontent AODSIM --datatier AODSIM --pileup 2012_Summer_inTimeOnly --beamspot Realistic8TeVCollision"
BASESTEPS="--step GEN,FASTSIM"
EVENTS_PER_JOB=2000
REMOTE_DIR_PRE="/TStauStau"

for file in $1/*.lhe; do
  BASE=$(basename $file)
  BASE=${BASE%.*}

  NUM_EVTS=$(cat $file | grep '<event>' | wc -l)
  NJOBS=$(expr $NUM_EVTS / $EVENTS_PER_JOB)
  if [ $(expr $NJOBS \* $EVENTS_PER_JOB) != $NUM_EVTS ];
  then
    NJOBS=$(expr $NJOBS + 1)
  fi

  FILEIN="--filein=file:$file"
  PYTHON_FILE="--python_filename $BASE.py"


  ##### HLT 5E33v4
  cd $STARTDIR/5E33

  FILEOUT="--fileout "$BASE"_5E33v4.root"
  STEPS="$BASESTEPS,HLT:5E33v4"
  CONDITIONS="--conditions auto:startup_5E33v4"

  FINAL_CMD="$BASE_CMD -n $NUM_EVTS $FILEIN $PYTHON_FILE $FILEOUT $STEPS $CONDITIONS $CMSDRIVER_OPT"
  echo $FINAL_CMD
  eval $FINAL_CMD

  TEMP_CMD="cp $STARTDIR/templates/crab_template.cfg ./"$BASE".cfg"
  eval $TEMP_CMD

  sed -i 's/{CMSSW_PY_FILE}/'$BASE'.py/' ./$BASE.cfg
  sed -i 's/{EVTS_PER_JOB}/'$EVENTS_PER_JOB'/' ./$BASE.cfg
  sed -i 's/{NJOBS}/'$NJOBS'/' ./$BASE.cfg
  sed -i 's:{REMOTE_DIR}:'$REMOTE_DIR_PRE'/'$BASE'_5E33/:' ./$BASE.cfg
  sed -i 's/{WORKING_DIR}/'$BASE'/' ./$BASE.cfg

  TEMP=$(sed -nr 's/.*\{([a-zA-Z_]+)\}.*/\1/p' ./$BASE.cfg)
  if [ "$TEMP" != "" ];
  then
    echo "Not all crab variables have been processed: "
    echo -e "\t"$TEMP
    exit 1
  fi


  ##### HLT 8E33v2
  cd $STARTDIR/8E33

  FILEOUT="--fileout "$BASE"_8E33v2.root"
  STEPS="$BASESTEPS,HLT:8E33v2"
  CONDITIONS="--conditions auto:startup_8E33v2"

  FINAL_CMD="$BASE_CMD -n $NUM_EVTS $FILEIN $PYTHON_FILE $FILEOUT $STEPS $CONDITIONS $CMSDRIVER_OPT"
  echo $FINAL_CMD
  eval $FINAL_CMD

  TEMP_CMD="cp $STARTDIR/templates/crab_template.cfg ./"$BASE".cfg"
  eval $TEMP_CMD

  sed -i 's/{CMSSW_PY_FILE}/'$BASE'.py/' ./$BASE.cfg
  sed -i 's/{EVTS_PER_JOB}/'$EVENTS_PER_JOB'/' ./$BASE.cfg
  sed -i 's/{NJOBS}/'$NJOBS'/' ./$BASE.cfg
  sed -i 's:{REMOTE_DIR}:'$REMOTE_DIR_PRE'/'$BASE'_8E33/:' ./$BASE.cfg
  sed -i 's/{WORKING_DIR}/'$BASE'/' ./$BASE.cfg

  TEMP=$(sed -nr 's/.*\{([a-zA-Z_]+)\}.*/\1/p' ./$BASE.cfg)
  if [ "$TEMP" != "" ];
  then
    echo "Not all crab variables have been processed: "
    echo -e "\t"$TEMP
    exit 1
  fi
done

rm -f $CMSSW_BASE/src/Configuration/Generator/python/myGenFragment_cfi.py
cd $STARTDIR
