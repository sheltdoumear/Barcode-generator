#define _CRT_SECURE_NO_WARNINGS

#include "barcode_generator.h"
#include <iostream>
#include <string>

int main() {
    // Create configuration
    BarcodeGenerator::Config config;
    config.narrowBarWidth = 3;      // Bar width in pixels
    config.height = 100;             // Height of barcode
    config.margin = 30;              // Margin around barcode
    config.showText = false;         

    BarcodeGenerator generator(config);

    // CHANGE THIS TO YOUR DATA
    // Only uppercase A-Z, numbers 0-9, and symbols: !"#$%&'()*+,-./:;<=>?@[\]^_
    std::string data = "GODFILE44";

    std::string filename = "barcode.png";

    std::cout << "========================================" << std::endl;
    std::cout << "Code 128A Barcode Generator" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Encoding: " << data << std::endl;
    std::cout << "Generating..." << std::endl;

    if (generator.generate(data, filename)) {
        std::cout << "SUCCESS! Barcode saved to: " << filename << std::endl;
        system("explorer .");
    }
    else {
        std::cout << "ERROR: " << generator.getLastError() << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}