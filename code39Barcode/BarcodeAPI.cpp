#include "BarcodeAPI.h"

#include "Code39.h"
#include "BMPWriter.h"
#include "BarcodeImage.h"

#include <iostream>

void generateBarcodePNG(const char* text,
                        const char* filename)
{
    try {

        std::cout << "STEP 1\n";

        Code39 code;
        BarcodeImage renderer;

        std::cout << "STEP 2\n";

        int narrow = 2;
        int wide = 6;
        int scale = 3;

        auto barcode = code.encode(text, narrow, wide);

        std::cout << "STEP 3\n";

        renderer.savePNG(filename, barcode, 120, 20, scale);

        std::cout << "STEP 4\n";

    }
    catch (const std::exception& e) {

        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    catch (...) {

        std::cerr << "UNKNOWN ERROR\n";
    }
}