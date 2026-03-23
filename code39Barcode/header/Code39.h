
#ifndef BARCODE_GENERATOR_CODE39_H
#define BARCODE_GENERATOR_CODE39_H


#include <string>
#include <vector>

using namespace std;

class Code39 {
public:
    vector<int> encode(const string& input); // вектор - последовательность полос, 1 черная, 0 белая
};


#endif //BARCODE_GENERATOR_CODE39_H