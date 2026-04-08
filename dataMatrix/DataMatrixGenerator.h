#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

class DataMatrixGenerator {
public:
    struct SymbolSize {
        int rows;
        int cols;
        int dataRegionRows;
        int dataRegionCols;
        int capacity; // bytes capacity
    };

    DataMatrixGenerator();

   
    std::vector<std::vector<bool>> generate(const std::string& data);

    
    void saveToBitmap(const std::vector<std::vector<bool>>& matrix,
        const std::string& filename,
        int pixelSize = 10);

   
    SymbolSize getSymbolSize() const { return currentSymbolSize; }

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

    
    SymbolSize selectSymbolSize(int dataLength);

    
    std::vector<uint8_t> encodeData(const std::string& data);

    
    std::vector<uint8_t> addPadding(const std::vector<uint8_t>& data, int capacity);

    
    std::vector<uint8_t> reedSolomonEncode(const std::vector<uint8_t>& data, int ecWords);

    
    std::vector<std::vector<bool>> placeModules(const std::vector<uint8_t>& codewords);

    
    void placeBit(std::vector<std::vector<bool>>& matrix, int row, int col, bool bit);
    void addFinderPattern(std::vector<std::vector<bool>>& matrix);
    void addAlignmentPattern(std::vector<std::vector<bool>>& matrix);

   
    uint8_t gf256Multiply(uint8_t a, uint8_t b);
    uint8_t gf256Add(uint8_t a, uint8_t b);
    std::vector<uint8_t> gf256PolyMultiply(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
    std::vector<uint8_t> generateECPolynomial(int ecWords);
};