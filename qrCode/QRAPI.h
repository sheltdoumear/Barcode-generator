#pragma once

#ifdef _WIN32
    #ifdef BARCODELIB_EXPORTS
        #define BARCODE_API __declspec(dllexport)
    #else
        #define BARCODE_API __declspec(dllimport)
    #endif
#else
    #define BARCODE_API
#endif

extern "C" {

    BARCODE_API int generateQRCodePNG(
        const char* text,
        const char* filename,
        int scale
    );

    BARCODE_API int generateQRCodeBMP(
        const char* text,
        const char* filename,
        int scale
    );

    BARCODE_API int generateQRCodeTGA(
        const char* text,
        const char* filename,
        int scale
    );

}