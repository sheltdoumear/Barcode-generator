#include "Code39.h"
#include <map>
#include <stdexcept>

//using namespace std;


static map<char, string> encoding = {
    {'*', "100101101101"},
    {'A', "110101001011"},
    {'B', "101101001011"},
    {'C', "110110100101"},
    {'H', "101001101101"},
    {'E', "110101100101"},
    {'L', "101011001101"},
    {'O', "110101001101"},
};


vector<int> Code39::encode(const string& input) {

    string full = "*" + input + "*";
    vector<int> result;
    bool isBlack = true;

    for (char c : full) {

        //если символа нет в encoding
        if (encoding.find(c) == encoding.end()) {
            throw runtime_error("unsupported character");
        }

        string pattern = encoding[c];

        // определяем ширину
        int width = 0;
        for (char bit : pattern) {
            if (bit == '1') {width = 6;}
            else  {width = 2;}


            for (int i = 0; i < width; i++) {
                result.push_back(isBlack ? 1 : 0);
            }

            isBlack = !isBlack;
        }

        for (int i = 0; i < 2; i++) {
            result.push_back(0);
        }
        isBlack = true;

    }

    return result;
}


