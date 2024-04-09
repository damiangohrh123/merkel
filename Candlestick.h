#pragma once
#include <string>
#include <vector>
#include <set>
#include "OrderBook.h"
#include "CSVReader.h"

class Candlestick {
  public:
    std::string time;
    double open;
    double close;
    double high;  
    double low;

    /** Candlestick object*/
    Candlestick(std::string time, double open, double close, double high, double low);

    /** Calculate candlestick data. Open, close, high, and low. */
    static Candlestick computeCandlestick(const std::vector<OrderBookEntry>& combinedEntries, const std::vector<OrderBookEntry>& previousEntries);

    /** Generate text-plot for the candlestick data */
    static void computeTextPlot(const std::vector<Candlestick>& candlesticks);
  
  private:
    /** Calculate average price of entries. */
    static double computeAveragePrice(const std::vector<OrderBookEntry>& entries);
    /** Return highest price in entries. */
    static double computeHigh(const std::vector<OrderBookEntry>& entries);
    /** Return lowest price in entries. */
    static double computeLow(const std::vector<OrderBookEntry>& entries);

    /** Get the highest amount of high among candlesticks. */
    static double getHighestHigh(const std::vector<Candlestick>& candlesticks);
    /** Get the lowest amount of low among candlesticks. */
    static double getLowestLow(const std::vector<Candlestick>& candlesticks);

    static void drawCandlestick(double open, double close, double high, double low, double lowerRange, double higherRange);
};