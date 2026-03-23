cmake_minimum_required(VERSION 3.20)
project(BarcodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BarcodeGenerator
                                   # все .cpp файлы
        code39Barcode/realization/Code39.cpp
        code39Barcode/main.cpp
)

target_include_directories(BarcodeGenerator PRIVATE
        code39Barcode/header
)