#include "barcode_generator.h"
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
// #define STB_IMAGE_WRITE_IMPLEMENTATION
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

extern "C" {
    CODE128_API void* CreateGenerator(int barWidth, int height, int margin) {
        return new BarcodeGenerator(barWidth, height, margin);
    }

    CODE128_API void DestroyGenerator(void* generator) {
        delete static_cast<BarcodeGenerator*>(generator);
    }

    CODE128_API int GenerateBarcode(void* generator, const char* data, const char* filename) {
        if (!generator || !data || !filename) return 0;
        return static_cast<BarcodeGenerator*>(generator)->generate(data, filename) ? 1 : 0;
    }

    CODE128_API const char* GetBarcode128Error(void* generator) {
        if (!generator) return "Invalid generator";
        return static_cast<BarcodeGenerator*>(generator)->getLastError();
    }
}