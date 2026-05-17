#include "ImageWriter.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

ImageWriter::ImageWriter(int w, int h) : width(w), height(h) {
    imageData.resize(w * h * 3, 255);
}

void ImageWriter::clear(unsigned char r, unsigned char g, unsigned char b) {
    for (int i = 0; i < width * height; ++i) {
        imageData[i * 3] = r;
        imageData[i * 3 + 1] = g;
        imageData[i * 3 + 2] = b;
    }
}

void ImageWriter::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    int index = (y * width + x) * 3;
    imageData[index] = r;
    imageData[index + 1] = g;
    imageData[index + 2] = b;
}

void ImageWriter::drawRectangle(int x, int y, int w, int h,
    unsigned char r, unsigned char g, unsigned char b) {
    for (int dy = 0; dy < h; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            setPixel(x + dx, y + dy, r, g, b);
        }
    }
}

bool ImageWriter::savePNG(const std::string& filepath) const {
    return stbi_write_png(filepath.c_str(), width, height, 3,
        imageData.data(), width * 3) != 0;
}