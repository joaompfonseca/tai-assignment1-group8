//
// Created by diogopaiva21 on 10-03-2024.
//

#include <iostream>
#include <cmath>
#include "CopyModelRunner.h"

using namespace std;

CopyModelRunner::CopyModelRunner(string stream, vector<char> alphabet, double threshold, double smoothing_factor, int window_size, bool global_metrics) {
    this->stream = stream;
    this->stream_size = stream.size();
    this->alphabet = alphabet;
    this->threshold = threshold;
    this->smoothing_factor = smoothing_factor;
    this->window_size = window_size;
    this->global_metrics = global_metrics;
    this->sequence_map = map<string, CopyModel>();
    this->estimated_number_of_bits = 0;
    this->ptr = 0;

    for (char c : stream) {
        if (counts.find(c) == counts.end()) {
            counts[c] = 1;
        } else {
            counts[c]++;
        }
    }

    if (global_metrics) {
        this->hits = 0;
        this->misses = 0;
    }
}

bool CopyModelRunner::has_next() const {
    return ptr < stream_size;
}

void CopyModelRunner::run_step() {
    string sequence = ptr - window_size < 0 ? string(window_size - ptr, alphabet[0]) + stream.substr(0, ptr) : stream.substr(ptr - window_size, window_size);
    cout << "Sequence: " << sequence << endl;
    char actual_char = stream[ptr];

    if (sequence_map.empty() || sequence_map.find(sequence) != sequence_map.end()) {

        char pred_char;
        CopyModel copy_model;

        if(!sequence_map.empty()) {
            copy_model = sequence_map[sequence];
            cout << "Reference: " << copy_model.reference << endl;
            pred_char = stream[copy_model.reference];
        } else {
            copy_model = CopyModel();
            sequence_map[sequence] = copy_model;
            pred_char = alphabet[0];
        }

        cout << "Predicted: " << pred_char << " Actual: " << actual_char << endl;

        if(!global_metrics) {
            hits = copy_model.hits;
            misses = copy_model.misses;
        }

        double symbol_prob = (hits + smoothing_factor) / (hits + misses + 2 * smoothing_factor);

        if (pred_char == actual_char) {
            hits++;
            estimated_number_of_bits += -log2(symbol_prob);
            cout << "Hit" << endl;
        } else {
            misses++;
            double relative_freq = static_cast<double>(counts[actual_char]) / (stream_size - counts[pred_char]);
            estimated_number_of_bits += -log2((1 - symbol_prob) * relative_freq);
            cout << "Miss" << endl;
        }

        // if sequence map not empty
        if(!sequence_map.empty() && hits / (hits + misses) < threshold) {
            copy_model.update_reference(true);
        }

        copy_model.add_anchor(ptr);
        cout << "Reference: " << copy_model.reference << endl;
    } else {
        cout << "Sequence not found" << endl;
        estimated_number_of_bits += -log2(static_cast<double>(counts[actual_char]) / stream_size);
        sequence_map[sequence] = CopyModel(ptr);
    }

    ptr++;

    cout << "Estimated number of bits: " << estimated_number_of_bits << endl;
}

// Test
//int main() {
//    string stream = "GATGAAGATTAGAT";
//    vector<char> alphabet = {'A','G','T'};
//    double threshold = 0.2;
//    double smoothing_factor = 1.0;
//    int window_size = 3;
//    bool global_metrics = true;
//
//    CopyModelRunner copyModelRunner = CopyModelRunner(stream, alphabet, threshold, smoothing_factor, window_size, global_metrics);
//
//    while (copyModelRunner.has_next()) {
//        copyModelRunner.run_step();
//    }
//
//    return 0;
//}
