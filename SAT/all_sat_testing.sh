#!/bin/bash
# all_sat_testing.sh
# Evan Heaton
# 09/29/2016
# AI Satisfiability Programming Assignment
# testing shell script

if [ "$#" -lt 1 ]
then
  echo "Usage: $0 [easy/~hard]" >&2
  exit 1
fi

EASYNOTHARD=$1
FORMULANUMBER=1

while [ $FORMULANUMBER -lt 101 ]
do
  echo -e "-----------\t $FORMULANUMBER \t------------" >> sat_results.txt
  ./satisfiability $EASYNOTHARD $FORMULANUMBER >> sat_results.txt
  echo "" >> sat_results.txt
  echo "completed formula $FORMULANUMBER"
  let FORMULANUMBER=FORMULANUMBER+1
done
