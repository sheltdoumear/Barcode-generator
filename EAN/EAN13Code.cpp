#include "EAN13Code.h"
#include <stdexcept>
#include <algorithm>

const std::string EAN13Code::L_CODES[10] = {
    "0001101", "0011001", "0010011", "0111101", "0100011",
    "0110001", "0101111", "0111011", "0110111", "0001011"
};

const std::string EAN13Code::G_CODES[10] = {
    "0100111", "0110011", "0011011", "0100001", "0011101",
    "0111001", "0000101", "0010001", "0001001", "0010111"
};

const std::string EAN13Code::R_CODES[10] = {
    "1110010", "1100110", "1101100", "1000010", "1011100",
    "1001110", "1010000", "1000100", "1001000", "1110100"
};

const std::string EAN13Code::PARITY_MAP[10] = {
    "LLLLLL", "LLGLGG", "LLGGLG", "LLGGGL", "LGLLGG",
    "LGGLLG", "LGGGLL", "LGLGLG", "LGLGGL", "LGGLGL"
};

EAN13Code::EAN13Code() {
}

EAN13Code::~EAN13Code() {
}

int EAN13Code::calculateChecksum(const std::string& code12) const {
    int sum = 0;
    for (int i = 0; i < 12; ++i) {
        int digit = code12[i] - '0';
        sum += (i % 2 == 0) ? digit : digit * 3;
    }
    return (10 - (sum % 10)) % 10;
}

std::string EAN13Code::getPatternForDigit(int digit, char encodingSet) const {
    if (digit < 0 || digit > 9) {
        throw std::invalid_argument("Digit must be between 0 and 9");
    }

    switch (encodingSet) {
    case 'L': return L_CODES[digit];
    case 'G': return G_CODES[digit];
    case 'R': return R_CODES[digit];
    default: throw std::invalid_argument("Invalid encoding set");
    }
}

bool EAN13Code::setData(const std::string& code12) {
    if (code12.length() != 12 && code12.length() != 13) {
        return false;
    }

    if (!std::all_of(code12.begin(), code12.end(), ::isdigit)) {
        return false;
    }

    std::string code = code12.substr(0, 12);
    int checksum = calculateChecksum(code);
    fullCode = code + std::to_string(checksum);

    buildBinaryPattern();
    return true;
}

bool EAN13Code::setData(const std::string& prefix, const std::string& manufacturer,
    const std::string& product) {
    std::string code12 = prefix + manufacturer + product;

    if (code12.length() != 11) {
        return false;
    }

    return setData(code12);
}

void EAN13Code::buildBinaryPattern() {
    if (fullCode.length() != 13) {
        throw std::runtime_error("Invalid EAN-13 code");
    }

    binaryPattern.clear();
    binaryPattern = "101";

    int firstDigit = fullCode[0] - '0';
    std::string leftEncoding = PARITY_MAP[firstDigit];

    for (int i = 0; i < 6; ++i) {
        int digit = fullCode[i + 1] - '0';
        binaryPattern += getPatternForDigit(digit, leftEncoding[i]);
    }

    binaryPattern += "01010";

    for (int i = 7; i < 13; ++i) {
        int digit = fullCode[i] - '0';
        binaryPattern += getPatternForDigit(digit, 'R');
    }

    binaryPattern += "101";
}

std::string EAN13Code::getPrefix() const {
    return fullCode.substr(0, 2);
}

std::string EAN13Code::getManufacturerCode() const {
    return fullCode.substr(2, 4);
}

std::string EAN13Code::getProductCode() const {
    return fullCode.substr(6, 5);
}

int EAN13Code::getChecksum() const {
    return fullCode[12] - '0';
}