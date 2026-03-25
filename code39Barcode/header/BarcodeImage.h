#pragma once
#include <vector>
#include <string>

class BarcodeImage {
public:
    void savePNG(const std::string& filename,
                 const std::vector<int>& barcode,
                 int height,
                 int margin);

    void saveSVG(const std::string& filename,
                 const std::vector<int>& barcode,
                 int height,
                 int margin);
};