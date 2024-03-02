# TAI Assignment 1 - Copy Model for Data Compression

## Description

...

**Course:** Algorithmic Theory of Information (2023/2024).

## Installation

- ...
- ...

## The `mutate` program

The `mutate` program is a simple command-line tool that mutates symbols in a given input file according to a
probability. The program reads the input file and writes the mutated stream to an output file. The user can specify
the seed for the random number generator and the probability of mutation.

### Usage

```
$ ./mutate \
-i input_file_path \
-o output_file_path \
[-p probability] \
[-s seed]
```

### Options

```
(-h) Help: shows how to use the program
(-i) Input: path to the input file (required)
- Example: -i input.txt
(-o) Output: path to the output file (required)
- Example: -o output.txt
(-p) Probability: probability of mutation (default == 0.0)
- Example: -p 0.5
(-s) Seed: initialization for the random number generator (default == current time)
- Example: -s 1234
```

### Example for testing

```
$ ./mutate -i ../example/chry_test.txt -o ../example/chry_test_mod.txt -p 0.5 -s 1234
```

## Authors

- Diogo Paiva, 103183
- João Fonseca, 103154
- Rafael Gonçalves, 102534
