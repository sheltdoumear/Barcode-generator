#pragma once

#ifdef _WIN32
#define DM_API extern "C"
#else
#define DM_API extern "C"
#endif

DM_API void* CreateDataMatrixGenerator();

DM_API void DestroyDataMatrixGenerator(
    void* generator
);

DM_API int GenerateDataMatrix(
    void* generator,
    const char* data,
    const char* filename,
    int pixelSize
);