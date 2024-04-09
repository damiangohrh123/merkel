#include "Candlestick.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>

// ANSI escape codes for text color
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_TEXT "\033[0m"

Candlestick::Candlestick(std::string _time, double _open, double _close, double _high, double _low)
: time(_time), open(_open), close(_close), high(_high), low(_low) {}

Candlestick Candlestick::computeCandlestick(const std::vector<OrderBookEntry>& entries, const std::vector<OrderBookEntry>& previousEntries) {

  // Get timestamp. Calculate open, close, high and low
  std::string time = entries[0].timestamp;
  double open = Candlestick::computeAveragePrice(previousEntries);
  double close = Candlestick::computeAveragePrice(entries);
  double high = Candlestick::computeHigh(entries);
  double low = Candlestick::computeLow(entries);

  Candlestick candlestick{time, open, close, high, low};
  return candlestick;
}

double Candlestick::computeAveragePrice(const std::vector<OrderBookEntry>& entries) {
  // Return 0 if no entries.
  if (entries.empty()) return 0.0; 

  double totalAmount = 0.0;
  double totalPrice = 0.0;

  // Calculate totalAmount and totalPrice.
  for (const OrderBookEntry& entry : entries) {
    totalPrice += entry.price * entry.amount;
    totalAmount += entry.amount;
  }

  // Return average.
  return (totalPrice / totalAmount);
}

double Candlestick::computeHigh(const std::vector<OrderBookEntry>& entries) {
  // Return 0 if no entries.
  if (entries.empty()) return 0.0;

  // Set maxPrice to first entry's price.
  double maxPrice = entries[0].price;

  // Iterate through entries and get highest price.
  for (const OrderBookEntry& entry : entries) {
    if (entry.price > maxPrice) maxPrice = entry.price;
  }
  return maxPrice;
}

double Candlestick::computeLow(const std::vector<OrderBookEntry>& entries) {
  // Return 0 if no entries.
  if (entries.empty()) return 0.0;

  // Set minPrice to first entry's price.
  double minPrice = entries[0].price;

  // Iterate thorugh entries and get lowest price.
  for (const OrderBookEntry& entry : entries) {
    if (entry.price < minPrice) minPrice = entry.price;
  }
  return minPrice;
}

void Candlestick::computeTextPlot(const std::vector<Candlestick>& candlesticks) {
  // Get highest high and lowest low to calculate scale of candlesticks.
  double highestHigh = Candlestick::getHighestHigh(candlesticks);
  double lowestLow = Candlestick::getLowestLow(candlesticks);
  double scale = (highestHigh - lowestLow) * 1.3;

  // Determine how many levels candlesticks have and the range per level.
  int levels = 30;
  double rangePerLevel = scale/levels;
  double currentRange = highestHigh + rangePerLevel;
  double previousRange = highestHigh + (2 * rangePerLevel);

  // Debugging purposes.
  std::cout << "Levels: " << levels << std::endl;
  std::cout << std::fixed << std::setprecision(10) << "Range per level: " << rangePerLevel << std::endl;
  std::cout << "highestHigh: " << highestHigh << std::endl;
  std::cout << "lowestLow: " << lowestLow << std::endl;
  std::cout << "=============================================================================================" <<
               "=============================================================================================" << std::endl;

  // Iterate through levels.
  for (int i = 0; i < levels; i++) {
    // Y-axis.
    std::cout << std::setw(12) << currentRange;

    // Iterate through candlesticks.
    for (const Candlestick& candlestick : candlesticks) {
      Candlestick::drawCandlestick(candlestick.open, candlestick.close, candlestick.high, candlestick.low, currentRange, previousRange);
    }

    // Update ranges.
    currentRange -= rangePerLevel;
    previousRange -= rangePerLevel;

    // Move to the next vertical level.
    std::cout << std::endl;
  }

  // Print the time labels at the bottom.
  std::cout << std::setw(12) << "Time";
  for (const Candlestick& candle : candlesticks) {
      std::cout << std::setw(30) << candle.time;
  }
  std::cout << std::endl;
}

void Candlestick::drawCandlestick(double open, double close, double high, double low, double currentRange, double previousRange) {
  std::string character = " ";  

  // Determine the character to be drawn at the current vertical level.
  if (high >= currentRange && high <= previousRange) { // Candlestick wick (High).
    character = "|               ";
  } else if (close >= currentRange && close <= previousRange) { // Candlestick wick (Low).
    character = "--               ";
  } else if (open >= currentRange && open <= previousRange) { // Open.
    character = "--               ";
  } else if (low >= currentRange && low <= previousRange) { // Close.
    character = "|               ";
  } else if ((currentRange > open && currentRange < close) || (currentRange > close && currentRange < open)) { // Between open and close (Candlestick body).
    character = "|      |            "; 
  } else if ((currentRange > open && currentRange < high) || (currentRange > close && currentRange < high)) { // Between high and open, or high and close.
    character = "|               ";
  } else if ((currentRange > low && currentRange < close) || (currentRange > low && currentRange < open)) { // Between low and open, or low and close.
    character = "|               ";
  }

  // Original console out code if color changing does not work. Uncomment this code and comment out color changing code below.
  // std::cout << RED_TEXT << std::setw(30) << character << RESET_TEXT;

  // Color changing code. Red color if close is lesser than open. Otherwise, green color of open is less than close.
  if (close < open) std::cout << RED_TEXT << std::setw(30) << character << RESET_TEXT;
  else std::cout << GREEN_TEXT << std::setw(30) << character << RESET_TEXT;
}

double Candlestick::getHighestHigh(const std::vector<Candlestick>& candlesticks) {
  // Return 0 if no entries
  if (candlesticks.empty()) return 0.0;

  double highestHigh = candlesticks[0].high;

  // Iterate through candlesticks and get highest high
  for (const Candlestick& candlestick : candlesticks) {
    if (candlestick.high > highestHigh) highestHigh = candlestick.high;
  }
  return highestHigh;
}

double Candlestick::getLowestLow(const std::vector<Candlestick>& candlesticks) {
  // Return 0 if no entries
  if (candlesticks.empty()) return 0.0;

  double lowestLow = candlesticks[0].high;

  // Iterate through candlesticks and get lowest low
  for (const Candlestick& candlestick : candlesticks) {
    if (candlestick.low < lowestLow) lowestLow = candlestick.low;
  }
  return lowestLow;
}
