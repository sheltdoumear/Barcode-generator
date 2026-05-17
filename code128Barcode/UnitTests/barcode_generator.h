#pragma once


#define CODE128_API

#ifdef __cplusplus
extern "C" {
#endif

    CODE128_API void* CreateGenerator(int barWidth, int height, int margin);
    CODE128_API void DestroyGenerator(void* generator);
    CODE128_API int GenerateBarcode(void* generator, const char* data, const char* filename);
    CODE128_API const char* GetLastError(void* generator);

#ifdef __cplusplus
}
#endif