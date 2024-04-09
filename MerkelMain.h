#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "Candlestick.h"
#include "CryptoEntry.h"
#include "CryptoBook.h"

class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void printCandlesticks(int iterations);
        void gotoNextTimeframe();
        void printGraph();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime;
        std::string previousTime;

//        OrderBook orderBook{"20200317.csv"};
	OrderBook orderBook{"20200601.csv"};
    Wallet wallet;
    
    CryptoBook cryptoBook{"ETH-USD.csv"};

};
