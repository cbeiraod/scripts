#!/bin/bash

if (( $# == 0 ))
then
echo "Wrong number of arguments"
  echo "Usage:"
  echo -e "\t"$0" dir"
  echo -e "\tdir - Directory created by createValidators.sh, where the root files are (must have already submitted the jobs)"
  exit 1
fi

if [ -d $1/plots ]; then
  rm -Rf $1/plots
fi
mkdir $1/plots

for file in $1/*.root; do
#  file=$(echo $file | sed 's://:/:')
  BASENAME=$(basename $file)
#  BASENAME=${BASENAME%.*}
  BASENAME=${BASENAME%_Val*}

  var="root -b -q 'exportPlots.cpp+(\"$1/plots\", \"$file\", \"$BASENAME\")'"
  eval $var
#  echo $file
#  echo $BASENAME

  var="tac < $1/$BASENAME.log | sed '0,/========/d' | sed '/Closed file/,\$d' | tac > "$1"/plots/"$BASENAME"_HLT.txt"
  eval $var
  var="convert -font \"/usr/share/fonts/bitstream-vera/VeraMono.ttf\" label:@- "$1"/plots/"$BASENAME"_HLT.png < "$1"/plots/"$BASENAME"_HLT.txt"
  eval $var
done
