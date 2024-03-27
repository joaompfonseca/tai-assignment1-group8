# TAI Assignment 1 - Copy Model for Data Compression

## Description

This first assignment's goal was to explore the usage of copy models for data compression.

This repository contains the [report](report/report.pdf) and the two programs developed for this [assignment](description.pdf): `cpm`
and `mutate`.

**Course:** Algorithmic Theory of Information (2023/2024).

## The `cpm` program

...

### Usage

...

### Options

...

### Example

...

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

## Authors

- Diogo Paiva, 103183
- João Fonseca, 103154
- Rafael Gonçalves, 102534
