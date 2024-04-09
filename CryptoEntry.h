#pragma once

#include <string>
#include <vector>

class CryptoEntry {
  public:
    CryptoEntry(std::string _timestamp, double _open, double _high, double _low, double _close);

    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
};
