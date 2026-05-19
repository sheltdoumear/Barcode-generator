#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cctype>

// ==================== BMPWriter.h ====================
#pragma once

class BMPWriter {
public:
    void save(const std::string& filename,
        const std::vector<int>& barcode,
        int height,
        int margin);
};

// ==================== BarcodeImage.h ====================
#pragma once

class BarcodeImage {
public:
    void savePNG(const std::string& filename,
        const std::vector<int>& barcode,
        int height,
        int margin,
        int scale);

    void saveSVG(const std::string& filename,
        const std::vector<int>& barcode,
        int height,
        int margin,
        int scale);
};

// ==================== Code39.h ====================
#pragma once

class Code39 {
public:
    std::vector<int> encode(const std::string& input, int narrow, int wide); // вектор - последовательность полос, 1 черная, 0 белая
};

// ==================== STB_IMAGE_WRITE ====================
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// ==================== BMPWriter.cpp ====================

void BMPWriter::save(const std::string& filename,
    const std::vector<int>& barcode,
    int height,
    int margin) {

    int width = barcode.size() + 2 * margin;
    int rowSize = (width * 3 + 3) & (~3);
    int imageSize = rowSize * height;
    int fileSize = 54 + imageSize;

    std::ofstream file(filename, std::ios::binary);

    unsigned char fileHeader[14] = {
        'B','M',  // сигнатура
        0,0,0,0,  // размер файла
        0,0,0,0,  // reserved
        54,0,0,0  // offset до пикселей
    };

    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;

    unsigned char infoHeader[40] = {
        40,0,0,0,  // размер header
        0,0,0,0,   // width
        0,0,0,0,   // height
        1,0,       // planes
        24,0,      // bits per pixel
        0,0,0,0,   // compression
        0,0,0,0,   // image size
        0,0,0,0,   // x ppm
        0,0,0,0,   // y ppm
        0,0,0,0,   // colors
        0,0,0,0
    };

    infoHeader[4] = width;
    infoHeader[5] = width >> 8;
    infoHeader[6] = width >> 16;
    infoHeader[7] = width >> 24;

    infoHeader[8] = height;
    infoHeader[9] = height >> 8;
    infoHeader[10] = height >> 16;
    infoHeader[11] = height >> 24;

    infoHeader[20] = imageSize;
    infoHeader[21] = imageSize >> 8;
    infoHeader[22] = imageSize >> 16;
    infoHeader[23] = imageSize >> 24;

    file.write((char*)fileHeader, 14);
    file.write((char*)infoHeader, 40);

    int padding = rowSize - width * 3;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            bool isBlack = false;

            if (x >= margin && x < margin + barcode.size()) {
                isBlack = barcode[x - margin] == 1;
            }

            if (isBlack) {
                file.put(0);   // B
                file.put(0);   // G
                file.put(0);   // R
            }
            else {
                file.put(255);
                file.put(255);
                file.put(255);
            }
        }

        // padding
        for (int p = 0; p < padding; p++) {
            file.put(0);
        }
    }

    file.close();
}

// ==================== BarcodeImage.cpp ====================

void BarcodeImage::savePNG(const std::string& filename,
    const std::vector<int>& barcode,
    int height,
    int margin,
    int scale)
{
    int logicalWidth = barcode.size() + 2 * margin;
    int width = logicalWidth * scale;

    std::vector<unsigned char> image(width * height * 3);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int logicalX = x / scale;
            bool isBlack = false;

            if (logicalX >= margin && logicalX < margin + barcode.size()) {
                isBlack = barcode[logicalX - margin] == 1;
            }

            int index = (y * width + x) * 3;

            unsigned char color = isBlack ? 0 : 255;

            image[index + 0] = color;
            image[index + 1] = color;
            image[index + 2] = color;
        }
    }

    stbi_write_png(filename.c_str(), width, height, 3,
        image.data(), width * 3);
}

void BarcodeImage::saveSVG(const std::string& filename,
    const std::vector<int>& barcode,
    int height,
    int margin,
    int scale)
{
    int logicalWidth = barcode.size() + 2 * margin;
    int width = logicalWidth * scale;


    std::ofstream file(filename);

    file << "<svg xmlns='http://www.w3.org/2000/svg' "
        << "width='" << width << "' height='" << height << "'>\n";

    file << "<rect width='100%' height='100%' fill='white'/>\n";

    for (int x = 0; x < barcode.size(); x++) {
        if (barcode[x] == 1) {
            file << "<rect x='" << (x + margin) * scale
                << "' y='0' width='" << scale
                << "' height='" << height
                << "' fill='black'/>\n";
        }
    }

    file << "</svg>";
}

// ==================== Code39.cpp ====================

// 1 - широкая, 0 - узкая
static std::map<char, std::string> encoding = {
    {'0', "000110100"},
    {'1', "100100001"},
    {'2', "001100001"},
    {'3', "101100000"},
    {'4', "000110001"},
    {'5', "100110000"},
    {'6', "001110000"},
    {'7', "000100101"},
    {'8', "100100100"},
    {'9', "001100100"},

    {'A', "100001001"},
    {'B', "001001001"},
    {'C', "101001000"},
    {'D', "000011001"},
    {'E', "100011000"},
    {'F', "001011000"},
    {'G', "000001101"},
    {'H', "100001100"},
    {'I', "001001100"},
    {'J', "000011100"},
    {'K', "100000011"},
    {'L', "001000011"},
    {'M', "101000010"},
    {'N', "000010011"},
    {'O', "100010010"},
    {'P', "001010010"},
    {'Q', "000000111"},
    {'R', "100000110"},
    {'S', "001000110"},
    {'T', "000010110"},
    {'U', "110000001"},
    {'V', "011000001"},
    {'W', "111000000"},
    {'X', "010010001"},
    {'Y', "110010000"},
    {'Z', "011010000"},

    {'-', "010000101"},
    {'.', "110000100"},
    {' ', "011000100"},
    {'$', "010101000"},
    {'/', "010100010"},
    {'+', "010001010"},
    {'%', "000101010"},
    {'*', "010010100"}
};

std::vector<int> Code39::encode(const std::string& input, int narrow, int wide) {

    std::string full = "*" + input + "*";
    std::vector<int> result;
    bool isBlack = true;

    for (size_t i = 0; i < full.size(); i++) {
        char c = toupper(full[i]);

        //если символа нет в encoding
        if (encoding.find(c) == encoding.end()) {
            throw std::runtime_error("unsupported character");
        }

        std::string pattern = encoding[c];

        // определяем ширину
        int width = 0;
        for (char bit : pattern) {

            int width = (bit == '1') ? wide : narrow;


            for (int i = 0; i < width; i++) {
                result.push_back(isBlack ? 1 : 0);
            }

            isBlack = !isBlack;
        }

        if (i != full.size() - 1) {
            for (int j = 0; j < narrow; j++) {
                result.push_back(0);
            }
        }
        isBlack = true;

    }

    return result;
}

// ==================== ТЕСТЫ ====================

void PrintTestResult(const std::string& testName, bool passed) {
    std::cout << (passed ? "[PASS]" : "[FAIL]") << " " << testName << std::endl;
}

// BMPWriter тесты
void TestBMPWriter_Save() {
    BMPWriter writer;
    std::vector<int> barcode = { 1,0,1,0,1,1,0,0,1 };

    try {
        writer.save("test_bmp_output.bmp", barcode, 100, 10);
        std::cout << "  - Created: test_bmp_output.bmp" << std::endl;
        assert(true);
    }
    catch (const std::exception& e) {
        assert(false);
    }
    PrintTestResult("TestBMPWriter_Save", true);
}

void TestBMPWriter_EmptyBarcode() {
    BMPWriter writer;
    std::vector<int> emptyBarcode;

    try {
        writer.save("test_bmp_empty.bmp", emptyBarcode, 50, 5);
        std::cout << "  - Created: test_bmp_empty.bmp" << std::endl;
        assert(true);
    }
    catch (const std::exception& e) {
        assert(false);
    }
    PrintTestResult("TestBMPWriter_EmptyBarcode", true);
}

// BarcodeImage тесты
void TestBarcodeImage_SavePNG() {
    BarcodeImage image;
    std::vector<int> barcode = { 1,0,1,0,1,1,0,0,1,0,1,0,1 };

    try {
        image.savePNG("test_png_output.png", barcode, 100, 10, 2);
        std::cout << "  - Created: test_png_output.png" << std::endl;
        assert(true);
    }
    catch (const std::exception& e) {
        assert(false);
    }
    PrintTestResult("TestBarcodeImage_SavePNG", true);
}

void TestBarcodeImage_SaveSVG() {
    BarcodeImage image;
    std::vector<int> barcode = { 1,0,1,0,1,1,0,0,1 };

    try {
        image.saveSVG("test_svg_output.svg", barcode, 100, 10, 2);
        std::cout << "  - Created: test_svg_output.svg" << std::endl;
        assert(true);
    }
    catch (const std::exception& e) {
        assert(false);
    }
    PrintTestResult("TestBarcodeImage_SaveSVG", true);
}

// Code39 тесты
void TestCode39_EncodeDigits() {
    Code39 code39;
    std::vector<int> result = code39.encode("123", 1, 3);
    assert(!result.empty());
    PrintTestResult("TestCode39_EncodeDigits", true);
}

void TestCode39_EncodeLetters() {
    Code39 code39;
    std::vector<int> result = code39.encode("ABC", 1, 3);
    assert(!result.empty());
    PrintTestResult("TestCode39_EncodeLetters", true);
}

void TestCode39_EncodeMixed() {
    Code39 code39;
    std::vector<int> result = code39.encode("A1B2C3", 1, 3);
    assert(!result.empty());
    PrintTestResult("TestCode39_EncodeMixed", true);
}

void TestCode39_EncodeWithSpaces() {
    Code39 code39;
    std::vector<int> result = code39.encode("HELLO WORLD", 1, 3);
    assert(!result.empty());
    PrintTestResult("TestCode39_EncodeWithSpaces", true);
}

void TestCode39_EncodeInvalidChar() {
    Code39 code39;
    bool exceptionThrown = false;

    try {
        std::vector<int> result = code39.encode("TEST@123", 1, 3);
    }
    catch (const std::runtime_error& e) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
    PrintTestResult("TestCode39_EncodeInvalidChar", true);
}

void TestCode39_DifferentWidths() {
    Code39 code39;
    std::vector<int> narrowWide = code39.encode("A", 1, 3);
    std::vector<int> customWide = code39.encode("A", 2, 5);
    assert(narrowWide.size() != customWide.size());
    PrintTestResult("TestCode39_DifferentWidths", true);
}

void TestCode39_StartsWithStar() {
    Code39 code39;
    std::vector<int> result = code39.encode("A", 1, 3);
    assert(result.size() > 0);
    assert(result[0] == 1);
    PrintTestResult("TestCode39_StartsWithStar", true);
}

void RunAllTests() {
   
    std::cout << "Running All Tests" << std::endl;
    

    // BMPWriter тесты
    TestBMPWriter_Save();
    TestBMPWriter_EmptyBarcode();

    // BarcodeImage тесты
    TestBarcodeImage_SavePNG();
    TestBarcodeImage_SaveSVG();

    // Code39 тесты
    TestCode39_EncodeDigits();
    TestCode39_EncodeLetters();
    TestCode39_EncodeMixed();
    TestCode39_EncodeWithSpaces();
    TestCode39_EncodeInvalidChar();
    TestCode39_DifferentWidths();
    TestCode39_StartsWithStar();

   
    std::cout << "All tests PASSED!" << std::endl;
   
}

// Демонстрация работы
void Demonstrate() {
    
    std::cout << "Demonstration" << std::endl;
    

    try {
        Code39 code39;
        BMPWriter bmpWriter;
        BarcodeImage imageWriter;

        std::string testData = "CODE39";
        std::cout << "Encoding: " << testData << std::endl;

        std::vector<int> barcode = code39.encode(testData, 1, 3);
        std::cout << "Barcode length: " << barcode.size() << " modules" << std::endl;

        bmpWriter.save("code39_demo.bmp", barcode, 100, 10);
        std::cout << "Saved: code39_demo.bmp" << std::endl;

        imageWriter.savePNG("code39_demo.png", barcode, 100, 10, 3);
        std::cout << "Saved: code39_demo.png" << std::endl;

        imageWriter.saveSVG("code39_demo.svg", barcode, 100, 10, 3);
        std::cout << "Saved: code39_demo.svg" << std::endl;


    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    RunAllTests();
    Demonstrate();


    return 0;
}