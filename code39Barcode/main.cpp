#include <iostream>
#include "Code39.h"
#include "BMPWriter.h"
#include "BarcodeImage.h"

int main() {
    Code39 code;
    BMPWriter writer;
    BarcodeImage renderer;


    int narrow = 2;
    int wide = narrow * 3;
    int scale = 3;

    auto barcode = code.encode("WIKIPEDIA", narrow, wide);

    renderer.savePNG("barcode.png", barcode, 120, 20, scale);
    renderer.saveSVG("barcode.svg", barcode, 120, 20, scale);

    std::cout << "all okay" << std::endl;

    writer.save("barcode.bmp", barcode, 100, 10);


}