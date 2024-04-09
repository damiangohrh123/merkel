#pragma once
#include "CSVReader.h"
#include "CryptoEntry.h"
#include <string>
#include <vector>

class CryptoBook {
  public:
    CryptoBook(const std::string& filename);
    std::vector<CryptoEntry> entries;

    void printGraph(const std::string& dataType, const int& iterations);

  private:
    std::vector<double> getDataType(const std::string& dataType);
    void drawBars(double value, double currentRange, double previousRange);
    double getHighest(const std::vector<double>& values);

    size_t currentPosition;
};
