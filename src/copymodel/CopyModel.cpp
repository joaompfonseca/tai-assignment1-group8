#include "CopyModel.h"
#include <algorithm>
#include <iostream>

using namespace std;

CopyModel::CopyModel(int reference) {
    this->start = reference;
    this->reference = reference;
    this->partial_number_of_bits = 0.0;
    this->hits = 0;
    this->misses = 0;
}

double CopyModel::getBits() {
    return partial_number_of_bits;
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
    partial_number_of_bits += amount;
}

void CopyModel::reset() {
    reference = start;
    hits = 0;
    misses = 0;
    partial_number_of_bits = 0;
}