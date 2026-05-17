#pragma once

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C" {

    DLL_EXPORT int GenerateEAN13PNG(
        const char* code,
        const char* filename,
        int moduleWidth,
        int barHeight
    );

}