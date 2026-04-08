#ifndef BARCODE_RENDERER_H
#define BARCODE_RENDERER_H

#include "BarcodeGenerator.h"
#include "ImageWriter.h"
#include <memory>

class BarcodeRenderer {
private:
    std::unique_ptr<ImageWriter> imageWriter;

    // Параметры рендеринга
    int moduleWidth;
    int barHeight;
    int quietZone;
    int textHeight;

public:
    BarcodeRenderer(int modWidth = 2, int bHeight = 200,
        int qZone = 20, int txtHeight = 30);
    ~BarcodeRenderer() = default;

    void render(const BarcodeGenerator& barcode);
    bool save(const std::string& filepath) const;

    // Геттеры и сеттеры для параметров
    void setModuleWidth(int width) { moduleWidth = width; }
    void setBarHeight(int height) { barHeight = height; }
    void setQuietZone(int zone) { quietZone = zone; }

    const ImageWriter& getImageWriter() const { return *imageWriter; }
};

#endif
