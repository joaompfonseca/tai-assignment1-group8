#include "CopyModel.h"

using namespace std;

CopyModel::CopyModel(int reference) {
    this->start = reference;
    this->reference = reference;
    this->partialNumberOfBits = 0.0;
    this->hits = 0;
    this->misses = 0;
}

double CopyModel::getBits() {
    return partialNumberOfBits;
}

int CopyModel::getHits() {
    return hits;
}

int CopyModel::getMisses() {
    return misses;
}

int CopyModel::getReference() {
    return reference;
}

void CopyModel::hit() {
    hits++;
}

void CopyModel::miss() {
    misses++;
}

void CopyModel::incrementReference() {
    reference++;
}

void CopyModel::addBits(double amount) {
    partialNumberOfBits += amount;
}

void CopyModel::reset() {
    hits = 0;
    misses = 0;
}
