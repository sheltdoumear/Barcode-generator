#include "BMPWriter.h"
#include <fstream>

void BMPWriter::save(const std::string& filename,
                     const std::vector<int>& barcode,
                     int height,
                     int margin) {

    int width = barcode.size() + 2 * margin;
    int rowSize = (width * 3 + 3) & (~3);
    int imageSize = rowSize * height;
    int fileSize = 54 + imageSize;

    std::ofstream file(filename, std::ios::binary);

    unsigned char fileHeader[14] = {
        'B','M',  // сигнатура
        0,0,0,0,  // размер файла
        0,0,0,0,  // reserved
        54,0,0,0  // offset до пикселей
    };

    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;

    unsigned char infoHeader[40] = {
        40,0,0,0,  // размер header
        0,0,0,0,   // width
        0,0,0,0,   // height
        1,0,       // planes
        24,0,      // bits per pixel
        0,0,0,0,   // compression
        0,0,0,0,   // image size
        0,0,0,0,   // x ppm
        0,0,0,0,   // y ppm
        0,0,0,0,   // colors
        0,0,0,0
    };

    infoHeader[4] = width;
    infoHeader[5] = width >> 8;
    infoHeader[6] = width >> 16;
    infoHeader[7] = width >> 24;

    infoHeader[8] = height;
    infoHeader[9] = height >> 8;
    infoHeader[10] = height >> 16;
    infoHeader[11] = height >> 24;

    infoHeader[20] = imageSize;
    infoHeader[21] = imageSize >> 8;
    infoHeader[22] = imageSize >> 16;
    infoHeader[23] = imageSize >> 24;

    file.write((char*)fileHeader, 14);
    file.write((char*)infoHeader, 40);

    int padding = rowSize - width * 3;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            bool isBlack = false;

            if (x >= margin && x < margin + barcode.size()) {
                isBlack = barcode[x - margin] == 1;
            }

            if (isBlack) {
                file.put(0);   // B
                file.put(0);   // G
                file.put(0);   // R
            } else {
                file.put(255);
                file.put(255);
                file.put(255);
            }
        }

        // padding
        for (int p = 0; p < padding; p++) {
            file.put(0);
        }
    }

    file.close();
}















