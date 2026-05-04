#ifndef QR_CODE_GENERATOR_H
#define QR_CODE_GENERATOR_H

#include <string>
#include <vector>
#include <cstdint>


enum class EccLevel {
    LOW,      // 7%
    MEDIUM,   // 15%
    QUARTILE, // 25%
    HIGH      // 30%
};
enum class ImageFormat {
    PNG,
    BMP,
    TGA
};
class QRCodeGenerator {
public:
    static bool generateToFile(const std::string& data,
                               const std::string& filename,
                               int scale = 10,
                               EccLevel ecc = EccLevel::MEDIUM,
                               ImageFormat format = ImageFormat::PNG);
    static std::vector<uint8_t> generateBitmap(const std::string& data,
                                               int& outWidth,
                                               int& outHeight,
                                               int scale = 10,
                                               EccLevel ecc = EccLevel::MEDIUM);
};

#endif // QR_CODE_GENERATOR_H