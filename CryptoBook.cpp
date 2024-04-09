#include "CryptoBook.h"
#include <iostream>
#include <iomanip>

/** Construct, reading a csv data file */
CryptoBook::CryptoBook(const std::string& filename) {
  entries = CSVReader::readCryptoCSV(filename);
  currentPosition = 0;
}

void CryptoBook::printGraph(const std::string& dataType, const int& iterations) {
  // Get the type of data according to the user.
  std::vector<double> datasAll = CryptoBook::getDataType(dataType);

  // Extract the first few elements based on iterations and currentPosition.
  // Limit iterations to size of data.
  size_t dataSize = datasAll.size();
  size_t endPosition = currentPosition + iterations;
  if (endPosition > dataSize) endPosition = dataSize;
  
  std::vector<double> datas(datasAll.begin() + currentPosition, datasAll.begin() + endPosition);
  currentPosition = endPosition; // Update position.

  // Get highest value to calculate scale of bar graph.
  double highestValue = CryptoBook::getHighest(datas);
  double scale = highestValue * 1.02;

  // Determine how many levels the bar graph will have and the range per level.
  int levels = 30;
  double rangePerLevel = scale/levels;
  double currentRange = scale;
  double previousRange = scale + rangePerLevel;

  // Debugging purposes.
  std::cout << "Levels: " << levels << std::endl;
  std::cout << std::fixed << std::setprecision(10) << "Range per level: " << rangePerLevel << std::endl;
  std::cout << "highest Value: " << highestValue << std::endl;
  std::cout << "=============================================================================================" <<
               "=============================================================================================" << std::endl;

  // Iterate through the levels.
  for (int i = 0; i < levels; i++) {
    // Y-axis.
    std::cout << std::setw(15) << currentRange;

    // Iterate through bars.
    for (const double& data : datas) {
      CryptoBook::drawBars(data, currentRange, previousRange);
    }
    currentRange -= rangePerLevel;
    previousRange -= rangePerLevel;
    
    // Move to the next vertical level.
    std::cout << std::endl;
  }

  // Print the time labels at the bottom.
  int count = 0;
  std::cout << std::setw(15) << "Time";
  for (const CryptoEntry& entry : entries) {
    if (count < iterations) {
      std::cout << std::setw(12) << entry.timestamp;
      count++;
    } else {
      break;
    }
  }
  std::cout << std::endl;
}

void CryptoBook::drawBars(double data, double currentRange, double previousRange) {
  std::string character = " ";

  // Determine the character to be drawn at the current vertical level.
  if (data >= currentRange) character = "|||||  ";

  std::cout << std::setw(12) << character;
}

std::vector<double> CryptoBook::getDataType(const std::string& dataType) {
  std::vector<double> values;

  for (const CryptoEntry& entry : entries) {
    double value;

    // Determine the value based on user's choice
    if (dataType == "open") value = entry.open;
    else if (dataType == "high") value = entry.high;
    else if (dataType == "low") value = entry.low;
    else if (dataType == "close") value = entry.close;
    else {
      std::cout << "CryptoBook::CryptoBook Invalid data type! " << std::endl;
      return values;
    }
    values.push_back(value);
  }
  return values;
}

double CryptoBook::getHighest(const std::vector<double>& values) {
  // Return 0 if values is empty
  if (values.empty()) return 0.0;

  double highestValue = values[0];

  // Iterate through values and get highest value
  for (const double& value : values) {
    if (value > highestValue) highestValue = value;
  }
  return highestValue;
}