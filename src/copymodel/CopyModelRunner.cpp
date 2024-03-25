#include <iostream>
#include <cmath>
#include "CopyModelRunner.h"

using namespace std;

CopyModelRunner::CopyModelRunner(string stream, vector<char> alphabet, double threshold, double smoothingFactor, int windowSize, int limit) {
    this->stream = stream;
    this->streamSize = stream.size();
    this->alphabet = alphabet;
    this->threshold = threshold;
    this->smoothingFactor = smoothingFactor;
    this->windowSize = windowSize;
    this->sequenceMap = unordered_map<string, vector<CopyModel>>();
    this->estimatedNumberOfBits = 0.0;
    this->limit = limit;
    this->ptr = 0;
    this->currentReferences = vector<CopyModel>();

    for (char c : stream) {
        if (counts.find(c) == counts.end()) {
            counts[c] = 1;
        } else {
            counts[c]++;
        }
    }
}

bool CopyModelRunner::hasNext() {
    // bool iter = ptr < streamSize;
    // if (!iter && !currentReferences.empty()) {
    //     estimatedNumberOfBits += currentReferences[0].getBits();
    // }
    // return iter;

    return ptr < streamSize;
}

int CopyModelRunner::runCopyModel(string sequence, CopyModel copyModel) {
    char actual_char = stream[ptr];
    int reference_ptr = copyModel.getReference();
    char pred_char = reference_ptr < 0 ? alphabet[0] : stream[reference_ptr];

    cout << "Predicted: " << pred_char << ", Actual: " << actual_char << endl;

    int hits = copyModel.getHits();
    int misses = copyModel.getMisses();

    double symbol_prob = (hits + smoothingFactor) / (hits + misses + 2 * smoothingFactor);

    if (pred_char == actual_char) {
        copyModel.hit();
        copyModel.addBits(-log2(symbol_prob));
        cout << "Hit" << endl;
        hits++;
    } else {
        copyModel.miss();
        double relative_freq = (double) counts[actual_char] / (streamSize - counts[pred_char]);
        copyModel.addBits(-log2((1 - symbol_prob) * relative_freq));
        misses++;
        cout << "Miss" << endl;
    }

    // disable copy model
    if(hits / (hits + misses) < threshold) {
        copyModel.reset();
        return -1;
    }

    copyModel.incrementReference();
    return 0;
}

void CopyModelRunner::runStep() {
    string sequence = ptr - windowSize < 0 ? string(windowSize - ptr, alphabet[0]) + stream.substr(0, ptr) : stream.substr(ptr - windowSize, windowSize);
    cout << "Sequence: " << sequence << endl;

    // no active copy models yet
    if (currentReferences.empty()) {
        if (sequenceMap.empty()) {
            CopyModel copyModel = CopyModel(-1);
            // add sequence
            sequenceMap[sequence] = vector<CopyModel>();
            sequenceMap[sequence].push_back(copyModel);
            currentReferences = sequenceMap[sequence];
        }
        else if (sequenceMap.find(sequence) != sequenceMap.end()) {
            cout << "Sequence found" << endl;
            int bound = min(limit, (int) sequenceMap[sequence].size());
            for (int i = bound - 1; i >= 0; i--) {
                CopyModel copyModel = sequenceMap[sequence][i];
                currentReferences.push_back(copyModel);
            }
        }
        else {
            cout << "Sequence not found" << endl;
            char actual_char = stream[ptr];
            estimatedNumberOfBits += -log2((double)(counts[actual_char]) / streamSize);
            sequenceMap[sequence] = vector<CopyModel>();
            sequenceMap[sequence].push_back(CopyModel(ptr));
            ptr++;
            return;
        }
    }

    int numberOfReferences = currentReferences.size();
    for (int i = 0; i < numberOfReferences; i++) {
        CopyModel copyModel = currentReferences[i];
        cout << "Reference: " << copyModel.getReference() << endl;
        if (runCopyModel(sequence, copyModel) == -1) {
            currentReferences.erase(currentReferences.begin() + i);

            // last standing copy model
            if (currentReferences.empty()) {
                estimatedNumberOfBits += copyModel.getBits();
                break;
            }
        }
    }

    sequenceMap[sequence].push_back(CopyModel(ptr)); // here it is guaranteed that the sequence is in the map
    ptr++;
}

// Test
// int main() {
//    string stream = "AAAAAT"; //"GATGGATTAGATGG";
//    vector<char> alphabet = {'A','G','T'};
//    double threshold = 0.2;
//    double smoothingFactor = 1.0;
//    int windowSize = 3;
//    bool global_metrics = true;

//    CopyModelRunner copyModelRunner = CopyModelRunner(stream, alphabet, threshold, smoothingFactor, windowSize, global_metrics);

//    while (copyModelRunner.hasNext()) {
//        copyModelRunner.runStep();
//    }

//    cout << "Estimated number of bits: " << copyModelRunner.estimatedNumberOfBits << endl;

//    return 0;
// }
