#define _SILENCE_CXX17_FILESYSTEM_DEPRECATION_WARNING

#include <iostream>
#include <string>
#include <filesystem>
#include <memory>
#include "EAN13Code.h"
#include "BarcodeRenderer.h"

namespace fs = std::filesystem;

class BarcodeApplication {
private:
    std::unique_ptr<EAN13Code> barcode;
    std::unique_ptr<BarcodeRenderer> renderer;
    std::string outputDirectory;

    void createOutputDirectory() {
        if (!fs::exists(outputDirectory)) {
            fs::create_directory(outputDirectory);
            std::cout << "Created directory: " << outputDirectory << std::endl;
        }
    }

    void getUserInput() {
        std::string prefix, manufacturer, product;

        std::cout << "\n=== EAN-13 Barcode Generator ===\n";
        std::cout << "Enter prefix (2 digits, e.g., 46 for Russia): ";
        std::cin >> prefix;
        std::cout << "Enter manufacturer code (4 digits): ";
        std::cin >> manufacturer;
        std::cout << "Enter product code (5 digits): ";
        std::cin >> product;

        if (!barcode->setData(prefix, manufacturer, product)) {
            throw std::runtime_error("Invalid input data. Check digit lengths.");
        }
    }

    void displayBarcodeInfo() {
        std::cout << "\n--- Barcode Information ---\n";
        std::cout << "Full code: " << barcode->getCode() << std::endl;
        std::cout << "Prefix: " << barcode->getPrefix() << std::endl;
        std::cout << "Manufacturer: " << barcode->getManufacturerCode() << std::endl;
        std::cout << "Product: " << barcode->getProductCode() << std::endl;
        std::cout << "Checksum: " << barcode->getChecksum() << std::endl;
        std::cout << "Pattern length: " << barcode->getBinaryPattern().length() << " bits\n";
    }

    void configureRenderer() {
        int moduleWidth, barHeight;

        std::cout << "\n--- Renderer Settings ---\n";
        std::cout << "Module width in pixels (default 2): ";
        std::cin >> moduleWidth;
        std::cout << "Bar height in pixels (default 200): ";
        std::cin >> barHeight;

        renderer = std::make_unique<BarcodeRenderer>(moduleWidth, barHeight);
    }

    void saveBarcode() {
        std::string filename;
        std::cout << "\nEnter output filename (without extension): ";
        std::cin >> filename;

        std::string filepath = outputDirectory + "/" + filename + ".png";

        renderer->render(*barcode);

        if (renderer->save(filepath)) {
            std::cout << "\nSuccess! Barcode saved to: " << filepath << std::endl;

            auto fileSize = fs::file_size(filepath);
            std::cout << "  Image size: " << renderer->getImageWriter().getWidth()
                << "x" << renderer->getImageWriter().getHeight() << " pixels\n";
            std::cout << "  File size: " << fileSize << " bytes\n";
        }
        else {
            throw std::runtime_error("Failed to save image file.");
        }
    }

public:
    BarcodeApplication(const std::string& outDir = "barcodes")
        : outputDirectory(outDir) {
        barcode = std::make_unique<EAN13Code>();
    }

    void run() {
        try {
            createOutputDirectory();
            getUserInput();
            displayBarcodeInfo();
            configureRenderer();
            saveBarcode();

            std::cout << "\nProgram completed successfully!\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << std::endl;
        }
    }
};

int main() {
    BarcodeApplication app;
    app.run();
    return 0;
}