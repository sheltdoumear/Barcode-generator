cmake_minimum_required(VERSION 3.10)
project(code128)

set(CMAKE_CXX_STANDARD 11)

add_compile_definitions(CODE128_EXPORTS _CRT_SECURE_NO_WARNINGS)

add_library(code128 SHARED barcode_generator.cpp)

target_include_directories(code128 PRIVATE .)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)