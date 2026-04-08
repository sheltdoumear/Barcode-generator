#include "BarcodeRenderer.h"
#include <memory>

BarcodeRenderer::BarcodeRenderer(int modWidth, int bHeight, int qZone, int txtHeight)
    : moduleWidth(modWidth), barHeight(bHeight), quietZone(qZone), textHeight(txtHeight) {
    imageWriter = nullptr;
}

void BarcodeRenderer::render(const BarcodeGenerator& barcode) {
    std::string pattern = barcode.getBinaryPattern();

    int width = static_cast<int>(pattern.length()) * moduleWidth + 2 * quietZone;
    int height = barHeight + textHeight;

    imageWriter = std::make_unique<ImageWriter>(width, height);
    imageWriter->clear(255, 255, 255);

    for (size_t i = 0; i < pattern.length(); ++i) {
        if (pattern[i] == '1') {
            int x = quietZone + static_cast<int>(i) * moduleWidth;
            imageWriter->drawRectangle(x, 0, moduleWidth, barHeight, 0, 0, 0);
        }
    }
}

bool BarcodeRenderer::save(const std::string& filepath) const {
    if (!imageWriter) {
        return false;
    }
    return imageWriter->savePNG(filepath);
}