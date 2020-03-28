#!/bin/bash

mkdir logs

if [[ $3 == 1 ]] || [[ $3 == 2 ]]; then
randgen=$3
else
randgen=0
fi
echo "Running benchmark for $1 with time $2 randgen $randgen"
rm -f $1
make $1 RANDGEN=$randgen
./$1 $2 > logs/$1-benchmark.txt
echo "Results: logs/$1-benchmark.txt"