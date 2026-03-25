#include <iostream>
#include "Code39.h"
#include "BMPWriter.h"
#include "BarcodeImage.h"

int main() {
    Code39 code;
    BMPWriter writer;
    BarcodeImage renderer;


    auto barcode = code.encode("HELLO");

    renderer.savePNG("barcode.png", barcode, 120, 20);
    renderer.saveSVG("barcode.svg", barcode, 120, 20);

    //writer.save("barcode.bmp", barcode, 100, 10);

    // for (int pixel : barcode) {
    //     std::cout << (pixel ? "#" : " ");
    // }
    //
    // std::cout << std::endl;
}