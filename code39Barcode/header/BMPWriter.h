#pragma once
#include <vector>
#include <string>

class BMPWriter {
public:
    void save(const std::string& filename,
              const std::vector<int>& barcode,
              int height,
              int margin);
};