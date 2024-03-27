#ifndef TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODELRUNNER_H
#define TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODELRUNNER_H

#include <string>
#include <map>
#include <unordered_map> 
#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <deque>
#include "CopyModel.h"

using namespace std;

class CopyModelRunner {
public:
    CopyModelRunner(string stream, vector<char> alphabet , int loopbackSize, int missingThreshold, double smoothingFactor, int windowSize, int limit = 1);
    bool hasNext();
    int runCopyModel(CopyModel* copyModel, vector<bool>* past, char actual_char);
    bool exceedsThreshold(int maxMisses, int nTries, vector<bool>* past);
    void runStep();
    void addRemainingBits();
    bool exceedsThreshold(int maxMisses, int nTries);
    double estimatedNumberOfBits;

private:
    string stream;
    int streamSize;
    int total_chars;
    int lookbackSize;
    int missingThreshold;
    double smoothingFactor;
    int windowSize;
    map<char, int> counts;
    unordered_map<string,vector<CopyModel>> sequenceMap;
    vector<CopyModel> currentReferences;
    vector<vector<bool>> pastOccurences;
    int limit;
    int ptr;
    string sequence;
    vector<char> alphabet;
};


#endif //TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODELRUNNER_H
