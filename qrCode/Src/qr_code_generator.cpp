#include "qr_code_generator.h"
#include "qrcodegen.hpp"       
#include "stb_image_write.h"      

#include <stdexcept>

static qrcodegen::QrCode::Ecc toNayukiEcc(EccLevel ecc) {
    switch (ecc) {
        case EccLevel::LOW:      return qrcodegen::QrCode::Ecc::LOW;
        case EccLevel::MEDIUM:   return qrcodegen::QrCode::Ecc::MEDIUM;
        case EccLevel::QUARTILE: return qrcodegen::QrCode::Ecc::QUARTILE;
        case EccLevel::HIGH:     return qrcodegen::QrCode::Ecc::HIGH;
        default:                 return qrcodegen::QrCode::Ecc::MEDIUM;
    }
}

std::vector<uint8_t> QRCodeGenerator::generateBitmap(const std::string& data,
                                                     int& outWidth,
                                                     int& outHeight,
                                                     int scale,
                                                     EccLevel ecc) {
    if (scale <= 0) scale = 1;
    if (data.empty())
        throw std::invalid_argument("Data for QR code cannot be empty");

  
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.c_str(), toNayukiEcc(ecc));

    int size = qr.getSize();              
    outWidth = size * scale;
    outHeight = size * scale;

    std::vector<uint8_t> bitmap(outWidth * outHeight, 255); 


    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (qr.getModule(x, y)) {     
                for (int dy = 0; dy < scale; ++dy) {
                    for (int dx = 0; dx < scale; ++dx) {
                        int px = x * scale + dx;
                        int py = y * scale + dy;
                        bitmap[py * outWidth + px] = 0; 
                    }
                }
            }
        }
    }
    return bitmap;
}

bool QRCodeGenerator::generateToFile(const std::string& data,
                                     const std::string& filename,
                                     int scale,
                                     EccLevel ecc,
                                     ImageFormat format) {
    try {
        int width, height;
        auto bitmap = generateBitmap(data, width, height, scale, ecc);

        int success = 0;
        switch (format) {
            case ImageFormat::PNG:
                success = stbi_write_png(filename.c_str(), width, height, 1, bitmap.data(), width);
                break;
            case ImageFormat::BMP:
                success = stbi_write_bmp(filename.c_str(), width, height, 1, bitmap.data());
                break;
            case ImageFormat::TGA:
                success = stbi_write_tga(filename.c_str(), width, height, 1, bitmap.data());
                break;
            default:
                return false;
        }
        return success != 0;
    } catch (const std::exception&) {
        return false;
    }
}