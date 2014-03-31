#!/bin/bash

if (( $# == 0 ))
then
echo "Wrong number of arguments"
  echo "Usage:"
  echo -e "\t"$0" dir"
  echo -e "\tdir - Directory that holds several subdirectories, each with several root files to be processed together"
  exit 1
fi

: ${CMSSW_BASE?"Must be in a CMSSW environment"}

STARTDIR=$(pwd -P)

cd $CMSSW_BASE/src
#scram b clean
#scram b -j 8
cd $STARTDIR

BaseFolder=$(echo $1 | sed 's://:/:')
BaseFolder=$(basename $BaseFolder)

mkdir $BaseFolder

for folder in $1/*; do
  if [ ! -d $folder ]; then
    echo "Not a folder, continuing"
    continue
  fi
  folder=$(echo $folder | sed 's://:/:')
  echo $folder

  if [ $(ls $folder/*.root | wc -l) == 0 ]; then
    echo "No contents in folder, continuing"
    continue
  fi

  subfolderName=$(basename $folder)
  cfgFile=$BaseFolder/$subfolderName"_cfg.py"
  var="cp ../myvalidator_cfg.py $cfgFile"
  eval $var

  files=""
  for file in $folder/*.root; do
    file=$(echo $file | sed 's://:/:')
    files=$files'"file:'$file'",\n'
  done

  sed -i 's:Outfile.root:'$subfolderName'_Validation.root:' $cfgFile
  sed -i 's-"file:/lustre/ncg.ingrid.pt/cmst3/store/user/cbeiraod/TStauStau/TStauStau_100_0_8E33/TStauStau_100_0_8E33v2_1_1_YIn.root",-'$files'-' $cfgFile


  scriptFile=$BaseFolder/$subfolderName".sh"

  echo "#!/bin/bash" > $scriptFile
  chmod +x $scriptFile
  echo "cd "$HOME"/software-area" >> $scriptFile
  echo ". setup.sh" >> $scriptFile
  echo "cd "$CMSSW_BASE"/src" >> $scriptFile
  echo "cmsenv" >> $scriptFile
  echo "cd "$STARTDIR"/"$BaseFolder >> $scriptFile
  echo "cmsRun "$subfolderName"_cfg.py 2>&1 | tee "$subfolderName".log" >> $scriptFile
done
