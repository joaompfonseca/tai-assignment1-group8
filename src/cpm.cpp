#include <iostream>
#include <unistd.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include "copymodel/CopyModel.h"
#include "copymodel/CopyModelRunner.h"
#include "reader/FileReader.h"

using namespace std;

struct ProgramArguments {
    string filename; // New field for filename
    unsigned int lookbackSize{}; // New field for loopback size
    unsigned int missesThreshold{}; // New field for misses threshold
    double smoothingFactor{}; // New field for smoothing factor
    unsigned int windowSize{}; // New field for window size
    unsigned int limit{}; // New field for limit
    string outputFilename; // New field for output filename

    // Overload the << operator to print the program arguments
    friend ostream &operator<<(ostream &os, const ProgramArguments &args) {
        os << "(-f) Filename: " << args.filename << endl;
        os << "(-n) Number of Tries: " << args.lookbackSize << endl;
        os << "(-m) Misses threshold: " << args.missesThreshold << endl;
        os << "(-s) Smoothing factor: " << args.smoothingFactor << endl;
        os << "(-w) Window size: " << args.windowSize << endl;
        os << "(-l) Limit: " << args.limit << endl;
        os << "(-o) Output filename: " << args.outputFilename << endl;
        return os;
    }
};

ProgramArguments getProgramArguments(int argc, char *argv[]) {
    ProgramArguments args;
    int opt;
    while ((opt = getopt(argc, argv, "f:n:m:s:w:l:o:h")) != -1) {
        switch (opt) {
            case 'f':
                args.filename = optarg;
                break;
            case 'n':
                try {
                    args.lookbackSize = stoul(optarg);
                } catch (const invalid_argument &e) {
                    cerr << "Error: Loopback Size (-ls) must be a integer." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'm':
                try {
                    args.missesThreshold = stoul(optarg);
                } catch (const invalid_argument &e) {
                    cerr << "Error: Misses Threshold (-m) must be a integer." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                try {
                    args.smoothingFactor = stod(optarg);
                } catch (const invalid_argument &e) {
                    cerr << "Error: Smoothing factor (-s) must be a double." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'w':
                try {
                    args.windowSize = stoul(optarg);
                } catch (const invalid_argument &e) {
                    cerr << "Error: Window size (-w) must be an unsigned integer." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'l':
                try {
                    args.limit = stoul(optarg);
                } catch (const invalid_argument &e) {
                    cerr << "Error: Limit (-l) must be an unsigned integer." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                args.outputFilename = optarg;
                break;
            case 'h':
                cout << "Usage: ./cpm -f filename -n loopbackSize -m missesThreshold -s smoothingFactor -w windowSize -l limitSize [-o outputFilename]" << endl
                     << "Options:" << endl
                     << " (-h) Help: shows how to use the program" << endl
                     << " (-f) Filename:  Path to the input file (string) (required)" << endl
                     << " (-n) Number of tries: Number of predicted symbols to consider when evaluating the copy model (int) (required)" << endl
                     << " (-m) Misses threshold: Maximum number of misses allowed in the lookback window (int) (required)" << endl
                     << " (-s) Smoothing factor: Parameter to smooth the first probability estimation (double) (required)" << endl
                     << " (-w) Window size: Size of the sequences (kmers) from which the copy model will be enabled (int) (required)" << endl
                     << " (-l) Limit: Maximum number of simultaneous copy models (int) (required)" << endl
                     << " (-o) Output filename: Path to export the results (string) (optional)" << endl;
                exit(EXIT_SUCCESS);
            case '?':
                if (optopt == 'f' || optopt == 'n' ||optopt == 'm' || optopt == 's' || optopt == 'w' || optopt == 'l' || optopt == 'o') {
                    cerr << "Option -" << static_cast<char>(optopt) << " requires an argument." << endl;
                } else {
                    cerr << "Unknown option -" << static_cast<char>(optopt) << endl;
                }
                exit(EXIT_FAILURE);
            default:
                exit(EXIT_FAILURE);
        }
    }
    // Ensure that mandatory arguments are provided
    if (args.filename.empty()) {
        cerr << "Error: Filename (-f) is required." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.lookbackSize <= 0) {
        cerr << "Error: Number of Tries (-n) must be an unsigned integer greater than 0." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.missesThreshold <= 0) {
        cerr << "Error: Misses threshold (-m) must be an unsigned integer greater than 0." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.smoothingFactor < 0) {
        cerr << "Error: Smoothing factor (-s) must be a double greater than or equal to 0." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.windowSize <= 0) {
        cerr << "Error: Window size (-w) must be an unsigned integer greater than 0." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.limit <= 0) {
        cerr << "Error: Limit (-l) must be an unsigned integer greater than 0." << endl;
        exit(EXIT_FAILURE);
    }
    return args;
}

// Arguments to be passed: <filename> <threshold> <smoothingFactor> <windowSize> <global_metrics>
int main(int argc, char *argv[]) {

    // parse program arguments
    ProgramArguments programArguments = getProgramArguments(argc, argv);

    // print program configuration
    cout << "Program configuration:" << endl
         << programArguments << endl;

    // read file
    FileReader fileReader = FileReader(programArguments.filename);
    fileReader.read();

    // Get content and alphabet
    string content = fileReader.getContent();
    vector<char> alphabet = fileReader.getAlphabet();

    // Create a copy model runner
    CopyModelRunner copyModelRunner = CopyModelRunner(content, alphabet, programArguments.lookbackSize, programArguments.missesThreshold, programArguments.smoothingFactor, programArguments.windowSize, programArguments.limit);

    // Time the execution
    clock_t start = clock();

    while (copyModelRunner.hasNext()) {
        copyModelRunner.runStep();
    }
    copyModelRunner.addRemainingBits();

    clock_t end = clock();

    double result = copyModelRunner.estimatedNumberOfBits / content.size();

    cout << "Estimated number of bits per symbol (bps): " << result << endl;

    cout << "Cumulative number of bits: " << copyModelRunner.estimatedNumberOfBits << endl;

    double executionTime = (double) (end - start) / CLOCKS_PER_SEC;

    cout << "Execution time: " << executionTime << " seconds" << endl;

    if (programArguments.outputFilename.empty()) {
        return 0;
    } else {
        // Write to csv
        ofstream outputFile(programArguments.outputFilename, ios::app);
        if (!outputFile.is_open()) {
            cerr << "Error: Unable to open output file." << endl;
            return EXIT_FAILURE;
        }

        // Write the header if the file is empty
        if (outputFile.tellp() == 0) {
            outputFile << "lookback_size,misses_threshold,alpha,window,limit,bps,totalb,time" << endl;
        }

        // Append the result to the output file
        outputFile << programArguments.lookbackSize << "," << programArguments.missesThreshold << "," << programArguments.smoothingFactor << "," << programArguments.windowSize << "," << programArguments.limit << "," << result << "," << copyModelRunner.estimatedNumberOfBits << "," << executionTime << endl;

        outputFile.close();

        return 0;
    }
}
