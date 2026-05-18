#pragma once

#ifdef _WIN32
#define UNIFIED_API extern "C" __declspec(dllexport)
#else
#define UNIFIED_API extern "C"
#endif

UNIFIED_API int GenerateCode39(
    const char* text,
    const char* outputPath
);

UNIFIED_API int GenerateCode128(
    const char* text,
    const char* outputPath
);

UNIFIED_API int GenerateQRCode(
    const char* text,
    const char* outputPath
);

UNIFIED_API int GenerateDataMatrixCode(
    const char* text,
    const char* outputPath
);

UNIFIED_API int GenerateEAN13Code(
    const char* text,
    const char* outputPath
);