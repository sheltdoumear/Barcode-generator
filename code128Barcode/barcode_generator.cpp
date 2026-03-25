#define _CRT_SECURE_NO_WARNINGS

#include "barcode_generator.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Code 128 encoding table
// Format: [bar, space, bar, space, bar, space] - each value 1-4
static const int CODE128_PATTERNS[107][6] = {
    {2,1,2,2,2,2}, // 0
    {2,2,2,1,2,2}, // 1
    {2,2,2,2,2,1}, // 2
    {1,2,1,2,2,3}, // 3
    {1,2,1,3,2,2}, // 4
    {1,3,1,2,2,2}, // 5
    {1,2,2,2,1,3}, // 6
    {1,2,2,3,1,2}, // 7
    {1,3,2,2,1,2}, // 8
    {2,2,1,2,1,3}, // 9
    {2,2,1,3,1,2}, // 10
    {2,3,1,2,1,2}, // 11
    {1,1,2,2,3,2}, // 12
    {1,2,2,1,3,2}, // 13
    {1,2,2,2,3,1}, // 14
    {1,1,3,2,2,2}, // 15
    {1,2,3,1,2,2}, // 16
    {1,2,3,2,2,1}, // 17
    {2,2,3,2,1,1}, // 18
    {2,2,1,1,3,2}, // 19
    {2,2,1,2,3,1}, // 20
    {2,1,3,2,1,2}, // 21
    {2,2,3,1,1,2}, // 22
    {3,1,2,1,3,1}, // 23
    {3,1,1,2,2,2}, // 24
    {3,2,1,1,2,2}, // 25
    {3,2,1,2,2,1}, // 26
    {3,1,2,2,1,2}, // 27
    {3,2,2,1,1,2}, // 28
    {3,2,2,2,1,1}, // 29
    {2,1,2,1,2,3}, // 30
    {2,1,2,3,2,1}, // 31
    {2,3,2,1,2,1}, // 32
    {1,1,1,3,2,3}, // 33
    {1,3,1,1,2,3}, // 34
    {1,3,1,3,2,1}, // 35
    {1,1,2,3,1,3}, // 36
    {1,3,2,1,1,3}, // 37
    {1,3,2,3,1,1}, // 38
    {2,1,1,3,1,3}, // 39
    {2,3,1,1,1,3}, // 40
    {2,3,1,3,1,1}, // 41
    {1,1,2,1,3,3}, // 42
    {1,1,2,3,3,1}, // 43
    {1,3,2,1,3,1}, // 44
    {1,1,3,1,2,3}, // 45
    {1,1,3,3,2,1}, // 46
    {1,3,3,1,2,1}, // 47
    {3,1,3,1,2,1}, // 48
    {2,1,1,3,3,1}, // 49
    {2,3,1,1,3,1}, // 50
    {2,1,3,1,1,3}, // 51
    {2,1,3,3,1,1}, // 52
    {2,1,3,1,3,1}, // 53
    {3,1,1,1,2,3}, // 54
    {3,1,1,3,2,1}, // 55
    {3,3,1,1,2,1}, // 56
    {3,1,2,1,1,3}, // 57
    {3,1,2,3,1,1}, // 58
    {3,3,2,1,1,1}, // 59
    {3,1,4,1,1,1}, // 60
    {2,2,1,4,1,1}, // 61
    {4,3,1,1,1,1}, // 62
    {1,1,1,2,2,4}, // 63
    {1,1,1,4,2,2}, // 64
    {1,2,1,1,2,4}, // 65
    {1,2,1,4,2,1}, // 66
    {1,4,1,1,2,2}, // 67
    {1,4,1,2,2,1}, // 68
    {1,1,2,2,1,4}, // 69
    {1,1,2,4,1,2}, // 70
    {1,2,2,1,1,4}, // 71
    {1,2,2,4,1,1}, // 72
    {1,4,2,1,1,2}, // 73
    {1,4,2,2,1,1}, // 74
    {2,4,1,2,1,1}, // 75
    {2,2,1,1,1,4}, // 76
    {4,1,3,1,1,1}, // 77
    {2,4,1,1,1,2}, // 78
    {1,3,4,1,1,1}, // 79
    {1,1,1,2,4,2}, // 80
    {1,2,1,1,4,2}, // 81
    {1,2,1,2,4,1}, // 82
    {1,1,4,2,1,2}, // 83
    {1,2,4,1,1,2}, // 84
    {1,2,4,2,1,1}, // 85
    {4,1,1,2,1,2}, // 86
    {4,2,1,1,1,2}, // 87
    {4,2,1,2,1,1}, // 88
    {2,1,2,1,4,1}, // 89
    {2,1,4,1,2,1}, // 90
    {4,1,2,1,2,1}, // 91
    {1,1,1,1,4,3}, // 92
    {1,1,1,3,4,1}, // 93
    {1,3,1,1,4,1}, // 94
    {1,1,4,1,1,3}, // 95
    {1,1,4,3,1,1}, // 96
    {4,1,1,1,1,3}, // 97
    {4,1,1,3,1,1}, // 98
    {1,1,3,1,4,1}, // 99
    {1,1,4,1,3,1}, // 100
    {3,1,1,1,4,1}, // 101
    {4,1,1,1,3,1}, // 102
    {2,1,1,2,2,2}, // 103 - Start A
    {2,2,1,2,2,1}, // 104 - Start B
    {2,2,1,1,2,2}  // 105 - Start C
};

// Stop pattern is special - has 7 elements
static const int STOP_PATTERN[7] = { 2, 3, 3, 1, 1, 1, 2 };

BarcodeGenerator::BarcodeGenerator(const Config& cfg) : config(cfg) {}

bool BarcodeGenerator::validateCode128A(const std::string& data) {
    if (data.empty()) {
        lastError = "Data is empty";
        return false;
    }

    for (char c : data) {
        // Code 128A: ASCII 32-95
        if (c < 32 || c > 95) {
            lastError = "Invalid character: '" + std::string(1, c) +
                "'. Use only A-Z, 0-9 and symbols !\"#$%&'()*+,-./:;<=>?@[\\]^_";
            return false;
        }
    }
    return true;
}

int BarcodeGenerator::getCharCode(char c) {
    return static_cast<int>(c) - 32;
}

int BarcodeGenerator::calculateChecksum(const std::vector<int>& codes) {
    int checksum = codes[0]; // Start code

    for (size_t i = 1; i < codes.size(); i++) {
        checksum += static_cast<int>(i) * codes[i];
    }

    return checksum % 103;
}

std::vector<bool> BarcodeGenerator::getPattern(int code) {
    std::vector<bool> result;

    if (code == 106) {
        // Stop pattern - special case (7 elements)
        for (int i = 0; i < 7; i++) {
            bool isBar = (i % 2 == 0);
            int width = STOP_PATTERN[i];
            for (int w = 0; w < width; w++) {
                result.push_back(isBar);
            }
        }
    }
    else if (code >= 0 && code <= 105) {
        // Regular pattern (6 elements)
        for (int i = 0; i < 6; i++) {
            bool isBar = (i % 2 == 0);
            int width = CODE128_PATTERNS[code][i];
            for (int w = 0; w < width; w++) {
                result.push_back(isBar);
            }
        }
    }

    return result;
}

std::vector<bool> BarcodeGenerator::generatePatterns(const std::string& data) {
    std::vector<bool> fullPattern;

    // Start Code A (103)
    std::vector<bool> startPattern = getPattern(103);
    fullPattern.insert(fullPattern.end(), startPattern.begin(), startPattern.end());

    // Data
    std::vector<int> codes;
    codes.push_back(103); // Start code

    for (char c : data) {
        int code = getCharCode(c);
        codes.push_back(code);

        std::vector<bool> charPattern = getPattern(code);
        fullPattern.insert(fullPattern.end(), charPattern.begin(), charPattern.end());
    }

    // Checksum
    int checksum = calculateChecksum(codes);
    codes.push_back(checksum);

    std::vector<bool> checksumPattern = getPattern(checksum);
    fullPattern.insert(fullPattern.end(), checksumPattern.begin(), checksumPattern.end());

    // Stop pattern (106)
    std::vector<bool> stopPattern = getPattern(106);
    fullPattern.insert(fullPattern.end(), stopPattern.begin(), stopPattern.end());

    return fullPattern;
}

// Simple font for displaying text under barcode (5x7 pixel font)
static const unsigned char FONT_5x7[95][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x00,0x00,0x5f,0x00,0x00}, // !
    {0x00,0x07,0x00,0x07,0x00}, // "
    {0x14,0x7f,0x14,0x7f,0x14}, // #
    {0x24,0x2a,0x7f,0x2a,0x12}, // $
    {0x23,0x13,0x08,0x64,0x62}, // %
    {0x36,0x49,0x55,0x22,0x50}, // &
    {0x00,0x05,0x03,0x00,0x00}, // '
    {0x00,0x1c,0x22,0x41,0x00}, // (
    {0x00,0x41,0x22,0x1c,0x00}, // )
    {0x14,0x08,0x3e,0x08,0x14}, // *
    {0x08,0x08,0x3e,0x08,0x08}, // +
    {0x00,0x50,0x30,0x00,0x00}, // ,
    {0x08,0x08,0x08,0x08,0x08}, // -
    {0x00,0x60,0x60,0x00,0x00}, // .
    {0x20,0x10,0x08,0x04,0x02}, // /
    {0x3e,0x51,0x49,0x45,0x3e}, // 0
    {0x00,0x42,0x7f,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4b,0x31}, // 3
    {0x18,0x14,0x12,0x7f,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3c,0x4a,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1e}, // 9
    {0x00,0x36,0x36,0x00,0x00}, // :
    {0x00,0x56,0x36,0x00,0x00}, // ;
    {0x08,0x14,0x22,0x41,0x00}, // <
    {0x14,0x14,0x14,0x14,0x14}, // =
    {0x00,0x41,0x22,0x14,0x08}, // >
    {0x02,0x01,0x51,0x09,0x06}, // ?
    {0x32,0x49,0x79,0x41,0x3e}, // @
    {0x7e,0x11,0x11,0x11,0x7e}, // A
    {0x7f,0x49,0x49,0x49,0x36}, // B
    {0x3e,0x41,0x41,0x41,0x22}, // C
    {0x7f,0x41,0x41,0x22,0x1c}, // D
    {0x7f,0x49,0x49,0x49,0x41}, // E
    {0x7f,0x09,0x09,0x09,0x01}, // F
    {0x3e,0x41,0x49,0x49,0x7a}, // G
    {0x7f,0x08,0x08,0x08,0x7f}, // H
    {0x00,0x41,0x7f,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3f,0x01}, // J
    {0x7f,0x08,0x14,0x22,0x41}, // K
    {0x7f,0x40,0x40,0x40,0x40}, // L
    {0x7f,0x02,0x0c,0x02,0x7f}, // M
    {0x7f,0x04,0x08,0x10,0x7f}, // N
    {0x3e,0x41,0x41,0x41,0x3e}, // O
    {0x7f,0x09,0x09,0x09,0x06}, // P
    {0x3e,0x41,0x51,0x21,0x5e}, // Q
    {0x7f,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7f,0x01,0x01}, // T
    {0x3f,0x40,0x40,0x40,0x3f}, // U
    {0x1f,0x20,0x40,0x20,0x1f}, // V
    {0x3f,0x40,0x38,0x40,0x3f}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x07,0x08,0x70,0x08,0x07}, // Y
    {0x61,0x51,0x49,0x45,0x43}, // Z
    {0x00,0x7f,0x41,0x41,0x00}, // [
    {0x02,0x04,0x08,0x10,0x20}, // backslash
    {0x00,0x41,0x41,0x7f,0x00}, // ]
    {0x04,0x02,0x01,0x02,0x04}, // ^
    {0x40,0x40,0x40,0x40,0x40}, // _
    {0x00,0x01,0x02,0x04,0x00}, // `
    {0x20,0x54,0x54,0x54,0x78}, // a
    {0x7f,0x48,0x44,0x44,0x38}, // b
    {0x38,0x44,0x44,0x44,0x20}, // c
    {0x38,0x44,0x44,0x48,0x7f}, // d
    {0x38,0x54,0x54,0x54,0x18}, // e
    {0x08,0x7e,0x09,0x01,0x02}, // f
    {0x0c,0x52,0x52,0x52,0x3e}, // g
    {0x7f,0x08,0x04,0x04,0x78}, // h
    {0x00,0x44,0x7d,0x40,0x00}, // i
    {0x20,0x40,0x44,0x3d,0x00}, // j
    {0x7f,0x10,0x28,0x44,0x00}, // k
    {0x00,0x41,0x7f,0x40,0x00}, // l
    {0x7c,0x04,0x18,0x04,0x78}, // m
    {0x7c,0x08,0x04,0x04,0x78}, // n
    {0x38,0x44,0x44,0x44,0x38}, // o
    {0x7c,0x14,0x14,0x14,0x08}, // p
    {0x08,0x14,0x14,0x18,0x7c}, // q
    {0x7c,0x08,0x04,0x04,0x08}, // r
    {0x48,0x54,0x54,0x54,0x20}, // s
    {0x04,0x3f,0x44,0x40,0x20}, // t
    {0x3c,0x40,0x40,0x20,0x7c}, // u
    {0x1c,0x20,0x40,0x20,0x1c}, // v
    {0x3c,0x40,0x30,0x40,0x3c}, // w
    {0x44,0x28,0x10,0x28,0x44}, // x
    {0x0c,0x50,0x50,0x50,0x3c}, // y
    {0x44,0x64,0x54,0x4c,0x44}  // z
};

void BarcodeGenerator::drawTextSimple(std::vector<uint8_t>& image, int width,
    const std::string& text, int x, int y) {
    int currentX = x;

    for (char c : text) {
        if (c < 32 || c > 126) continue;

        int idx = c - 32;

        // Draw 5x7 character
        for (int row = 0; row < 7; row++) {
            unsigned char byte = FONT_5x7[idx][row];
            for (int col = 0; col < 5; col++) {
                if (byte & (1 << (4 - col))) {
                    int px = currentX + col;
                    int py = y + row;
                    if (px >= 0 && px < width && py >= 0) {
                        image[py * width + px] = 0; // Black
                    }
                }
            }
        }

        currentX += 6; // 5px char + 1px space
    }
}

std::vector<uint8_t> BarcodeGenerator::renderToBitmap(const std::vector<bool>& pattern,
    int& outWidth, int& outHeight) {
    int patternWidth = static_cast<int>(pattern.size()) * config.narrowBarWidth;

    // Calculate text area
    int textHeight = 0;

    if (config.showText && !lastData.empty()) {
        textHeight = 15; // Space for text
    }

    outWidth = patternWidth + 2 * config.margin;
    outHeight = config.height + textHeight + 2 * config.margin;

    // Create white image
    std::vector<uint8_t> image(outWidth * outHeight, 255);

    // Draw barcode
    for (size_t i = 0; i < pattern.size(); i++) {
        if (pattern[i]) { // Black bar
            int x = config.margin + static_cast<int>(i) * config.narrowBarWidth;
            for (int w = 0; w < config.narrowBarWidth; w++) {
                for (int y = config.margin; y < config.margin + config.height; y++) {
                    if (x + w < outWidth && y < outHeight) {
                        image[y * outWidth + (x + w)] = 0;
                    }
                }
            }
        }
    }

    // Draw text under barcode
    if (config.showText && !lastData.empty()) {
        int textX = config.margin;
        int textYPos = config.margin + config.height + 5;

        // Calculate text width
        int textWidth = static_cast<int>(lastData.length()) * 6;
        if (textWidth < patternWidth) {
            textX = config.margin + (patternWidth - textWidth) / 2;
        }

        drawTextSimple(image, outWidth, lastData, textX, textYPos);
    }

    return image;
}

bool BarcodeGenerator::writePNG(const std::vector<uint8_t>& image, int width, int height,
    const std::string& filename) {
    return stbi_write_png(filename.c_str(), width, height, 1, image.data(), width) != 0;
}

bool BarcodeGenerator::generate(const std::string& data, const std::string& outputFile) {
    if (!validateCode128A(data)) {
        return false;
    }

    lastData = data;

    std::vector<bool> pattern = generatePatterns(data);

    int width, height;
    std::vector<uint8_t> image = renderToBitmap(pattern, width, height);

    if (!writePNG(image, width, height, outputFile)) {
        lastError = "Failed to write PNG file: " + outputFile;
        return false;
    }

    return true;
} 