#include <iostream>
#include <unistd.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <set>

using namespace std;

struct ProgramArguments {
    string inputFilePath, outputFilePath;
    double probability{};
    unsigned int seed{};

    // overload the << operator to print the program arguments
    friend ostream &operator<<(ostream &os, const ProgramArguments &args) {
        os << "(-i) Input file path: " << args.inputFilePath << endl;
        os << "(-o) Output file path: " << args.outputFilePath << endl;
        os << "(-p) Mutation probability: " << args.probability << endl;
        os << "(-s) Seed: " << args.seed;
        return os;
    }
};

ProgramArguments getProgramArguments(int argc, char *argv[]) {
    ProgramArguments args;
    args.seed = (unsigned int) time(nullptr); // default seed is the current time
    int opt;
    while ((opt = getopt(argc, argv, "i:o:p:s:h")) != -1) {
        switch (opt) {
            case 'i':
                args.inputFilePath = optarg;
                break;
            case 'o':
                args.outputFilePath = optarg;
                break;
            case 'p': {
                try {
                    args.probability = stod(optarg);
                    if (args.probability < 0 || args.probability > 1) {
                        throw invalid_argument("");
                    }
                } catch (const invalid_argument &e) {
                    cerr << "Error: Probability (-p) must be a double in the range [0, 1]." << endl;
                    exit(EXIT_FAILURE);
                }
            }
                break;
            case 's':
                try {
                    args.seed = stoul(optarg);
                } catch (const invalid_argument &e) {
                    args.seed = hash<string>{}(optarg);
                }
                break;
            case 'h':
                cout << "Usage: ./mutate -i input_file_path -o output_file_path [-p probability] [-s seed]" << endl
                     << "Options:" << endl
                     << "  (-h) Help: shows how to use the program" << endl
                     << "  (-i) Input: path to the input file (required)" << endl
                     << "  (-o) Output: path to the output file (required)" << endl
                     << "  (-p) Probability: probability of mutation (default == 0.0)" << endl
                     << "  (-s) Seed: initialization for the random number generator (default == current time)" << endl;
                exit(EXIT_SUCCESS);
            case '?':
                if (optopt == 'i' || optopt == 'o' || optopt == 'p' || optopt == 's') {
                    cerr << "Option -" << static_cast<char>(optopt) << " requires an argument." << endl;
                } else {
                    cerr << "Unknown option -" << static_cast<char>(optopt) << endl;
                }
                exit(EXIT_FAILURE);
            default:
                exit(EXIT_FAILURE);
        }
    }
    // ensure that mandatory arguments are provided
    if (args.inputFilePath.empty()) {
        cerr << "Error: Input (-i) file path is required." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.outputFilePath.empty()) {
        cerr << "Error: Output (-o) file path is required." << endl;
        exit(EXIT_FAILURE);
    }
    return args;
}

set<char> getAlphabet(const string &inputFilePath) {
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        exit(EXIT_FAILURE);
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
    ProgramArguments programArguments = getProgramArguments(argc, argv);

    // print program configuration
    cout << "Program configuration:" << endl
         << programArguments << endl;

    // read input file to get the alphabet
    set<char> alphabet = getAlphabet(programArguments.inputFilePath);

    // print the alphabet
    cout << "Alphabet:" << endl;
    for (char c: alphabet) {
        cout << c << " ";
    }
    cout << endl;

    // read input file again and mutate the stream to the output file
    ifstream inputFile(programArguments.inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return EXIT_FAILURE;
    }

    ofstream outputFile(programArguments.outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return EXIT_FAILURE;
    }

    // if alphabet has at most one symbol, just copy the input file to the output file
    if (alphabet.size() <= 1) {
        outputFile << inputFile.rdbuf();
        return EXIT_SUCCESS;
    }

    // set the seed for the random number generator
    setSeed(programArguments.seed);

    char c;
    while (inputFile.get(c)) {
        // mutate the char based on the probability
        if (getRandomDouble01() < programArguments.probability) {
            // get a random symbol from the alphabet
            int index = (int) (getRandomDouble01() * (double) alphabet.size());
            auto it = alphabet.begin();
            advance(it, index);
            outputFile << *it;
        } else {
            outputFile << c;
        }
    }
    return EXIT_SUCCESS;
}