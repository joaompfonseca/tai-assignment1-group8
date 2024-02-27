# TAI Assignment 1 - Copy Model for Data Compression

## Description

...

**Course:** Algorithmic Theory of Information (2023/2024).

## Installation

- ...
- ...

## The `mutate` program

The `mutate` program is a simple command-line tool that mutates a given input file according to a set of probabilities. The program takes as input a file and a set of probabilities for each symbol in the file. The program then mutates the input file according to the given probabilities and writes the result to an output file. The program also takes an optional seed for the random number generator.

### Usage

```
$ ./mutate \
-i <string:input file path> \
[-o <string:output file path>] \
[-s <uint:seed>] \
[-p <char:symbol> <double:probability> ...]
```

### Options

```
(-h) Help: Shows how to use the program.
(-i) Input: Path to the input file (required).
- Example: -i input.txt
(-o) Output: Path to the output file.
- Example: -o output.txt
(-s) Seed: Integer seed for the random number generator.
- Example: -s 1234
(-p) Probability: Probability of a symbol being mutated.
- Example: -p A 0.5 -p C 0.9
```

### Example for testing

```
$ ./mutate -i ../example/chry_test.txt -o ../example/chry_test_mod.txt -s 1234 -p A 0.5 -p C 0.9
```

## Authors

- Diogo Paiva, 103183
- João Fonseca, 103154
- Rafael Gonçalves, 102534
