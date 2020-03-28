#!/bin/bash

if [[ $# < 1 ]]
then
    echo "Unspecified duration of tests"
    exit 1
fi

./run_benchmark.sh breakthrough $1 $2
./run_benchmark.sh connect4 $1 $2
./run_benchmark.sh english_draughts $1 $2
./run_benchmark.sh reversi $1 $2
