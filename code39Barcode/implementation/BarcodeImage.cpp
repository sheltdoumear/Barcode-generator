#include "BarcodeImage.h"
#include <fstream>

// PNG
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// ================= PNG =================

void BarcodeImage::savePNG(const std::string& filename,
                           const std::vector<int>& barcode,
                           int height,
                           int margin)
{
    int width = barcode.size() + 2 * margin;

    std::vector<unsigned char> image(width * height * 3);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            bool isBlack = false;

            if (x >= margin && x < margin + barcode.size())
                isBlack = barcode[x - margin] == 1;

            int index = (y * width + x) * 3;

            if (isBlack) {
                image[index + 0] = 0;
                image[index + 1] = 0;
                image[index + 2] = 0;
            } else {
                image[index + 0] = 255;
                image[index + 1] = 255;
                image[index + 2] = 255;
            }
        }
    }

    stbi_write_png(filename.c_str(), width, height, 3,
                   image.data(), width * 3);
}

// ================= SVG =================

void BarcodeImage::saveSVG(const std::string& filename,
                           const std::vector<int>& barcode,
                           int height,
                           int margin)
{
    int width = barcode.size() + 2 * margin;

    std::ofstream file(filename);

    file << "<svg xmlns='http://www.w3.org/2000/svg' "
         << "width='" << width << "' height='" << height << "'>\n";

    file << "<rect width='100%' height='100%' fill='white'/>\n";

    for (int x = 0; x < barcode.size(); x++) {
        if (barcode[x] == 1) {
            file << "<rect x='" << x + margin
                 << "' y='0' width='1' height='" << height
                 << "' fill='black'/>\n";
        }
    }

    file << "</svg>";
}