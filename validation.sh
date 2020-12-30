#!/bin/bash

if [ $# -ne 0 ]
  then echo "Usage $0 " && exit
fi

diff testing.csv predictions.csv | grep "^>" | wc -l > b

noOfLines=( $(wc -l predictions.csv))
mistakes=( $(cat b) )


let "correct= $noOfLines - $mistakes"


let "percent = $correct * 100 / $noOfLines"
echo "ACCURACY $percent%"
echo "$mistakes LINES WRONG OUT OF $noOfLines"

rm b
