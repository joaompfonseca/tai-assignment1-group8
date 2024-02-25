#include <iostream>
#include <unistd.h> // for getopt
#include <string>
#include <random>
#include <unordered_set>
#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <set>

using namespace std;

struct ProgramArguments {
    string inputFilePath, outputFilePath;
    unsigned int seed;
    map<char, double> symbolProbabilities;

    // overload the << operator to print the program arguments
    friend ostream &operator<<(ostream &os, const ProgramArguments &args) {
        os << "(-i) Input file path: " << args.inputFilePath << endl;
        os << "(-o) Output file path: " << args.outputFilePath << endl;
        os << "(-s) Seed: " << args.seed << endl;
        os << "(-p) Symbol probabilities: ";
        auto it = args.symbolProbabilities.begin();
        os << it->first << " -> " << it->second;
        for (it++; it != args.symbolProbabilities.end(); it++) {
            os << ", " << it->first << " -> " << it->second;
        }
        return os;
    }
};

ProgramArguments getProgramArguments(int argc, char *argv[]) {
    ProgramArguments args;
    // set default seed to the current time
    args.seed = time(nullptr);
    int opt;
    while ((opt = getopt(argc, argv, "i:o:s:p:h")) != -1) {
        switch (opt) {
            case 'i':
                args.inputFilePath = optarg;
                break;
            case 'o':
                args.outputFilePath = optarg;
                break;
            case 's':
                args.seed = stoul(optarg);
                break;
            case 'p': {
                char symbol = optarg[0];
                double probability = stod(argv[optind]);
                args.symbolProbabilities[symbol] = probability;
                optind++;
            }
                break;
            case 'h':
                cout
                        << "Usage: mutate -i <string:input file path> [-o <string:output file path>] [-s <uint:seed>] [-p <char:symbol> <double:probability> ...]"
                        << endl;
                exit(0);
            case '?':
                if (optopt == 'i' || optopt == 'o' || optopt == 's' || optopt == 'p') {
                    cerr << "Option -" << static_cast<char>(optopt) << " requires an argument." << endl;
                } else {
                    cerr << "Unknown option -" << static_cast<char>(optopt) << endl;
                }
                exit(1);
            default:
                exit(1);
        }
    }
    // ensure that input file path is provided
    if (args.inputFilePath.empty()) {
        cerr << "Error: Input (-i) file path is required." << endl;
        exit(1);
    }
    return args;
}

set<char> getAlphabet(const string &inputFilePath) {
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    }
    // read every char from the input file and add it to the alphabet
    set<char> alphabet;
    char c;
    while (inputFile.get(c)) {
        alphabet.insert(c);
    }
    return alphabet;
}

void setSeed(unsigned int seed) {
    srand(seed);
}

double getRandomDouble01() {
    return (rand() / (double) RAND_MAX);
}

int main(int argc, char *argv[]) {
    // parse program arguments
    ProgramArguments programArgs = getProgramArguments(argc, argv);

    // print program configuration
    cout << "<Program configuration>" << endl << programArgs << endl;

    // read input file to get the alphabet
    set<char> alphabet = getAlphabet(programArgs.inputFilePath);

    // print the alphabet
    cout << "<Alphabet>" << endl;
    for (char c : alphabet) {
        cout << c << " ";
    }
    cout << endl;

    // read input file again and mutate the stream to the output file
    ifstream inputFile(programArgs.inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    ofstream outputFile(programArgs.outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    // if alphabet has at most one symbol, just copy the input file to the output file
    if (alphabet.size() <= 1) {
        outputFile << inputFile.rdbuf();
        return 0;
    }

    // set the seed for the random number generator
    setSeed(programArgs.seed);

    char c;
    while (inputFile.get(c)) {
        // if the symbol does not have a probability, write it to the output file
        if (programArgs.symbolProbabilities.find(c) == programArgs.symbolProbabilities.end()) {
            outputFile << c;
            continue;
        }
        // get the probability of the symbol
        double probability = programArgs.symbolProbabilities[c];
        // mutate the char based on the probability
        if (probability > getRandomDouble01()) {
            // get a random symbol from the alphabet
            int index = (int) (getRandomDouble01() * (double) alphabet.size());
            auto it = alphabet.begin();
            advance(it, index);
            outputFile << *it;
        } else {
            outputFile << c;
        }
    }
    return 0;
}