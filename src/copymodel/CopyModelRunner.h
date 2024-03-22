//
// Created by diogopaiva21 on 10-03-2024.
//

#ifndef TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODELRUNNER_H
#define TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODELRUNNER_H

#include <string>
#include <map>
#include "CopyModel.h"

using namespace std;

class CopyModelRunner {
public:
    CopyModelRunner(string stream, vector<char> alphabet , double threshold, double smoothing_factor, int window_size, bool global_metrics = false);

    bool has_next() const;
    void run_step();
    double estimated_number_of_bits;

private:
    string stream;
    int stream_size;
    double threshold;
    double smoothing_factor;
    int window_size;
    bool global_metrics;
    map<char, int> counts;
    map<string,CopyModel> sequence_map;
    int ptr;
    int hits;
    int misses;
    vector<char> alphabet;
};


#endif //TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODELRUNNER_H
