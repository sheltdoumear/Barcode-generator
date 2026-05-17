#include "QRAPI.h"
#include "qr_code_generator.h"

extern "C" {

    int generateQRCodePNG(
        const char* text,
        const char* filename,
        int scale
    ) {
        return QRCodeGenerator::generateToFile(
            text,
            filename,
            scale,
            EccLevel::MEDIUM,
            ImageFormat::PNG
        );
    }

    int generateQRCodeBMP(
        const char* text,
        const char* filename,
        int scale
    ) {
        return QRCodeGenerator::generateToFile(
            text,
            filename,
            scale,
            EccLevel::MEDIUM,
            ImageFormat::BMP
        );
    }

    int generateQRCodeTGA(
        const char* text,
        const char* filename,
        int scale
    ) {
        return QRCodeGenerator::generateToFile(
            text,
            filename,
            scale,
            EccLevel::MEDIUM,
            ImageFormat::TGA
        );
    }

}