#!/bin/bash

if [[ $# < 2 ]]
then
    echo "Incorrect number of parameters"
    exit 1
fi

if [[ ! -d logs ]]
then
    mkdir logs
fi

if [[ $3 == 1 ]] || [[ $3 == 2 ]]; then
randgen=$3
else
randgen=0
fi
echo "Running benchmark for $1 with time $2 randgen $randgen"
rm -f $1
make $1 RANDGEN=$randgen
taskset -c 0 ./$1 $2 > logs/$1-benchmark.txt
echo "Results: logs/$1-benchmark.txt"