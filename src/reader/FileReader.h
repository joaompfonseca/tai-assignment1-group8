//
// Created by diogopaiva21 on 10-03-2024.
//

#ifndef TAI_ASSIGNMENT1_GROUP8_SRC_FILEREADER_H
#define TAI_ASSIGNMENT1_GROUP8_SRC_FILEREADER_H

#include <string>
#include <vector>

using namespace std;

class FileReader {
    string filename;
    string content;
    vector<char> alphabet;

    public:
     FileReader(string filename);
        void read();
        string getContent();
        vector<char> getAlphabet();
};


#endif //TAI_ASSIGNMENT1_GROUP8_SRC_FILEREADER_H
