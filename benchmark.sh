#!/bin/bash

# Compile the main program
g++ -Wall -O3 src/cpm.cpp src/reader/FileReader.cpp src/reader/FileReader.h src/copymodel/CopyModel.cpp src/copymodel/CopyModel.h src/copymodel/CopyModelRunner.h src/copymodel/CopyModelRunner.cpp -o cpm

# Compile the mutation program
g++ -Wall -O3 src/mutate/main.cpp -o mutate

# If directory does not exist, create it
if [ ! -d "results" ]; then
    mkdir results
fi

# Default parameters
./cpm -f example/chry.txt -t 0.2 -s 1 -w 15 -l 1 -o results/default.csv

# Variable threshold
for ((i=1; i<=6; i++)); do
    val=$(bc <<< "scale=1; $i / 10")
    ./cpm -f example/chry.txt -t "$val" -s 1 -w 15 -l 1 -o results/threshold.csv
done

# Variable window size
for ((i=5; i<=20; i++)); do
    ./cpm -f example/chry.txt -t 0.2 -s 1 -w "$i" -l 1 -o results/window.csv
done

# Variable smoothing factor
for ((i=1; i<=10; i++)); do
    val=$(bc <<< "scale=1; $i / 10")
    ./cpm -f example/chry.txt -t 0.2 -s "$val" -w 15 -l 1 -o results/alpha.csv
done

# Variable limit
for ((i=1; i<=20; i++)); do
    ./cpm -f example/chry.txt -t 0.2 -s 1 -w 15 -l "$i" -o results/limit.csv
done

# Mutate the file
for ((i=1; i<=9; i++)); do
    val=$(bc <<< "scale=1; $i / 10")
    ./mutate -i example/chry.txt -o example/chry_0"$val".txt -p "$val"
    ./cpm -f example/chry_0"$val".txt -t 0.2 -s 1 -w 15 -l 1 -o results/mutate_0"$val".csv
done