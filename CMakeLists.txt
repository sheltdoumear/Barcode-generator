cmake_minimum_required(VERSION 3.20)
project(BarcodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BarcodeGenerator
         # code 39
        code39Barcode/implementation/Code39.cpp
        code39Barcode/implementation/BMPWriter.cpp
        code39Barcode/implementation/BarcodeImage.cpp
        code39Barcode/main.cpp

        # code 128
        #code128Barcode/barcode_generator.cpp
        #code128Barcode/Main.cpp
)

target_include_directories(BarcodeGenerator PRIVATE
        code39Barcode/header
        #code128Barcode
)