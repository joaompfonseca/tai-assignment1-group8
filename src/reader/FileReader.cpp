//
// Created by diogopaiva21 on 10-03-2024.
//

#include "FileReader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

FileReader::FileReader(string filename) {
    this->filename = std::move(filename);
    this->content = "";
    this->alphabet = {};
}

void FileReader::read() {
    ifstream file(this->filename, ios::in);

    if (file.is_open()) {
        // Read file char by char
        char ch;
        while (file >> noskipws >> ch) {
            content += ch;
            if(find(alphabet.begin(), alphabet.end(), ch) == alphabet.end()) {
                alphabet.push_back(ch);
            }
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
}

string FileReader::getContent() {
    return this->content;
}

vector<char> FileReader::getAlphabet() {
    return this->alphabet;
}

// Test
//int main() {
//    FileReader fileReader = FileReader("chry_test.txt");
//    fileReader.read();
//    cout << fileReader.getContent() << endl;
//    for (char c : fileReader.getAlphabet()) {
//        cout << c << " ";
//    }
//    cout << endl;
//    return 0;
//}