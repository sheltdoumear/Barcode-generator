cmake_minimum_required(VERSION 3.20)

project(BarcodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(BarcodeGenerator SHARED

        # code39
        code39Barcode/BarcodeAPI.cpp
        code39Barcode/implementation/Code39.cpp
        code39Barcode/implementation/BMPWriter.cpp
        code39Barcode/implementation/BarcodeImage.cpp

        # code128
        code128Barcode/barcode_generator.cpp

        # qr code
        qrCode/qr_code_generator.cpp
        qrCode/qrcodegen.cpp
        qrCode/QRAPI.h
        qrCode/QRAPI.cpp
        dataMatrix/DataMatrixAPI.cpp

        # data matrix
        dataMatrix/DataMatrixAPI.cpp
        dataMatrix/DataMatrixGenerator.cpp
        EAN/EAN13API.h
        EAN/EAN13API.cpp

        # ean
        EAN/BarcodeRenderer.cpp
        EAN/EAN13API.cpp
        EAN/EAN13Code.cpp
        EAN/ImageWriter.cpp

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
        qrCode
        dataMatrix
        EAN
)

target_compile_definitions(BarcodeGenerator PRIVATE
        BARCODELIB_EXPORTS
        CODE128_EXPORTS
)