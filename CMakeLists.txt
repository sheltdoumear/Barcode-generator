cmake_minimum_required(VERSION 3.20)
project(BarcodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BarcodeGenerator
                                   # все .cpp файлы
        code39Barcode/implementation/Code39.cpp
        code39Barcode/main.cpp
        code39Barcode/header/BMPWriter.h
        code39Barcode/implementation/BMPWriter.cpp
        code39Barcode/header/stb_image_write.h
        code39Barcode/header/BarcodeImage.h
        code39Barcode/implementation/BarcodeImage.cpp
)

target_include_directories(BarcodeGenerator PRIVATE
        code39Barcode/header
)