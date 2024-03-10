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
    double threshold{}; // New field for threshold
    double smoothingFactor{}; // New field for smoothing factor
    unsigned int windowSize{}; // New field for window size
    bool globalMetrics{}; // New field for global metrics (assuming it's a boolean flag)

    // Overload the << operator to print the program arguments
    friend ostream &operator<<(ostream &os, const ProgramArguments &args) {
        os << "(-f) Filename: " << args.filename << endl;
        os << "(-t) Threshold: " << args.threshold << endl;
        os << "(-s) Smoothing factor: " << args.smoothingFactor << endl;
        os << "(-w) Window size: " << args.windowSize << endl;
        os << "(-g) Global metrics: " << (args.globalMetrics ? "true" : "false") << endl;
        return os;
    }
};

ProgramArguments getProgramArguments(int argc, char *argv[]) {
    ProgramArguments args;
    int opt;
    while ((opt = getopt(argc, argv, "f:t:s:w:g:h")) != -1) {
        switch (opt) {
            case 'f':
                args.filename = optarg;
                break;
            case 't':
                try {
                    args.threshold = stod(optarg);
                } catch (const invalid_argument &e) {
                    cerr << "Error: Threshold (-t) must be a double." << endl;
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
            case 'g':
                try {
                    string temp = optarg;
                    if (temp == "true") {
                        args.globalMetrics = true;
                    } else if (temp == "false") {
                        args.globalMetrics = false;
                    } else {
                        throw invalid_argument("");
                    }
                } catch (const invalid_argument &e) {
                    cerr << "Error: Global metrics (-g) must be a boolean." << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                cout << "Usage: ./cpm -f filename -t threshold -s smoothing_factor -w window_size -g" << endl
                     << "Options:" << endl
                     << " (-h) Help: shows how to use the program" << endl
                     << " (-f) Filename: filename (required)" << endl
                     << " (-t) Threshold: threshold value (required)" << endl
                     << " (-s) Smoothing factor: smoothing factor (required)" << endl
                     << " (-w) Window size: window size (required)" << endl
                     << " (-g) Global metrics: true or false (required)" << endl;
                exit(EXIT_SUCCESS);
            case '?':
                if (optopt == 'f' || optopt == 't' || optopt == 's' || optopt == 'w' || optopt == 'g') {
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
    if (args.threshold < 0 || args.threshold > 1) {
        cerr << "Error: Threshold (-t) must be a double in the range [0, 1]." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.smoothingFactor < 0) {
        cerr << "Error: Smoothing factor (-s) must be a double greater than or equal to 0." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.windowSize == 0) {
        cerr << "Error: Window size (-w) must be an unsigned integer greater than 0." << endl;
        exit(EXIT_FAILURE);
    }
    if (args.globalMetrics) {
        cout << "Global metrics enabled." << endl;
    }
    return args;
}

// Arguments to be passed: <filename> <threshold> <smoothing_factor> <window_size> <global_metrics>
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
    CopyModelRunner copyModelRunner = CopyModelRunner(content, alphabet, programArguments.threshold, programArguments.smoothingFactor, programArguments.windowSize, programArguments.globalMetrics);

    while (copyModelRunner.has_next()) {
        copyModelRunner.run_step();
    }

    return 0;

}
