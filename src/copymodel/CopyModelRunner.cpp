#include "CopyModelRunner.h"

using namespace std;

CopyModelRunner::CopyModelRunner(string stream, vector<char> alphabet, double threshold, double smoothingFactor, int windowSize, int limit) {
    this->stream = stream;
    this->streamSize = stream.size();
    this->total_chars = streamSize;
    this->alphabet = alphabet;
    this->threshold = threshold;
    this->smoothingFactor = smoothingFactor;
    this->windowSize = windowSize;
    this->sequenceMap = unordered_map<string, vector<CopyModel>>();
    this->limit = limit;
    this->ptr = 0;
    this->currentReferences = vector<CopyModel>();

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
    //cout << "Overhead: " << estimatedNumberOfBits << endl << endl;
}

bool CopyModelRunner::hasNext() {
    return ptr < streamSize;
}

int CopyModelRunner::runCopyModel(CopyModel* copyModel, char actual_char) {
    int reference_ptr = copyModel->getReference();
    char pred_char = reference_ptr < 0 ? alphabet[0] : stream[reference_ptr];

    //cout << "Predicted: " << pred_char << ", Actual: " << actual_char << endl;

    int hits = copyModel->getHits();
    int misses = copyModel->getMisses();

    double symbol_prob = (hits + smoothingFactor) / (hits + misses + 2 * smoothingFactor);

    if (pred_char == actual_char) {
        copyModel->hit();
        copyModel->addBits(-log2(symbol_prob));
        //cout << "Hit: " << ' ' << symbol_prob << endl;
        hits++;
    } else {
        copyModel->miss();
        double relative_freq = (double) counts[actual_char] / (total_chars - counts[pred_char]);
        copyModel->addBits(-log2((1 - symbol_prob) * relative_freq));
        //cout << "Miss: " << "(1 - " << symbol_prob << ") * " << relative_freq << " = " << (1 - symbol_prob) * relative_freq << endl;
        misses++;
    }

    // disable copy model
    if (hits + misses == 15) {
        copyModel->reset();
    }

    if(misses > 7) {
        //cout << "Disabling copy model" << endl;
        return -1;
    }

    copyModel->incrementReference();
    return 0;
}

void CopyModelRunner::runStep() {
    //cout << "Pointer: " << ptr << endl;
    sequence = ptr - windowSize < 0 ? string(windowSize - ptr, alphabet[0]) + stream.substr(0, ptr) : stream.substr(ptr - windowSize, windowSize);
    //cout << "Sequence: " << sequence << endl;
    char actual_char;
    // new anchor (disabled by default, i.e, not in the currentReferences vector)

    // no active copy models yet
    if (currentReferences.empty()) {
        if (sequenceMap.empty()) {
            //cout << "Starting new copy model" << endl;
            CopyModel copyModel = CopyModel(-1);
            // add sequence
            sequenceMap[sequence] = vector<CopyModel>();
            sequenceMap[sequence].push_back(copyModel);
            currentReferences = sequenceMap[sequence];
        }
        else if (sequenceMap.find(sequence) != sequenceMap.end()) {
            //cout << "Starting new copy model" << endl;
            int size = sequenceMap[sequence].size();
            int bound = min(limit, size);
            for (int i = size - 1; i >= size - bound; i--) {
                CopyModel copyModel = sequenceMap[sequence][i];
                currentReferences.push_back(copyModel);
            }
        }
        else {
            actual_char = stream[ptr];
            estimatedNumberOfBits += -log2((double)(counts[actual_char]) / total_chars);

            //cout << "Starting new copy model" << endl;

            sequenceMap[sequence] = vector<CopyModel>();
            //cout << "Predicted: None, Actual: " << actual_char << endl;
            //cout << "Fallback: " << (double)(counts[actual_char]) / streamSize << endl;
        }
    }

    // if (!currentReferences.empty()) {
    //     //cout << "--- Current references ---" << endl;
    //     for (CopyModel copyModel : currentReferences) {
    //         //cout << "Reference pointer: " << copyModel.getReference() << endl;
    //     }
    //     //cout << "-------------------------" << endl;
    // }

    vector <CopyModel> newReferences = vector<CopyModel>();
    int numberOfReferences = currentReferences.size();
    
    actual_char = stream[ptr];
    double partialBits;
    for (int i = 0; i < numberOfReferences; i++) {
        if (runCopyModel(&currentReferences[i], actual_char) == -1) {
            partialBits = currentReferences[i].getBits();
            //cout << "Partial bits: " << partialBits << endl;

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

    // decrement the count of the actual character and total_chars
    counts[actual_char]--;
    total_chars--;

    CopyModel copyModel = CopyModel(ptr);
    sequenceMap[sequence].push_back(copyModel); // here it is guaranteed that the sequence is in the map
    ptr++;

    //cout << "Estimated number of bits: " << estimatedNumberOfBits << endl;
    //cout << endl;
}

void CopyModelRunner::addRemainingBits() { // when the stream is over but there were still active copy models
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

// Test
// int main() {
//    string stream = "AAGTAAT";
//    // try also "AAAAAGGAAAAG"; AAGTAAT
//    vector<char> alphabet = {'A','G','T'};
//    double threshold = 0.2;
//    double smoothingFactor = 1.0;
//    int windowSize = 2;
//    int limit = 2;

//    CopyModelRunner copyModelRunner = CopyModelRunner(stream, alphabet, threshold, smoothingFactor, windowSize, limit);

//    while (copyModelRunner.hasNext()) {
//        copyModelRunner.runStep();
//    }
//    copyModelRunner.addRemainingBits();

//    //cout << "Estimated number of bits: " << copyModelRunner.estimatedNumberOfBits << endl;

//    return 0;
// }
