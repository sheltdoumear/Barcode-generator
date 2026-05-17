cmake_minimum_required(VERSION 3.20)

project(BarcodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(BarcodeGenerator SHARED

        code39Barcode/BarcodeAPI.cpp
        code39Barcode/implementation/Code39.cpp
        code39Barcode/implementation/BMPWriter.cpp
        code39Barcode/implementation/BarcodeImage.cpp

        code128Barcode/barcode_generator.cpp
)

target_link_options(BarcodeGenerator PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
        -Wl,--export-all-symbols
)

target_include_directories(BarcodeGenerator PRIVATE
        code39Barcode/header
        code128Barcode
)

target_compile_definitions(BarcodeGenerator PRIVATE
        BARCODELIB_EXPORTS
        CODE128_EXPORTS
)