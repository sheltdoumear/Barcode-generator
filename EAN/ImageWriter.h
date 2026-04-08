#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>

class ImageWriter {
private:
    int width;
    int height;
    std::vector<unsigned char> imageData;

public:
    ImageWriter(int w, int h);
    ~ImageWriter() = default;

    void clear(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void drawRectangle(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
    bool savePNG(const std::string& filepath) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif#pragma once
