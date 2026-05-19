#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <iostream>
#include <cassert>

#pragma once

class DataMatrixGenerator {
public:
    struct SymbolSize {
        int rows;
        int cols;
        int dataRegionRows;
        int dataRegionCols;
        int capacity; // bytes capacity
    };

    DataMatrixGenerator() {}

    std::vector<std::vector<bool>> generate(const std::string& data);
    void saveToBitmap(const std::vector<std::vector<bool>>& matrix,
        const std::string& filename,
        int pixelSize = 10);
    SymbolSize getSymbolSize() const { return currentSymbolSize; }

    SymbolSize selectSymbolSize(int dataLength);
    std::vector<uint8_t> encodeData(const std::string& data);
    std::vector<uint8_t> addPadding(const std::vector<uint8_t>& data, int capacity);
    std::vector<uint8_t> reedSolomonEncode(const std::vector<uint8_t>& data, int ecWords);
    uint8_t gf256Multiply(uint8_t a, uint8_t b);
    uint8_t gf256Add(uint8_t a, uint8_t b);
    std::vector<uint8_t> gf256PolyMultiply(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
    std::vector<uint8_t> generateECPolynomial(int ecWords);
    void placeBit(std::vector<std::vector<bool>>& matrix, int row, int col, bool bit);
    void addFinderPattern(std::vector<std::vector<bool>>& matrix);
    void addAlignmentPattern(std::vector<std::vector<bool>>& matrix);
    std::vector<std::vector<bool>> placeModules(const std::vector<uint8_t>& codewords);

private:
    SymbolSize currentSymbolSize;

    const std::vector<SymbolSize> symbolSizes = {
        {10, 10, 8, 8, 3},    // 10x10
        {12, 12, 10, 10, 5},   // 12x12
        {14, 14, 12, 12, 8},   // 14x14
        {16, 16, 14, 14, 12},  // 16x16
        {18, 18, 16, 16, 18},  // 18x18
        {20, 20, 18, 18, 22},  // 20x20
        {22, 22, 20, 20, 30},  // 22x22
        {24, 24, 22, 22, 36},  // 24x24
        {26, 26, 24, 24, 44},  // 26x26
        {32, 32, 30, 30, 62},  // 32x32
        {36, 36, 34, 34, 86},  // 36x36
        {40, 40, 38, 38, 114}, // 40x40
        {44, 44, 42, 42, 144}, // 44x44
        {48, 48, 46, 46, 174}, // 48x48
        {52, 52, 50, 50, 204}, // 52x52
        {64, 64, 62, 62, 280}, // 64x64
        {72, 72, 70, 70, 368}, // 72x72
        {80, 80, 78, 78, 456}, // 80x80
        {88, 88, 86, 86, 576}, // 88x88
        {96, 96, 94, 94, 696}, // 96x96
        {104, 104, 102, 102, 816}, // 104x104
        {120, 120, 118, 118, 1050}, // 120x120
        {132, 132, 130, 130, 1304}, // 132x132
        {144, 144, 142, 142, 1558}  // 144x144
    };
};

DataMatrixGenerator::SymbolSize DataMatrixGenerator::selectSymbolSize(int dataLength) {
    for (const auto& size : symbolSizes) {
        if (size.capacity >= dataLength) {
            return size;
        }
    }
    throw std::runtime_error("Data too large for Data Matrix");
}

std::vector<uint8_t> DataMatrixGenerator::encodeData(const std::string& data) {
    std::vector<uint8_t> encoded;


    for (char c : data) {
        encoded.push_back(static_cast<uint8_t>(c));
    }

    return encoded;
}

std::vector<uint8_t> DataMatrixGenerator::addPadding(const std::vector<uint8_t>& data, int capacity) {
    std::vector<uint8_t> padded = data;


    while (padded.size() < static_cast<size_t>(capacity)) {
        padded.push_back(129); // Pad character
    }


    if (padded.size() < static_cast<size_t>(capacity)) {
        for (size_t i = data.size(); i < static_cast<size_t>(capacity); i++) {
            padded.push_back(static_cast<uint8_t>(129 + ((i + 1) % 26)));
        }
    }

    return padded;
}

uint8_t DataMatrixGenerator::gf256Multiply(uint8_t a, uint8_t b) {
    uint8_t product = 0;
    uint8_t mask = 0x01;

    for (int i = 0; i < 8; i++) {
        if (b & mask) {
            product ^= a;
        }
        mask <<= 1;

        uint8_t highBit = a & 0x80;
        a <<= 1;
        if (highBit) {
            a ^= 0x1D; // Полином для GF(256)
        }
    }

    return product;
}

uint8_t DataMatrixGenerator::gf256Add(uint8_t a, uint8_t b) {
    return a ^ b;
}

std::vector<uint8_t> DataMatrixGenerator::gf256PolyMultiply(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    std::vector<uint8_t> result(a.size() + b.size() - 1, 0);

    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < b.size(); j++) {
            result[i + j] = gf256Add(result[i + j], gf256Multiply(a[i], b[j]));
        }
    }

    return result;
}

std::vector<uint8_t> DataMatrixGenerator::generateECPolynomial(int ecWords) {
    std::vector<uint8_t> polynomial = { 1 };

    for (int i = 0; i < ecWords; i++) {
        std::vector<uint8_t> factor = { static_cast<uint8_t>(i + 1), 1 };
        polynomial = gf256PolyMultiply(polynomial, factor);
    }

    return polynomial;
}

std::vector<uint8_t> DataMatrixGenerator::reedSolomonEncode(const std::vector<uint8_t>& data, int ecWords) {
    std::vector<uint8_t> generator = generateECPolynomial(ecWords);
    std::vector<uint8_t> message(data.size() + ecWords, 0);


    for (size_t i = 0; i < data.size(); i++) {
        message[i] = data[i];
    }


    for (size_t i = 0; i < data.size(); i++) {
        uint8_t coefficient = message[i];

        if (coefficient != 0) {
            for (size_t j = 0; j < generator.size(); j++) {
                message[i + j] = gf256Add(message[i + j],
                    gf256Multiply(coefficient, generator[j]));
            }
        }
    }


    std::vector<uint8_t> result(data.size() + ecWords);
    for (size_t i = 0; i < data.size(); i++) {
        result[i] = data[i];
    }
    for (int i = 0; i < ecWords; i++) {
        result[data.size() + i] = message[data.size() + i];
    }

    return result;
}

void DataMatrixGenerator::placeBit(std::vector<std::vector<bool>>& matrix, int row, int col, bool bit) {
    if (row >= 0 && row < static_cast<int>(matrix.size()) &&
        col >= 0 && col < static_cast<int>(matrix[0].size())) {
        matrix[row][col] = bit;
    }
}

void DataMatrixGenerator::addFinderPattern(std::vector<std::vector<bool>>& matrix) {
    int size = matrix.size();


    for (int i = 0; i < size; i++) {
        matrix[i][0] = true;
        matrix[size - 1][i] = true;
    }


    for (int i = 1; i < size - 1; i++) {
        matrix[i][1] = (i % 2 == 0);
        matrix[size - 2][i] = ((size - i) % 2 == 0);
    }


    matrix[size - 1][0] = true;
    matrix[0][size - 1] = false;
}

void DataMatrixGenerator::addAlignmentPattern(std::vector<std::vector<bool>>& matrix) {

}

std::vector<std::vector<bool>> DataMatrixGenerator::placeModules(const std::vector<uint8_t>& codewords) {
    int size = currentSymbolSize.rows;
    std::vector<std::vector<bool>> matrix(size, std::vector<bool>(size, false));


    addFinderPattern(matrix);


    std::vector<std::pair<int, int>> placementOrder;

    for (int col = size - 1; col >= 0; col--) {
        if (col % 2 == (size % 2 == 0 ? 0 : 1)) {

            for (int row = size - 1; row >= 0; row--) {
                if (!matrix[row][col] && !matrix[row][col - 1] &&
                    !(col == 0 || col == 1 || row == size - 1 || row == size - 2)) {
                    placementOrder.push_back({ row, col });
                }
            }
        }
        else {

            for (int row = 0; row < size; row++) {
                if (!matrix[row][col] && !matrix[row][col - 1] &&
                    !(col == 0 || col == 1 || row == size - 1 || row == size - 2)) {
                    placementOrder.push_back({ row, col });
                }
            }
        }
    }


    size_t bitIndex = 0;
    for (const auto& pos : placementOrder) {
        if (bitIndex < codewords.size() * 8) {
            int byteIndex = bitIndex / 8;
            int bitPos = 7 - (bitIndex % 8);
            bool bit = (codewords[byteIndex] >> bitPos) & 1;
            matrix[pos.first][pos.second] = bit;
            bitIndex++;
        }
    }

    return matrix;
}

std::vector<std::vector<bool>> DataMatrixGenerator::generate(const std::string& data) {

    std::vector<uint8_t> encodedData = encodeData(data);


    currentSymbolSize = selectSymbolSize(encodedData.size());


    std::vector<uint8_t> paddedData = addPadding(encodedData, currentSymbolSize.capacity);


    int totalCodewords = (currentSymbolSize.rows * currentSymbolSize.cols) / 8;
    int ecWords = totalCodewords - currentSymbolSize.capacity;


    std::vector<uint8_t> rsData = reedSolomonEncode(paddedData, ecWords);


    std::vector<std::vector<bool>> matrix = placeModules(rsData);

    return matrix;
}

void DataMatrixGenerator::saveToBitmap(const std::vector<std::vector<bool>>& matrix,
    const std::string& filename,
    int pixelSize) {
    int width = matrix[0].size() * pixelSize;
    int height = matrix.size() * pixelSize;


    int fileSize = 54 + width * height * 3;
    std::vector<uint8_t> bmp(fileSize, 0);


    bmp[0] = 'B';
    bmp[1] = 'M';
    *(int*)&bmp[2] = fileSize;
    *(int*)&bmp[10] = 54;


    *(int*)&bmp[14] = 40;
    *(int*)&bmp[18] = width;
    *(int*)&bmp[22] = height;
    *(short*)&bmp[26] = 1;
    *(short*)&bmp[28] = 24;
    *(int*)&bmp[34] = width * height * 3;


    for (int y = 0; y < height; y++) {
        int matrixY = y / pixelSize;
        for (int x = 0; x < width; x++) {
            int matrixX = x / pixelSize;
            bool isBlack = matrix[matrixY][matrixX];

            int pixelIndex = 54 + (y * width + x) * 3;
            uint8_t color = isBlack ? 0 : 255;

            bmp[pixelIndex] = color;
            bmp[pixelIndex + 1] = color;
            bmp[pixelIndex + 2] = color;
        }
    }


    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    file.write(reinterpret_cast<char*>(bmp.data()), bmp.size());
    file.close();
}

// ==================== ЮНИТ-ТЕСТЫ ====================

void PrintTestResult(const std::string& testName, bool passed) {
    std::cout << (passed ? "[PASS]" : "[FAIL]") << " " << testName << std::endl;
}

void TestSelectSymbolSize() {
    DataMatrixGenerator generator;

    auto size = generator.selectSymbolSize(1);
    assert(size.rows == 10 && size.cols == 10);

    size = generator.selectSymbolSize(3);
    assert(size.rows == 10 && size.cols == 10);

    size = generator.selectSymbolSize(4);
    assert(size.rows == 12 && size.cols == 12);

    size = generator.selectSymbolSize(8);
    assert(size.rows == 14 && size.cols == 14);

    size = generator.selectSymbolSize(86);
    assert(size.rows == 36 && size.cols == 36);

    size = generator.selectSymbolSize(87);
    assert(size.rows == 40 && size.cols == 40);

    PrintTestResult("TestSelectSymbolSize", true);
}

void TestEncodeData() {
    DataMatrixGenerator generator;

    std::string testData = "Hello";
    auto encoded = generator.encodeData(testData);
    assert(encoded.size() == 5);
    assert(encoded[0] == 'H');
    assert(encoded[1] == 'e');
    assert(encoded[2] == 'l');
    assert(encoded[3] == 'l');
    assert(encoded[4] == 'o');

    testData = "";
    encoded = generator.encodeData(testData);
    assert(encoded.empty());

    PrintTestResult("TestEncodeData", true);
}

void TestAddPadding() {
    DataMatrixGenerator generator;

    std::vector<uint8_t> data = { 65, 66, 67 };
    auto padded = generator.addPadding(data, 10);
    assert(padded.size() == 10);
    assert(padded[0] == 65);
    assert(padded[1] == 66);
    assert(padded[2] == 67);

    data = { 1, 2, 3, 4, 5 };
    padded = generator.addPadding(data, 5);
    assert(padded.size() == 5);

    data = { 1, 2 };
    padded = generator.addPadding(data, 5);
    assert(padded[2] == 129);

    PrintTestResult("TestAddPadding", true);
}

void TestGF256Multiply() {
    DataMatrixGenerator generator;

    assert(generator.gf256Multiply(5, 0) == 0);
    assert(generator.gf256Multiply(0, 5) == 0);
    assert(generator.gf256Multiply(5, 1) == 5);
    assert(generator.gf256Multiply(2, 2) == 4);
    assert(generator.gf256Multiply(3, 7) == generator.gf256Multiply(7, 3));

    PrintTestResult("TestGF256Multiply", true);
}

void TestGF256Add() {
    DataMatrixGenerator generator;

    assert(generator.gf256Add(5, 3) == 6);
    assert(generator.gf256Add(0xFF, 0xAA) == 0x55);
    assert(generator.gf256Add(0, 5) == 5);
    assert(generator.gf256Add(5, 5) == 0);

    PrintTestResult("TestGF256Add", true);
}

void TestGenerateECPolynomial() {
    DataMatrixGenerator generator;

    auto poly = generator.generateECPolynomial(1);
    assert(poly.size() == 2);
    assert(poly[0] == 1);
    assert(poly[1] == 1);

    poly = generator.generateECPolynomial(2);
    assert(poly.size() == 3);

    poly = generator.generateECPolynomial(3);
    assert(poly.size() == 4);

    PrintTestResult("TestGenerateECPolynomial", true);
}

void TestReedSolomonEncode() {
    DataMatrixGenerator generator;

    std::vector<uint8_t> data = { 65, 66, 67 };
    auto encoded = generator.reedSolomonEncode(data, 2);
    assert(encoded.size() == 5);
    assert(encoded[0] == 65);
    assert(encoded[1] == 66);
    assert(encoded[2] == 67);

    data = { 1, 2, 3, 4 };
    encoded = generator.reedSolomonEncode(data, 5);
    assert(encoded.size() == 9);

    PrintTestResult("TestReedSolomonEncode", true);
}

void TestPlaceBit() {
    DataMatrixGenerator generator;
    std::vector<std::vector<bool>> matrix(10, std::vector<bool>(10, false));

    generator.placeBit(matrix, 5, 5, true);
    assert(matrix[5][5] == true);

    generator.placeBit(matrix, 3, 4, false);
    assert(matrix[3][4] == false);

    generator.placeBit(matrix, 20, 20, true);

    PrintTestResult("TestPlaceBit", true);
}

void TestGenerateDataMatrix() {
    DataMatrixGenerator generator;

    std::string testData = "Test";
    auto matrix = generator.generate(testData);

    assert(!matrix.empty());
    assert(!matrix[0].empty());

    auto size = generator.getSymbolSize();
    assert(size.rows == static_cast<int>(matrix.size()));
    assert(size.cols == static_cast<int>(matrix[0].size()));

    matrix = generator.generate("");
    assert(!matrix.empty());

    std::string longData(80, 'A');
    matrix = generator.generate(longData);
    assert(!matrix.empty());

    PrintTestResult("TestGenerateDataMatrix", true);
}

void TestSaveToBitmap() {
    DataMatrixGenerator generator;

    std::string testData = "Test";
    auto matrix = generator.generate(testData);

    try {
        generator.saveToBitmap(matrix, "test_default.bmp");
        std::cout << "  - Created: test_default.bmp" << std::endl;
    }
    catch (const std::exception& e) {
        assert(false && "Failed to save");
    }

    try {
        generator.saveToBitmap(matrix, "test_custom.bmp", 5);
        std::cout << "  - Created: test_custom.bmp" << std::endl;
    }
    catch (const std::exception& e) {
        assert(false && "Failed to save");
    }

    PrintTestResult("TestSaveToBitmap", true);
}

void RunAllTests() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Running DataMatrixGenerator Unit Tests" << std::endl;
    std::cout << "========================================\n" << std::endl;

    try {
        TestSelectSymbolSize();
        TestEncodeData();
        TestAddPadding();
        TestGF256Multiply();
        TestGF256Add();
        TestGenerateECPolynomial();
        TestReedSolomonEncode();
        TestPlaceBit();
        TestGenerateDataMatrix();
        TestSaveToBitmap();

        std::cout << "\n========================================" << std::endl;
        std::cout << "All tests PASSED!" << std::endl;
        std::cout << "========================================\n" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "\n========================================" << std::endl;
        std::cerr << "TEST FAILED: " << e.what() << std::endl;
        std::cerr << "========================================\n" << std::endl;
        throw;
    }
}

void DemonstrateGenerator() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "DataMatrix Generator Demonstration" << std::endl;
    std::cout << "========================================\n" << std::endl;

    try {
        DataMatrixGenerator generator;

        std::vector<std::string> testStrings = {
            "Hello, World!",
            "Data Matrix ECC-200",
            "https://github.com",
            "Test 12345"
        };

        for (size_t i = 0; i < testStrings.size(); i++) {
            std::cout << "Generating for: " << testStrings[i] << std::endl;
            auto matrix = generator.generate(testStrings[i]);
            auto size = generator.getSymbolSize();
            std::cout << "  Symbol size: " << size.rows << "x" << size.cols << std::endl;

            std::string filename = "datamatrix_demo" + std::to_string(i + 1) + ".bmp";
            generator.saveToBitmap(matrix, filename, 10);
            std::cout << "  Saved to: " << filename << std::endl;
            std::cout << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    RunAllTests();
    DemonstrateGenerator();

    return 0;
}