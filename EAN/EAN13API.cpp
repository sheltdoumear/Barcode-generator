#include "EAN13API.h"

#include "EAN13Code.h"
#include "BarcodeRenderer.h"

extern "C" {

    int GenerateEAN13PNG(
        const char* code,
        const char* filename,
        int moduleWidth,
        int barHeight
    ) {
        try {

            EAN13Code barcode;

            if (!barcode.setData(code)) {
                return 0;
            }

            BarcodeRenderer renderer(moduleWidth, barHeight);

            renderer.render(barcode);

            return renderer.save(filename) ? 1 : 0;

        }
        catch (...) {
            return 0;
        }
    }

}