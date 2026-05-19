#define _CRT_SECURE_NO_WARNINGS
#include "qr_code_generator.h"
#include "QRAPI.h"
#include <cassert>
#include <iostream>

void test(const std::string& name, bool condition) {
    std::cout << name << " - run" << std::endl;
    if (condition) {
        std::cout << "test completed" << std::endl;
    }
    else {
        std::cout << "TEST FAILED" << std::endl;
    }
    std::cout << std::endl;
    assert(condition);
}

bool testValidBitmap() {
    int w, h;
    auto bitmap = QRCodeGenerator::generateBitmap("Hello", w, h, 10, EccLevel::MEDIUM);
    return !bitmap.empty() && w > 0 && h > 0;
}

bool testEmptyData() {
    try {
        int w, h;
        QRCodeGenerator::generateBitmap("", w, h, 10, EccLevel::MEDIUM);
        return false;
    }
    catch (...) {
        return true;
    }
}

int main() {
    test("test_generateBitmap_valid_data", testValidBitmap());
    test("test_generateBitmap_empty_data", testEmptyData());

    bool result1 = QRCodeGenerator::generateToFile("Hello", "test_qr.png", 10, EccLevel::MEDIUM, ImageFormat::PNG);
    test("test_generateToFile_PNG", result1);

    bool result2 = QRCodeGenerator::generateToFile("Hello", "test_qr.bmp", 10, EccLevel::MEDIUM, ImageFormat::BMP);
    test("test_generateToFile_BMP", result2);

    bool result3 = QRCodeGenerator::generateToFile("Hello", "test_qr.tga", 10, EccLevel::MEDIUM, ImageFormat::TGA);
    test("test_generateToFile_TGA", result3);

    bool result4 = QRCodeGenerator::generateToFile("", "test_empty.png", 10, EccLevel::MEDIUM, ImageFormat::PNG);
    test("test_generateToFile_empty_data", result4 == false);

    int apiResult1 = generateQRCodePNG("API", "test_api.png", 10);
    test("test_generateQRCodePNG_API", apiResult1 == 1);

    int apiResult2 = generateQRCodeBMP("API", "test_api.bmp", 10);
    test("test_generateQRCodeBMP_API", apiResult2 == 1);

    int apiResult3 = generateQRCodeTGA("API", "test_api.tga", 10);
    test("test_generateQRCodeTGA_API", apiResult3 == 1);

    int apiResult4 = generateQRCodePNG("", "test_empty_api.png", 10);
    test("test_generateQRCodeAPI_empty", apiResult4 == 0);

    std::cout << "press enter to exit...";
    std::cin.get();

    return 0;
}