#include "DataMatrixGenerator.h"

#include <iostream>

#ifdef _WIN32
#define DM_API extern "C" __declspec(dllexport)
#else
#define DM_API extern "C"
#endif

DM_API void* CreateDataMatrixGenerator() {
    return new DataMatrixGenerator();
}

DM_API void DestroyDataMatrixGenerator(void* generator) {
    delete static_cast<DataMatrixGenerator*>(generator);
}

DM_API int GenerateDataMatrix(
    void* generator,
    const char* data,
    const char* filename,
    int pixelSize
) {
    if (!generator || !data || !filename)
        return 0;

    try {
        auto* gen = static_cast<DataMatrixGenerator*>(generator);

        auto matrix = gen->generate(data);

        gen->saveToBitmap(matrix, filename, pixelSize);

        return 1;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}