#include "CopyModelRunner.h"

using namespace std;

CopyModelRunner::CopyModelRunner(string stream, vector<char> alphabet, int lookbackSize, int missingThreshold, double smoothingFactor, int windowSize, int limit) {
    this->stream = stream;
    this->streamSize = stream.size();
    this->total_chars = streamSize;
    this->alphabet = alphabet;
    this->lookbackSize = lookbackSize;
    this->missingThreshold = missingThreshold;
    this->smoothingFactor = smoothingFactor;
    this->windowSize = windowSize;
    this->sequenceMap = unordered_map<string, vector<CopyModel>>();
    this->limit = limit;
    this->ptr = 0;
    this->currentReferences = vector<CopyModel>();
    this->pastOccurences = vector<vector<bool>>();

    int necessaryBits = 1.0;
    for (char c : stream) {
        if (counts.find(c) == counts.end()) {
            counts[c] = 1;
        } else {
            counts[c]++;
        }

        if (counts[c] > (2 << (necessaryBits - 1))) {
            necessaryBits++;
        }
    }

    // Overhead: 8 bits (alphabet size) + 8 bits per symbol + necessaryBits bits per symbol absolute frequency
    this->estimatedNumberOfBits = 8.0 + alphabet.size() * (8.0 + necessaryBits);
}

bool CopyModelRunner::hasNext() {
    return ptr < streamSize;
}

bool CopyModelRunner::exceedsThreshold(int maxMisses, int nTries, vector<bool>* past) {
    vector<bool> last = vector<bool>();
    int currentMisses = 0;

    for (int i = max(0, (int) past->size() - nTries); i < (int) past->size(); i++) {
        if (!(*past)[i]) {
            currentMisses++;
        }
        last.push_back((*past)[i]);
    }

    *past = last;
    return currentMisses > maxMisses;
}

int CopyModelRunner::runCopyModel(CopyModel* copyModel, vector<bool>* past, char actual_char) {
    int reference_ptr = copyModel->getReference();
    char pred_char = reference_ptr < 0 ? alphabet[0] : stream[reference_ptr];

    int hits = copyModel->getHits();
    int misses = copyModel->getMisses();

    double symbol_prob = (hits + smoothingFactor) / (hits + misses + 2 * smoothingFactor);

    if (pred_char == actual_char) {
        copyModel->hit();
        copyModel->addBits(-log2(symbol_prob));
        hits++;
        past->push_back(true);
    } else {
        copyModel->miss();
        double relative_freq = (double) counts[actual_char] / (total_chars - counts[pred_char]);
        copyModel->addBits(-log2((1 - symbol_prob) * relative_freq));
        misses++;
        past->push_back(false);
    }

    if (exceedsThreshold(this->missingThreshold, this->lookbackSize, past)) {
        // disable copy model
        return -1;
    }

    copyModel->incrementReference();
    return 0;
}

void CopyModelRunner::runStep() {
    sequence = ptr - windowSize < 0 ? string(windowSize - ptr, alphabet[0]) + stream.substr(0, ptr) : stream.substr(ptr - windowSize, windowSize);
    char actual_char;

    // no active copy models yet
    if (currentReferences.empty()) {
        if (sequenceMap.empty()) {
            // adding new copy model
            CopyModel copyModel = CopyModel(-1);
            sequenceMap[sequence] = vector<CopyModel>();
            sequenceMap[sequence].push_back(copyModel);
            currentReferences = sequenceMap[sequence];
            pastOccurences.push_back(vector<bool>());
        }
        else if (sequenceMap.find(sequence) != sequenceMap.end()) {
            // enabling copy models of known sequence
            int size = sequenceMap[sequence].size();
            int bound = min(limit, size);
            for (int i = size - 1; i >= size - bound; i--) {
                CopyModel copyModel = sequenceMap[sequence][i];
                currentReferences.push_back(copyModel);
                pastOccurences.push_back(vector<bool>());
            }
        }
        else {
            // using fallback model
            actual_char = stream[ptr];
            estimatedNumberOfBits += -log2((double)(counts[actual_char]) / total_chars);
            sequenceMap[sequence] = vector<CopyModel>();
        }
    }

    vector <CopyModel> newReferences = vector<CopyModel>();
    int numberOfReferences = currentReferences.size();
    
    actual_char = stream[ptr];
    double partialBits;
    for (int i = 0; i < numberOfReferences; i++) {
        if (runCopyModel(&currentReferences[i], &pastOccurences[i], actual_char) == -1) {
            pastOccurences.erase(pastOccurences.begin() + i);
            partialBits = currentReferences[i].getBits();

            // last standing copy model
            if (i == numberOfReferences - 1 && newReferences.empty()) {
                estimatedNumberOfBits += partialBits;
                break;
            }
            continue;
        }
        newReferences.push_back(currentReferences[i]);
    }
    currentReferences = newReferences;

    // decrement the count of already processed characters
    counts[actual_char]--;
    total_chars--;

    CopyModel copyModel = CopyModel(ptr);
    sequenceMap[sequence].push_back(copyModel); // here it is guaranteed that the sequence is in the map
    ptr++;

}

void CopyModelRunner::addRemainingBits() {
    // when the stream ended but there were still active copy models
    double min = INFINITY;
    double current;

    for (CopyModel copyModel : currentReferences) {
        current = copyModel.getBits();
        if (current < min) {
            min = current;
        }
    }

    estimatedNumberOfBits += min == INFINITY ? 0 : min;
}
