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

