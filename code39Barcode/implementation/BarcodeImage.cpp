#include "BarcodeImage.h"
#include <fstream>

// PNG
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


void BarcodeImage::savePNG(const std::string& filename,
                           const std::vector<int>& barcode,
                           int height,
                           int margin,
                           int scale)
{
    int logicalWidth = barcode.size() + 2 * margin;
    int width = logicalWidth * scale;

    std::vector<unsigned char> image(width * height * 3);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int logicalX = x / scale;
            bool isBlack = false;

            if (logicalX >= margin && logicalX < margin + barcode.size()) {
                isBlack = barcode[logicalX - margin] == 1;
            }

            int index = (y * width + x) * 3;

            unsigned char color = isBlack ? 0 : 255;

            image[index + 0] = color;
            image[index + 1] = color;
            image[index + 2] = color;
        }
    }

    stbi_write_png(filename.c_str(), width, height, 3,
                   image.data(), width * 3);
}

// SVG

void BarcodeImage::saveSVG(const std::string& filename,
                           const std::vector<int>& barcode,
                           int height,
                           int margin,
                           int scale)
{
    int logicalWidth = barcode.size() + 2 * margin;
    int width = logicalWidth * scale;


    std::ofstream file(filename);

    file << "<svg xmlns='http://www.w3.org/2000/svg' "
         << "width='" << width << "' height='" << height << "'>\n";

    file << "<rect width='100%' height='100%' fill='white'/>\n";

    for (int x = 0; x < barcode.size(); x++) {
        if (barcode[x] == 1) {
            file << "<rect x='" << (x + margin) * scale
                 << "' y='0' width='" << scale
                 << "' height='" << height
                 << "' fill='black'/>\n";
        }
    }

    file << "</svg>";
}