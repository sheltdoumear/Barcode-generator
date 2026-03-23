#include <iostream>
#include "Code39.h"

int main() {
    Code39 code;

    auto barcode = code.encode("HELLO");

    for (int pixel : barcode) {
        std::cout << (pixel ? "#" : " ");
    }

    std::cout << std::endl;
}