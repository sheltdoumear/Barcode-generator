cmake_minimum_required(VERSION 3.20)
project(BarcodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BarcodeGenerator SHARED

        code39Barcode/BarcodeAPI.cpp

        code39Barcode/implementation/Code39.cpp
        code39Barcode/implementation/BMPWriter.cpp
        code39Barcode/implementation/BarcodeImage.cpp
)

target_link_options(BarcodeGenerator PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
)

target_include_directories(BarcodeGenerator PRIVATE
        code39Barcode/header
)
target_compile_definitions(BarcodeGenerator PRIVATE BARCODELIB_EXPORTS)