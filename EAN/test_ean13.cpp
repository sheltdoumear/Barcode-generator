#define _CRT_SECURE_NO_WARNINGS
#include "EAN13Code.h"
#include "EAN13API.h"
#include <cassert>
#include <iostream>

void test(const std::string& name, bool condition) {
    std::cout << name << " - Run" << std::endl;
    assert(condition);
    std::cout << "Test completed" << std::endl;
    std::cout << std::endl;
}

int main() {
    EAN13Code barcode;

    test("test_setData_12_digits", barcode.setData("590123412345") && barcode.getCode() == "5901234123457");

    test("test_setData_13_digits", barcode.setData("5901234123457") && barcode.getCode() == "5901234123457");

    test("test_setData_invalid_length", !barcode.setData("12345"));

    test("test_setData_empty_string", !barcode.setData(""));

    test("test_setData_non_digits", !barcode.setData("ABC123456789"));

    barcode.setData("5901234123457");
    std::string pattern = barcode.getBinaryPattern();

    test("test_binaryPattern_length", pattern.length() == 95);

    test("test_binaryPattern_start", pattern.substr(0, 3) == "101");

    test("test_binaryPattern_center", pattern.substr(45, 5) == "01010");

    test("test_binaryPattern_end", pattern.substr(92, 3) == "101");

    test("test_getPrefix", barcode.getPrefix() == "59");

    test("test_getManufacturerCode", barcode.getManufacturerCode() == "0123");

    test("test_getProductCode", barcode.getProductCode() == "41234");

    barcode.setData("000000000000");
    test("test_getChecksum", barcode.getChecksum() == 0);

    test("test_GenerateEAN13PNG_valid", GenerateEAN13PNG("5901234123457", "test_ean13.png", 2, 200) == 1);

    test("test_GenerateEAN13PNG_invalid", GenerateEAN13PNG("123", "test_bad.png", 2, 200) == 0);

    return 0;
}