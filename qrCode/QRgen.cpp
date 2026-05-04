#include "qr_code_generator.h"

int main() {
    QRCodeGenerator::generateToFile("Hello, World!", "my_qrcode123123123.png", 10, EccLevel::MEDIUM, ImageFormat::PNG);

    return 0;
}