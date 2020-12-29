#!/bin/bash

if [ $# -ne 2 ]
  then echo "Usage $0 [inputFile] [numberOfLines]" && exit
fi

if [ ! -e $1 ]
  then echo "File '$1' does not exist, enter correct inputFile"
  exit
fi

tail -n $2 $1 > a
diff a predictions.csv | grep "^>" | wc -l > b


mistakes=( $(cat b) )

correct=0
let "correct= $2 - $mistakes"

percent=0
let "percent = $correct * 100 / $2"
echo "ACCURACY $percent%"
echo "$mistakes LINES WRONG OUT OF $2"

rm a b
