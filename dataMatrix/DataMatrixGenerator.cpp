#include "DataMatrixGenerator.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <bitset>

DataMatrixGenerator::DataMatrixGenerator() {
}

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

#include <iostream>

int main() {
    try {
        DataMatrixGenerator generator;

        
        std::string testData = "Hello, Data Matrix!";

        
        std::cout << "Generating Data Matrix for: " << testData << std::endl;
        auto matrix = generator.generate(testData);

        
        auto size = generator.getSymbolSize();
        std::cout << "Symbol size: " << size.rows << "x" << size.cols << std::endl;
        std::cout << "Capacity: " << size.capacity << " bytes" << std::endl;

        
        generator.saveToBitmap(matrix, "datamatrix.bmp", 10);
        std::cout << "Saved to datamatrix.bmp" << std::endl;

        
        std::vector<std::string> testStrings = {
            "Test 123",
            "Data Matrix ECC 200",
            "https://example.com",
            "Hello World!"
        };

        for (size_t i = 0; i < testStrings.size(); i++) {
            auto m = generator.generate(testStrings[i]);
            std::string filename = "datamatrix_test" + std::to_string(i + 1) + ".bmp";
            generator.saveToBitmap(m, filename, 10);
            std::cout << "Saved: " << filename << std::endl;
        }

        std::cout << "\nData Matrix generation completed successfully!" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}