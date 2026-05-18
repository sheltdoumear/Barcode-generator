#include "UnifiedBarcodeAPI.h"



#include "code39Barcode//header/BarcodeAPI.h"


#include "code128Barcode/barcode_generator.h"


#include "qrCode/QRAPI.h"


#include "dataMatrix/DataMatrixGenerator.h"
#include "dataMatrix/DataMatrixAPI.h"

#include "EAN/EAN13API.h"



int GenerateCode39(
    const char* text,
    const char* outputPath
) {
    return generateBarcodePNG(
        text,
        outputPath,
        100, // height
        10,  // margin
        2,   // scale
        2,   // narrow
        4    // wide
    );
}



int GenerateCode128(
    const char* text,
    const char* outputPath
) {

    void* generator = CreateGenerator(
        2,    // bar width
        100,  // height
        10    // margin
    );

    if (!generator)
        return 0;

    int result = GenerateBarcode(
        generator,
        text,
        outputPath
    );

    DestroyGenerator(generator);

    return result;
}



int GenerateQRCode(
    const char* text,
    const char* outputPath
) {
    return generateQRCodePNG(
        text,
        outputPath,
        10
    );
}


int GenerateDataMatrixCode(
    const char* text,
    const char* outputPath
) {

    void* generator = CreateDataMatrixGenerator();

    if (!generator)
        return 0;

    int result = GenerateDataMatrix(
        generator,
        text,
        outputPath,
        10
    );

    DestroyDataMatrixGenerator(generator);

    return result;
}



int GenerateEAN13Code(
    const char* text,
    const char* outputPath
) {

    return GenerateEAN13PNG(
        text,
        outputPath,
        2,
        120
    );
}