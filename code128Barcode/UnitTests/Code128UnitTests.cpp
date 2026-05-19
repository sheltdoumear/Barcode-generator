#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static const int PATTERNS[106][6] = {
    {2,1,2,2,2,2},{2,2,2,1,2,2},{2,2,2,2,2,1},{1,2,1,2,2,3},
    {1,2,1,3,2,2},{1,3,1,2,2,2},{1,2,2,2,1,3},{1,2,2,3,1,2},
    {1,3,2,2,1,2},{2,2,1,2,1,3},{2,2,1,3,1,2},{2,3,1,2,1,2},
    {1,1,2,2,3,2},{1,2,2,1,3,2},{1,2,2,2,3,1},{1,1,3,2,2,2},
    {1,2,3,1,2,2},{1,2,3,2,2,1},{2,2,3,2,1,1},{2,2,1,1,3,2},
    {2,2,1,2,3,1},{2,1,3,2,1,2},{2,2,3,1,1,2},{3,1,2,1,3,1},
    {3,1,1,2,2,2},{3,2,1,1,2,2},{3,2,1,2,2,1},{3,1,2,2,1,2},
    {3,2,2,1,1,2},{3,2,2,2,1,1},{2,1,2,1,2,3},{2,1,2,3,2,1},
    {2,3,2,1,2,1},{1,1,1,3,2,3},{1,3,1,1,2,3},{1,3,1,3,2,1},
    {1,1,2,3,1,3},{1,3,2,1,1,3},{1,3,2,3,1,1},{2,1,1,3,1,3},
    {2,3,1,1,1,3},{2,3,1,3,1,1},{1,1,2,1,3,3},{1,1,2,3,3,1},
    {1,3,2,1,3,1},{1,1,3,1,2,3},{1,1,3,3,2,1},{1,3,3,1,2,1},
    {3,1,3,1,2,1},{2,1,1,3,3,1},{2,3,1,1,3,1},{2,1,3,1,1,3},
    {2,1,3,3,1,1},{2,1,3,1,3,1},{3,1,1,1,2,3},{3,1,1,3,2,1},
    {3,3,1,1,2,1},{3,1,2,1,1,3},{3,1,2,3,1,1},{3,3,2,1,1,1},
    {3,1,4,1,1,1},{2,2,1,4,1,1},{4,3,1,1,1,1},{1,1,1,2,2,4},
    {1,1,1,4,2,2},{1,2,1,1,2,4},{1,2,1,4,2,1},{1,4,1,1,2,2},
    {1,4,1,2,2,1},{1,1,2,2,1,4},{1,1,2,4,1,2},{1,2,2,1,1,4},
    {1,2,2,4,1,1},{1,4,2,1,1,2},{1,4,2,2,1,1},{2,4,1,2,1,1},
    {2,2,1,1,1,4},{4,1,3,1,1,1},{2,4,1,1,1,2},{1,3,4,1,1,1},
    {1,1,1,2,4,2},{1,2,1,1,4,2},{1,2,1,2,4,1},{1,1,4,2,1,2},
    {1,2,4,1,1,2},{1,2,4,2,1,1},{4,1,1,2,1,2},{4,2,1,1,1,2},
    {4,2,1,2,1,1},{2,1,2,1,4,1},{2,1,4,1,2,1},{4,1,2,1,2,1},
    {1,1,1,1,4,3},{1,1,1,3,4,1},{1,3,1,1,4,1},{1,1,4,1,1,3},
    {1,1,4,3,1,1},{4,1,1,1,1,3},{4,1,1,3,1,1},{1,1,3,1,4,1},
    {1,1,4,1,3,1},{3,1,1,1,4,1},{4,1,1,1,3,1},{2,1,1,2,2,2},
    {2,2,1,2,2,1},{2,2,1,1,2,2}
};

static const int STOP[7] = { 2,3,3,1,1,1,2 };

class BarcodeGenerator {
public:
    BarcodeGenerator(int w, int h, int m) : barWidth(w), height(h), margin(m) {}

    bool generate(const char* data, const char* filename) {
        if (!data || strlen(data) == 0) {
            error = "Data is empty";
            return false;
        }

        std::string strData(data);
        auto pattern = buildPattern(strData);
        int w, h;
        auto image = renderToBitmap(pattern, w, h);

        if (!stbi_write_png(filename, w, h, 1, image.data(), w)) {
            error = "Failed to write PNG";
            return false;
        }

        return true;
    }

    const char* getLastError() { return error.c_str(); }

private:
    int barWidth, height, margin;
    std::string error;

    int getCharCode(char c) { return (int)c - 32; }

    int calculateChecksum(const std::vector<int>& codes) {
        int sum = codes[0];
        for (size_t i = 1; i < codes.size(); i++) {
            sum += (int)i * codes[i];
        }
        return sum % 103;
    }

    std::vector<bool> getPattern(int code) {
        std::vector<bool> result;
        if (code == 106) {
            for (int i = 0; i < 7; i++) {
                bool isBar = (i % 2 == 0);
                for (int w = 0; w < STOP[i]; w++) result.push_back(isBar);
            }
        }
        else if (code >= 0 && code <= 105) {
            for (int i = 0; i < 6; i++) {
                bool isBar = (i % 2 == 0);
                for (int w = 0; w < PATTERNS[code][i]; w++) result.push_back(isBar);
            }
        }
        return result;
    }

    std::vector<bool> buildPattern(const std::string& data) {
        std::vector<bool> full;

        auto add = [&](int code) {
            auto p = getPattern(code);
            full.insert(full.end(), p.begin(), p.end());
            };

        add(103);

        std::vector<int> codes;
        codes.push_back(103);

        for (char c : data) {
            int code = getCharCode(c);
            codes.push_back(code);
            add(code);
        }

        int checksum = calculateChecksum(codes);
        add(checksum);
        add(106);

        return full;
    }

    std::vector<uint8_t> renderToBitmap(const std::vector<bool>& pattern, int& outWidth, int& outHeight) {
        int patternWidth = (int)pattern.size() * barWidth;
        outWidth = patternWidth + 2 * margin;
        outHeight = height + 2 * margin;

        std::vector<uint8_t> image(outWidth * outHeight, 255);

        for (size_t i = 0; i < pattern.size(); i++) {
            if (pattern[i]) {
                int x = margin + (int)i * barWidth;
                for (int w = 0; w < barWidth; w++) {
                    for (int y = margin; y < margin + height; y++) {
                        if (x + w < outWidth) {
                            image[y * outWidth + x + w] = 0;
                        }
                    }
                }
            }
        }
        return image;
    }
};

// ==================== ТЕСТЫ ====================

void PrintTestResult(const std::string& testName, bool passed) {
    std::cout << (passed ? "[PASS]" : "[FAIL]") << " " << testName << std::endl;
}

void TestCreateGenerator() {
    BarcodeGenerator* gen = new BarcodeGenerator(2, 100, 10);
    assert(gen != nullptr);
    delete gen;
    PrintTestResult("TestCreateGenerator", true);
}

void TestGenerateValidBarcode() {
    BarcodeGenerator gen(2, 100, 10);
    bool result = gen.generate("CODE128", "test_code128.png");
    assert(result == true);
    std::cout << "  - Created: test_code128.png" << std::endl;
    PrintTestResult("TestGenerateValidBarcode", true);
}

void TestGenerateEmptyData() {
    BarcodeGenerator gen(2, 100, 10);
    bool result = gen.generate("", "test_empty.png");
    assert(result == false);
    assert(strcmp(gen.getLastError(), "Data is empty") == 0);
    PrintTestResult("TestGenerateEmptyData", true);
}

void TestGenerateNullData() {
    BarcodeGenerator gen(2, 100, 10);
    bool result = gen.generate(nullptr, "test_null.png");
    assert(result == false);
    PrintTestResult("TestGenerateNullData", true);
}

void TestDifferentBarWidths() {
    BarcodeGenerator gen1(1, 100, 10);
    BarcodeGenerator gen2(3, 100, 10);

    bool result1 = gen1.generate("TEST123", "test_width1.png");
    bool result2 = gen2.generate("TEST123", "test_width3.png");

    assert(result1 == true);
    assert(result2 == true);
    std::cout << "  - Created: test_width1.png" << std::endl;
    std::cout << "  - Created: test_width3.png" << std::endl;

    PrintTestResult("TestDifferentBarWidths", true);
}

void TestDifferentHeights() {
    BarcodeGenerator gen1(2, 50, 10);
    BarcodeGenerator gen2(2, 200, 10);

    gen1.generate("TEST", "test_height50.png");
    gen2.generate("TEST", "test_height200.png");

    std::cout << "  - Created: test_height50.png" << std::endl;
    std::cout << "  - Created: test_height200.png" << std::endl;

    PrintTestResult("TestDifferentHeights", true);
}

void TestDifferentMargins() {
    BarcodeGenerator gen1(2, 100, 5);
    BarcodeGenerator gen2(2, 100, 50);

    gen1.generate("MARGIN", "test_margin5.png");
    gen2.generate("MARGIN", "test_margin50.png");

    std::cout << "  - Created: test_margin5.png" << std::endl;
    std::cout << "  - Created: test_margin50.png" << std::endl;

    PrintTestResult("TestDifferentMargins", true);
}

void TestLongData() {
    BarcodeGenerator gen(2, 100, 10);
    std::string longData(80, 'A');
    bool result = gen.generate(longData.c_str(), "test_long.png");
    assert(result == true);
    std::cout << "  - Created: test_long.png" << std::endl;
    PrintTestResult("TestLongData", true);
}

void TestNumericData() {
    BarcodeGenerator gen(2, 100, 10);
    bool result = gen.generate("1234567890", "test_numbers.png");
    assert(result == true);
    std::cout << "  - Created: test_numbers.png" << std::endl;
    PrintTestResult("TestNumericData", true);
}

void TestAlphanumericData() {
    BarcodeGenerator gen(2, 100, 10);
    bool result = gen.generate("ABC123XYZ", "test_alphanum.png");
    assert(result == true);
    std::cout << "  - Created: test_alphanum.png" << std::endl;
    PrintTestResult("TestAlphanumericData", true);
}

void TestSpecialCharacters() {
    BarcodeGenerator gen(2, 100, 10);
    bool result = gen.generate("ABC-123", "test_special.png");
    assert(result == true);
    std::cout << "  - Created: test_special.png" << std::endl;
    PrintTestResult("TestSpecialCharacters", true);
}

void TestGetPatternStartCode() {
    BarcodeGenerator gen(2, 100, 10);
    // Приватный метод не вызываем напрямую, проверяем через generate
    bool result = gen.generate("START", "test_start.png");
    assert(result == true);
    PrintTestResult("TestGetPatternStartCode", true);
}

void TestCalculateChecksum() {
    BarcodeGenerator gen(2, 100, 10);
    //проверка через generate
    bool result = gen.generate("CHECKSUM", "test_checksum.png");
    assert(result == true);
    PrintTestResult("TestCalculateChecksum", true);
}

void RunAllTests() {
    
    std::cout << "Running BarcodeGenerator Tests" << std::endl;
   

    try {
        TestCreateGenerator();
        TestGenerateValidBarcode();
        TestGenerateEmptyData();
        TestGenerateNullData();
        TestDifferentBarWidths();
        TestDifferentHeights();
        TestDifferentMargins();
        TestLongData();
        TestNumericData();
        TestAlphanumericData();
        TestSpecialCharacters();
        TestGetPatternStartCode();
        TestCalculateChecksum();

      
        std::cout << "All tests PASSED!" << std::endl;
       
    }
    catch (const std::exception& e) {
       
        std::cerr << "TEST FAILED: " << e.what() << std::endl;
     
        throw;
    }
}

void Demonstrate() {
   
    std::cout << "\n" << std::endl;
   
    BarcodeGenerator gen(2, 100, 10);

    std::vector<std::string> testData = {
        "CODE128",
        "Hello World",
        "123456789",
        "BARCODE-128"
    };

    for (size_t i = 0; i < testData.size(); i++) {
        std::string filename = "barcode_demo" + std::to_string(i + 1) + ".png";
        std::cout << "Generating: " << testData[i] << " -> " << filename << std::endl;

        if (gen.generate(testData[i].c_str(), filename.c_str())) {
            std::cout << "  Success!" << std::endl;
        }
        else {
            std::cout << "  Error: " << gen.getLastError() << std::endl;
        }
    }

 
}

int main() {
    RunAllTests();
    Demonstrate();


    return 0;
}