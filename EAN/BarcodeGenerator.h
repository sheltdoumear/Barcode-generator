#ifndef BARCODE_GENERATOR_H
#define BARCODE_GENERATOR_H

#include <string>
#include <vector>

class BarcodeGenerator {
protected:
    std::string fullCode;
    std::string binaryPattern;

    virtual void buildBinaryPattern() = 0;

public:
    virtual ~BarcodeGenerator() = default;
    virtual bool setData(const std::string& data) = 0;
    virtual std::string getCode() const { return fullCode; }
    virtual std::string getBinaryPattern() const { return binaryPattern; }
    virtual int getCodeLength() const = 0;
};

#endif
