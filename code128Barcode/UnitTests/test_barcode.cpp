
#include "barcode_generator.h"
#include <iostream>
#include <cstring>
#include <cstdio>   


#define TEST(name) \
    std::cout << "  TEST: " << name << " ... "; \
    try {

#define TEST_END \
        std::cout << "PASSED" << std::endl; \
    } catch (const std::exception& e) { \
        std::cout << "FAILED (" << e.what() << ")" << std::endl; \
    }

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "=== Code 128 Barcode Generator Tests ===" << std::endl << std::endl;

    int passed = 0;
    int total = 0;

    // ---------------------------------------------------------------
    // Тест 1: Создание и уничтожение генератора
    // ---------------------------------------------------------------
    std::cout << "[1] Создание/уничтожение генератора" << std::endl;
    TEST("CreateGenerator с корректными параметрами")
        void* gen = CreateGenerator(3, 100, 10);
    if (gen == nullptr) throw std::runtime_error("генератор не создан");
    DestroyGenerator(gen);
    TEST_END

        TEST("CreateGenerator с нулевой шириной")
        void* gen = CreateGenerator(0, 100, 10);
    if (gen == nullptr) throw std::runtime_error("генератор не создан (ожидался объект)");
    DestroyGenerator(gen);
    TEST_END

        // ---------------------------------------------------------------
        // Тест 2: Генерация простого штрихкода
        // ---------------------------------------------------------------
        std::cout << "[2] Генерация штрихкода" << std::endl;
    TEST("Генерация с простой строкой")
        void* gen = CreateGenerator(2, 50, 5);
    int result = GenerateBarcode(gen, "12345", "test_simple.png");
    if (result == 0) {
        const char* err = GetLastError(gen);
        throw std::runtime_error(std::string("ошибка генерации: ") + err);
    }
    // Проверяем, что файл создался
    FILE* f = fopen("test_simple.png", "rb");
    if (!f) throw std::runtime_error("файл не создан");
    fclose(f);
    remove("test_simple.png");
    DestroyGenerator(gen);
    TEST_END

        TEST("Генерация с одной буквой")
        void* gen = CreateGenerator(2, 50, 5);
    int result = GenerateBarcode(gen, "A", "test_single.png");
    if (result == 0) {
        const char* err = GetLastError(gen);
        throw std::runtime_error(std::string("ошибка: ") + err);
    }
    remove("test_single.png");
    DestroyGenerator(gen);
    TEST_END

        // ---------------------------------------------------------------
        // Тест 3: Разные типы данных
        // ---------------------------------------------------------------
        std::cout << "[3] Разные типы данных" << std::endl;
    TEST("Цифры")
        void* gen = CreateGenerator(2, 50, 5);
    if (!GenerateBarcode(gen, "9876543210", "test_digits.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_digits.png");
    DestroyGenerator(gen);
    TEST_END

        TEST("Буквы верхнего регистра")
        void* gen = CreateGenerator(2, 50, 5);
    if (!GenerateBarcode(gen, "HELLO", "test_upper.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_upper.png");
    DestroyGenerator(gen);
    TEST_END

        TEST("Спецсимволы")
        void* gen = CreateGenerator(2, 50, 5);
    if (!GenerateBarcode(gen, "CODE-128", "test_special.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_special.png");
    DestroyGenerator(gen);
    TEST_END

        TEST("Пробелы в строке")
        void* gen = CreateGenerator(2, 50, 5);
    if (!GenerateBarcode(gen, "A B C", "test_spaces.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_spaces.png");
    DestroyGenerator(gen);
    TEST_END

        // ---------------------------------------------------------------
        // Тест 4: Обработка ошибок
        // ---------------------------------------------------------------
        std::cout << "[4] Обработка ошибок" << std::endl;
    TEST("Пустая строка данных")
        void* gen = CreateGenerator(2, 50, 5);
    int result = GenerateBarcode(gen, "", "test_empty.png");
    if (result != 0) throw std::runtime_error("ожидалась ошибка, но генерация прошла успешно");
    const char* err = GetLastError(gen);
    if (strlen(err) == 0) throw std::runtime_error("сообщение об ошибке пустое");
    std::cout << "(ошибка: \"" << err << "\") ... ";
    DestroyGenerator(gen);
    TEST_END

        TEST("NULL вместо данных")
        void* gen = CreateGenerator(2, 50, 5);
    int result = GenerateBarcode(gen, nullptr, "test_null.png");
    if (result != 0) throw std::runtime_error("ожидалась ошибка для NULL-данных");
    DestroyGenerator(gen);
    TEST_END

        TEST("NULL вместо имени файла")
        void* gen = CreateGenerator(2, 50, 5);
    int result = GenerateBarcode(gen, "TEST", nullptr);
    if (result != 0) throw std::runtime_error("ожидалась ошибка для NULL-имени файла");
    DestroyGenerator(gen);
    TEST_END

        TEST("NULL-генератор")
        int result = GenerateBarcode(nullptr, "TEST", "test.png");
    if (result != 0) throw std::runtime_error("ожидалась ошибка для NULL-генератора");
    TEST_END

        TEST("GetLastError с NULL-генератором")
        const char* err = GetLastError(nullptr);
    if (!err || strlen(err) == 0) throw std::runtime_error("ожидалось сообщение об ошибке");
    std::cout << "(ошибка: \"" << err << "\") ... ";
    TEST_END

        // ---------------------------------------------------------------
        // Тест 5: Разные размеры штрихкода
        // ---------------------------------------------------------------
        std::cout << "[5] Разные размеры" << std::endl;
    TEST("Узкие полосы (barWidth=1)")
        void* gen = CreateGenerator(1, 30, 2);
    if (!GenerateBarcode(gen, "NARROW", "test_narrow.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_narrow.png");
    DestroyGenerator(gen);
    TEST_END

        TEST("Широкие полосы (barWidth=10)")
        void* gen = CreateGenerator(10, 100, 20);
    if (!GenerateBarcode(gen, "WIDE", "test_wide.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_wide.png");
    DestroyGenerator(gen);
    TEST_END

        TEST("Высокий штрихкод")
        void* gen = CreateGenerator(3, 300, 10);
    if (!GenerateBarcode(gen, "TALL", "test_tall.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_tall.png");
    DestroyGenerator(gen);
    TEST_END

        TEST("Нулевой отступ (margin=0)")
        void* gen = CreateGenerator(2, 50, 0);
    if (!GenerateBarcode(gen, "NOMARGIN", "test_nomargin.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_nomargin.png");
    DestroyGenerator(gen);
    TEST_END

        // ---------------------------------------------------------------
        // Тест 6: Длинные строки и контрольная сумма
        // ---------------------------------------------------------------
        std::cout << "[6] Длинные данные" << std::endl;
    TEST("Длинная строка (50 символов)")
        void* gen = CreateGenerator(2, 50, 5);
    std::string longStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMN";
    if (!GenerateBarcode(gen, longStr.c_str(), "test_long.png"))
        throw std::runtime_error(GetLastError(gen));
    remove("test_long.png");
    DestroyGenerator(gen);
    TEST_END

        // ---------------------------------------------------------------
        // Тест 7: Множественные генераторы одновременно
        // ---------------------------------------------------------------
        std::cout << "[7] Множественные генераторы" << std::endl;
    TEST("Два генератора одновременно")
        void* gen1 = CreateGenerator(2, 50, 5);
    void* gen2 = CreateGenerator(3, 80, 15);

    int r1 = GenerateBarcode(gen1, "FIRST", "test_first.png");
    int r2 = GenerateBarcode(gen2, "SECOND", "test_second.png");

    if (!r1 || !r2) {
        if (!r1) std::cout << "(ошибка gen1: " << GetLastError(gen1) << ") ";
        if (!r2) std::cout << "(ошибка gen2: " << GetLastError(gen2) << ") ";
        throw std::runtime_error("один из генераторов не сработал");
    }

    remove("test_first.png");
    remove("test_second.png");
    DestroyGenerator(gen1);
    DestroyGenerator(gen2);
    TEST_END

        // ---------------------------------------------------------------
        // Тест 8: Повторное использование генератора
        // ---------------------------------------------------------------
        std::cout << "[8] Повторное использование" << std::endl;
    TEST("Три генерации одним генератором")
        void* gen = CreateGenerator(2, 50, 5);
    if (!GenerateBarcode(gen, "ONE", "test_one.png"))
        throw std::runtime_error(std::string("ошибка 1: ") + GetLastError(gen));
    remove("test_one.png");

    if (!GenerateBarcode(gen, "TWO", "test_two.png"))
        throw std::runtime_error(std::string("ошибка 2: ") + GetLastError(gen));
    remove("test_two.png");

    if (!GenerateBarcode(gen, "THREE", "test_three.png"))
        throw std::runtime_error(std::string("ошибка 3: ") + GetLastError(gen));
    remove("test_three.png");

    DestroyGenerator(gen);
    TEST_END

        std::cout << std::endl << "=== Все тесты завершены ===" << std::endl;
    return 0;
}