#pragma once

#ifdef _WIN32
    #define BARCODE_API extern "C" __declspec(dllexport)
#else
    #define BARCODE_API extern "C"
#endif

BARCODE_API bool generateBarcodePNG(
    const char* text,
    const char* filename,
    int height,
    int margin,
    int scale,
    int narrow,
    int wide
);

BARCODE_API bool generateBarcodeSVG(
    const char* text,
    const char* filename,
    int height,
    int margin,
    int scale,
    int narrow,
    int wide
);

BARCODE_API bool generateBarcodeBMP(
    const char* text,
    const char* filename,
    int height,
    int margin,
    int narrow,
    int wide
);

BARCODE_API const char* code39GetLastError();