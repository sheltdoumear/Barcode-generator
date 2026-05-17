#include "BarcodeAPI.h"

#include "Code39.h"
#include "BMPWriter.h"
#include "BarcodeImage.h"

#include <string>

static std::string lastError;

// error

const char* code39GetLastError()
{
    return lastError.c_str();
}



static bool buildBarcode(
    const char* text,
    std::vector<int>& output,
    int narrow,
    int wide
)
{
    try {

        lastError.clear();

        Code39 code;

        output = code.encode(text, narrow, wide);

        return true;
    }
    catch (const std::exception& e) {

        lastError = e.what();
        return false;
    }
    catch (...) {

        lastError = "Unknown error";
        return false;
    }
}

// png

bool generateBarcodePNG(
    const char* text,
    const char* filename,
    int height,
    int margin,
    int scale,
    int narrow,
    int wide
)
{
    try {

        std::vector<int> barcode;

        if (!buildBarcode(text, barcode, narrow, wide))
            return false;

        BarcodeImage renderer;

        renderer.savePNG(
            filename,
            barcode,
            height,
            margin,
            scale
        );

        return true;
    }
    catch (const std::exception& e) {

        lastError = e.what();
        return false;
    }
    catch (...) {

        lastError = "Unknown error";
        return false;
    }
}

// svg

bool generateBarcodeSVG(
    const char* text,
    const char* filename,
    int height,
    int margin,
    int scale,
    int narrow,
    int wide
)
{
    try {

        std::vector<int> barcode;

        if (!buildBarcode(text, barcode, narrow, wide))
            return false;

        BarcodeImage renderer;

        renderer.saveSVG(
            filename,
            barcode,
            height,
            margin,
            scale
        );

        return true;
    }
    catch (const std::exception& e) {

        lastError = e.what();
        return false;
    }
    catch (...) {

        lastError = "Unknown error";
        return false;
    }
}

// bmp

bool generateBarcodeBMP(
    const char* text,
    const char* filename,
    int height,
    int margin,
    int narrow,
    int wide
)
{
    try {

        std::vector<int> barcode;

        if (!buildBarcode(text, barcode, narrow, wide))
            return false;

        BMPWriter writer;

        writer.save(
            filename,
            barcode,
            height,
            margin
        );

        return true;
    }
    catch (const std::exception& e) {

        lastError = e.what();
        return false;
    }
    catch (...) {

        lastError = "Unknown error";
        return false;
    }
}