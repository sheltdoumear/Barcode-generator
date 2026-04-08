#ifndef EAN13_CODE_H
#define EAN13_CODE_H

#include "BarcodeGenerator.h"
#include <string>

class EAN13Code : public BarcodeGenerator {
private:
    static const std::string L_CODES[10];
    static const std::string G_CODES[10];
    static const std::string R_CODES[10];
    static const std::string PARITY_MAP[10];

    int calculateChecksum(const std::string& code12) const;
    std::string getPatternForDigit(int digit, char encodingSet) const;

protected:
    void buildBinaryPattern() override;

public:
    EAN13Code();
    ~EAN13Code();

    bool setData(const std::string& code12) override;
    bool setData(const std::string& prefix, const std::string& manufacturer,
        const std::string& product);
    int getCodeLength() const override { return 13; }

    std::string getPrefix() const;
    std::string getManufacturerCode() const;
    std::string getProductCode() const;
    int getChecksum() const;
};

#endif