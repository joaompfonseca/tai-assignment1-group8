# TAI Assignment 1 - Copy Model for Data Compression

## Description

This first assignment's goal was to explore the usage of copy models for data compression.

This repository contains the [report](report/report.pdf) and the two programs developed for this [assignment](description.pdf): `cpm`
and `mutate`.

**Course:** Algorithmic Theory of Information (2023/2024).

## The `mutate` program

The `mutate` program is a simple command-line tool that mutates symbols in a given input file according to a
probability. The program reads the input file and writes the mutated stream to an output file. The user can specify
the seed for the random number generator and the probability of mutation.

### Compile

It's optional, since you can use the provided executable inside the `bin` directory.

- Run `cd bin` in root.
- Run `g++ -Wall -O3 -o mutate ../src/mutate.cpp` to compile the program.

### Execute

- Run `cd bin` in root to change to the executable's directory.
- Run `./mutate REQUIRED OPTIONAL` to execute the program.

#### Required arguments

- `-i input_file_path`: path to the input file (string).
- `-o output_file_path`: path to the output file (string).

#### Optional arguments

- `-h`: shows how to use the program.
- `-p probability`: probability of mutation (double, default=0.0).
- `-s seed`: initialization for the random number generator (long or string, default=current_ts)

#### Example

`./mutate -i ../example/chry.txt -o ../example/chry_mod.txt -p 0.5 -s 1234`

## The `cpm` program

The `cpm` program is a command-line tool that estimates the total number of bits to encode a file, along with the average number of bits per symbol.
Initially, the program loads the input file to memory by performing a first pass through the text and storing the alphabet and the absolute frequency 
of each symbol. Then, it performs a second pass through the text with a reading pointer and at least one copy pointer.

### Compile

It's optional, since you can use the provided executable inside the `bin` directory.

- Run `cd bin` in root.
- Run `cmake .. && make` to compile the program.

### Execute

- Run `cd bin` in root to change to the executable's directory.
- Run `./cpm REQUIRED OPTIONAL` to execute the program.

#### Required arguments

- `-f input_file_path`: path to the input file (string).
- `-w window_size`: size of the sequences (kmers) from which the copy model will be enabled (int).
- `-n number_of_tries`: number of predicted symbols to consider when evaluating the miss rate (int).
- `-m misses_threshold`: maximum number of misses allowed in the last number_of_tries predictions (int).
- `-s smoothing_factor`: parameter to smooth the first probability estimation (double).
- `-l limit`: maximum number of simultaneous copy models (int).

#### Optional arguments

- `-h`: shows how to use the program.
- `-o output_file_path`: path to export the results (string).

#### Example

`./cpm -f ../example/chry.txt -w 15 -s 1.0 -m 7 -n 15 -l 1`

## Authors

- Diogo Paiva, 103183
- João Fonseca, 103154
- Rafael Gonçalves, 102534
