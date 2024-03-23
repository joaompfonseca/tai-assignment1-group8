#include "CopyModel.h"
#include <algorithm>
#include <iostream>

using namespace std;

CopyModel::CopyModel(int reference) {
    this->reference = reference;
    this->anchors = vector<int>();
    if (reference != -1) {
        this->anchors.push_back(reference);
    }
    this->hits = 0;
    this->misses = 0;
}

void CopyModel::add_anchor(int position) {
    if (reference != -1) {
        anchors.push_back(position);
    } else {
        reference = position;
        anchors = {position};
    }
}

void CopyModel::update_reference(bool remove_current, string criteria) {
    if (reference != -1 && remove_current) {
        anchors.erase(remove(anchors.begin(), anchors.end(), reference), anchors.end());
    }
    if(!anchors.empty()) {
        if (criteria == "last") {
            reference = anchors.back();
        } else if (criteria == "first") {
            reference = anchors.front();
        } else if (criteria == "better") {
            reference = *max_element(anchors.begin(), anchors.end(), [&](int a, int b) {
                return hits / (hits + misses);
            });
        } else if (criteria == "random") {
            reference = anchors[rand() % anchors.size()];
        }
    } else {
        reference = -1;
    }
}

void CopyModel::hit() {
    hits++;
}

void CopyModel::miss() {
    misses++;
}