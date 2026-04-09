#include "qr_code_generator.h"
#include <iostream>
#include <fstream>

// Проверка, что файл существует и не пуст
bool fileExistsAndNotEmpty(const std::string& path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f.is_open()) return false;
    return f.tellg() > 0;
}

int main() {
    std::cout << "=== Тестирование генератора QR-кодов ===\n";

    // Тест 1: PNG с высоким уровнем коррекции
    bool ok = QRCodeGenerator::generateToFile("https://github.com/sheltdoumear/Barcode-generator",
                                              "test1.png", 10, EccLevel::HIGH, ImageFormat::PNG);
    if (ok && fileExistsAndNotEmpty("test1.png"))
        std::cout << "[OK] test1.png создан\n";
    else
        std::cout << "[FAIL] test1.png\n";

    // Тест 2: BMP с обычным текстом
    ok = QRCodeGenerator::generateToFile("Hello, World!", "test2.bmp", 8, EccLevel::MEDIUM, ImageFormat::BMP);
    if (ok && fileExistsAndNotEmpty("test2.bmp"))
        std::cout << "[OK] test2.bmp создан\n";
    else
        std::cout << "[FAIL] test2.bmp\n";

    // Тест 3: TGA с длинной строкой
    std::string longText(500, 'A');
    ok = QRCodeGenerator::generateToFile(longText, "test3.tga", 6, EccLevel::LOW, ImageFormat::TGA);
    if (ok && fileExistsAndNotEmpty("test3.tga"))
        std::cout << "[OK] test3.tga создан\n";
    else
        std::cout << "[FAIL] test3.tga\n";

    // Тест 4: получение битовой карты в памяти
    int w, h;
    auto bitmap = QRCodeGenerator::generateBitmap("Test123", w, h, 5);
    if (!bitmap.empty() && w > 0 && h > 0) {
        std::cout << "[OK] Битовая карта: " << w << "x" << h << "\n";
        // Проверим, есть ли чёрные пиксели
        bool hasBlack = false;
        for (auto p : bitmap) if (p == 0) { hasBlack = true; break; }
        std::cout << (hasBlack ? "[OK] Есть чёрные пиксели\n" : "[WARN] Нет чёрных пикселей\n");
    } else {
        std::cout << "[FAIL] Битовая карта\n";
    }

    // Тест 5: пустая строка (должна вызвать исключение, generateToFile вернёт false)
    ok = QRCodeGenerator::generateToFile("", "empty.png", 5);
    if (!ok)
        std::cout << "[OK] Пустая строка корректно отклонена\n";
    else
        std::cout << "[FAIL] Пустая строка не должна генерировать файл\n";

    std::cout << "=== Тестирование завершено ===\n";
    return 0;
}