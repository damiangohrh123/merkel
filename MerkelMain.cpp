#include "MerkelMain.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "CryptoBook.h"
#include "CryptoEntry.h"

MerkelMain::MerkelMain() {}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    // 1 Print help
    std::cout << "1: Print help " << std::endl;
    // 2 Print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 Make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 Make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 Print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 Print candlesticks  
    std::cout << "6: Print candlesticks " << std::endl;
    // 7 Print graph of ETH-USD data
    std::cout << "7: Print graph of ETH-USD data " << std::endl;
    // 8 Continue
    std::cout << "8: Continue " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe()
{
	std::cout << "Going to next time frame. " << std::endl;
	for (std::string p : orderBook.getKnownProducts()){
		std::cout << "matching " << p << std::endl;
		std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
		std::cout << "Sales: " << sales.size() << std::endl;
		for (OrderBookEntry& sale : sales){
			std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
			if (sale.username == "simuser"){
				// update the wallet
				wallet.processSale(sale);
			}
		}
	}
	currentTime = orderBook.getNextTime(currentTime);
}
 
int MerkelMain::getUserOption()
{
	int userOption = 0;
	std::string line;
	std::cout << "Type in 1-8" << std::endl;
	std::getline(std::cin, line);
	try{
		userOption = std::stoi(line);
	}catch(const std::exception& e)
	{
		// 
	}
	std::cout << "You chose: " << userOption << std::endl;
	return userOption;
}

void MerkelMain::processUserOption(int userOption)
{	
	if (userOption == 0) std::cout << "Invalid choice. Choose 1-8" << std::endl;
	if (userOption == 1) printHelp();
	if (userOption == 2) printMarketStats();
	if (userOption == 3) enterAsk();
	if (userOption == 4) enterBid();
	if (userOption == 5) printWallet();
	if (userOption == 6) printCandlesticks(5);
    if (userOption == 7) printGraph();
    if (userOption == 8) gotoNextTimeframe();
}

void MerkelMain::printGraph() {
    std::cout << "Enter the type of data and how many timeframes. Max 18 timeframes. E.g. high,15" << std::endl;
	std::string input;
	std::getline(std::cin, input);

    // Tokenise user's input into dataType and iterations.
	std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if (tokens.size() != 2) {
        std::cout << "MerkelMain::printGraph Bad input! " << input << std::endl;
        return;
    } else {
        try {
            std::string dataType = tokens[0];
            int iterations = std::min(std::stoi(tokens[1]), 18); // Limit iterations to 18.

            // Make code more robust.
            if (dataType == "open" || dataType == "high" || dataType == "low" || dataType == "close") {
                cryptoBook.printGraph(dataType, iterations);
            } else {
                std::cout << "MerkelMain::printGraph Bad input!" << std::endl;
                return;
            }

            // Provide user options to print next data or exit to menu.
            while (true) {
                std::cout << "Enter 'next' to show the next set of graphs." << std::endl
                          << "Enter 'exit' to go back to the menu." << std::endl;

                std::string userInput;
                std::getline(std::cin, userInput);

            if (userInput == "next") {
                // Print the next set of graphs.
                cryptoBook.printGraph(dataType, iterations);
            } else if (userInput == "exit") {
                // Exit the loop and go back to the menu.
                break;
            } else {
                std::cout << "Invalid input. Please enter 'next' or 'exit'." << std::endl;
            }
        }

        } catch(const std::exception& e) {
            std::cout << "MerkelMain::printGraph Bad input" << std::endl;
            return;
        }
    }
}

void MerkelMain::printCandlesticks(int iterations) {
	std::cout << "Enter the product and type of entry. eg  ETH/BTC,ask" << std::endl;
	std::string input;
	std::getline(std::cin, input);

    // Tokenise user's input into product and OrderBookType
	std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
	if (tokens.size() != 2){
		std::cout << "MerkelMain::printCandleSticks Bad input!" << input << std::endl;
        return;
	} else {
        try {
            std::string product = tokens[0];
            std::string type = tokens[1];

            // Check Order book type
            OrderBookType orderType;
            if (type == "ask") orderType = OrderBookType::ask;
            else if (type == "bid") orderType = OrderBookType::bid;
            else {
                std::cout << "Invalid entry type. Use 'ask' or 'bid'" << std::endl;
                return;
            }
            
            // Store orderbook entries for 5 timeframes to pass to computeCandleStick.
            std::vector<Candlestick> candlesticks;

            // Seperate time variable so as not to change currentTime.
            std::string thisTime = currentTime; 
                                    
            // Obtain orderbook entries.
            for (size_t i = 0; i < iterations; i++) {
                    // Get entries based on user's input in current time frame.
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(orderType, product, thisTime);

                    // Get previous time frame's entries.
                    std::string previousTime = orderBook.getPreviousTime(thisTime);
                    std::vector<OrderBookEntry> previousEntries = orderBook.getOrders(orderType, product, previousTime);

                    // Compute candlestick data for specified entries.
                    Candlestick candlestick = Candlestick::computeCandlestick(entries, previousEntries);

                    // Append the candlesticks for this iteration to the vector
                    candlesticks.push_back(candlestick);

                    // Update the thisTime for the next iteration.
                    thisTime = orderBook.getNextTime(thisTime);
                }   

                // Table headers
                std::cout << std::setw(25) << "Time"
                          << std::setw(20) << "Open"
                          << std::setw(20) << "High"
                          << std::setw(20) << "Low"
                          << std::setw(20) << "Close" << std::endl;
                                
                // Print candlesticks data
                for (const Candlestick& candlestick : candlesticks) {
                    std::cout << std::setw(25) << candlestick.time 
                              << std::setw(20) << candlestick.open
                              << std::setw(20) << candlestick.high
                              << std::setw(20) << candlestick.low
                              << std::setw(20) << candlestick.close << std::endl;
                }

                // Print out graphical text-based plot of candlesticks
                Candlestick::computeTextPlot(candlesticks);
                
		}catch (const std::exception& e){
			std::cout << " MerkelMain::printCandleSticks Bad input " << std::endl;
		}   
	}
}
