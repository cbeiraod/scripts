#!/bin/bash

if (( $# == 0 ))
then
echo "Wrong number of arguments"
  echo "Usage:"
  echo -e "\t"$0" dir"
  echo -e "\tdir - Directory created by createValidators.sh"
  exit 1
fi

for file in $1/*.sh; do
  qsub $file
done
