#pragma once

#ifdef _WIN32
    #define BARCODE_API extern "C" __declspec(dllexport)
#else
    #define BARCODE_API extern "C"
#endif

BARCODE_API void generateBarcodePNG(
    const char* text,
    const char* filename
);

BARCODE_API void generateBarcodeSVG(
    const char* text,
    const char* filename
);

BARCODE_API void generateBarcodeBMP(
    const char* text,
    const char* filename
);