#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <algorithm>

#define THRESHOLD 0.2
#define SMOOTHING_FACTOR 1.0
#define WINDOW_SIZE 3

using namespace std;

int main() {
    string str = "GATGAAGATTAG";
    // comparar com GATGATGATGAT

    double estimatedNumberOfBits = 0;

    map<string, int> currentSequencePointer;
    map<string, vector<int>> previousPositions;

    for (int i = 0; i < str.length() - WINDOW_SIZE + 1; i++) {
        string sequence = str.substr(i, WINDOW_SIZE);
        printf("%s\n", sequence.c_str());

        if (previousPositions.count(sequence)) {

            int ptr;
            double hitProbability;
            double missProbability;

            if (currentSequencePointer.count(sequence) <= 0) {
                ptr = currentSequencePointer[sequence];
            } else {
                ptr = 0;
                currentSequencePointer[sequence] = 0;
                printf("current sequence pointer");
            }

            int pastPosition = previousPositions[sequence][ptr];

            if (find(previousPositions[sequence].begin(), previousPositions[sequence].end(), i) == previousPositions[sequence].end()) {
                previousPositions[sequence].push_back(i);
            }

            int j = 0;
            int hits = 0;
            int misses = 0;
            
            do {
                printf("pastPosition: %d, i: %d, j: %d\n", pastPosition, i, j);
                char predChar = str[pastPosition + j];
                char nextChar = str[i + j];
                printf("%c %c\n", predChar, nextChar);

                hitProbability = (hits + SMOOTHING_FACTOR) / (hits + misses + 2*SMOOTHING_FACTOR);
                missProbability = 1 - hitProbability;

                previousPositions[sequence].push_back(i + j);

                if (predChar == nextChar) {
                    estimatedNumberOfBits += -log2(hitProbability);
                    hits++;
                }
                else {
                    estimatedNumberOfBits += -log2(missProbability);
                    misses++;
                }

                j++;
            }
            while (hitProbability >= 0.2 && i + j < str.length());
        }

        else {
            printf("new sequence\n");
            estimatedNumberOfBits += -log2(1.0/str.length());
            previousPositions[sequence] = {i};
        }

        printf("%f\n", estimatedNumberOfBits);
    }

    fflush(stdout);

}
