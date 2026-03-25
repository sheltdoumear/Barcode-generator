#pragma once
#include <string>
#include <vector>
#include <cstdint>

class BarcodeGenerator {
public:
    struct Config {
        int narrowBarWidth = 3;      // Width of narrow bar in pixels
        int height = 120;            // Height of barcode in pixels
        int margin = 30;             // Left/right margin
        bool showText = true;        // Show text under barcode

        Config() = default;
    };

    explicit BarcodeGenerator(const Config& config = Config());

    // Main generation function
    bool generate(const std::string& data, const std::string& outputFile);

    // Get last error
    std::string getLastError() const { return lastError; }

private:
    Config config;
    std::string lastError;
    std::string lastData;

    bool validateCode128A(const std::string& data);
    int getCharCode(char c);
    int calculateChecksum(const std::vector<int>& codes);
    std::vector<bool> getPattern(int code);
    std::vector<bool> generatePatterns(const std::string& data);
    std::vector<uint8_t> renderToBitmap(const std::vector<bool>& pattern,
        int& outWidth, int& outHeight);
    bool writePNG(const std::vector<uint8_t>& image, int width, int height,
        const std::string& filename);

    // Helper to draw text using simple bitmap
    void drawTextSimple(std::vector<uint8_t>& image, int width, const std::string& text, int x, int y);
};