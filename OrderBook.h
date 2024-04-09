#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
public:
	/* construct, reading a csv data file */
	OrderBook(std::string filename);

	/* returns vector of all known products in the dataset */
	std::vector<std::string> getKnownProducts();

	/* returns vector of Orders according to the sent filters */
	std::vector<OrderBookEntry> getOrders(
		OrderBookType type,
		std::string product,
		std::string timestamp);

	/* returns the earliest time in the orderbook */
	std::string getEarliestTime();

	/* returns the next time after the sent time in the orderbook. if there is no next timestamp, wraps around to the start. */
	std::string getNextTime(std::string timestamp);

	/* returns the previous time before the sent time in the orderbook. if there is no previous timestamp, wraps around to the end. */
	std::string getPreviousTime(std::string timestamp);

	/* returns maximum price of orders */
	static double getHighPrice(std::vector<OrderBookEntry>& orders);

	/* returns minimum price of orders */
	static double getLowPrice(std::vector<OrderBookEntry>& orders);

	/* returns average price of orders */
	static double getMean(std::vector<OrderBookEntry>& orders);

	/* inserts new order */
	void insertOrder(OrderBookEntry& order);

	/* matches asks to bids */
	std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

	std::vector<OrderBookEntry> orders;
};