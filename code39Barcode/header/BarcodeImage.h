#pragma once
#include <vector>
#include <string>

class BarcodeImage {
public:
    void savePNG(const std::string& filename,
                 const std::vector<int>& barcode,
                 int height,
                 int margin,
                 int scale);

    void saveSVG(const std::string& filename,
                 const std::vector<int>& barcode,
                 int height,
                 int margin,
                 int scale);
};