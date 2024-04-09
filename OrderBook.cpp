#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>

/* construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
	orders = CSVReader::readCSV(filename);
}

/* returns vector of all known products in the dataset */
std::vector<std::string> OrderBook::getKnownProducts()
{
	std::vector<std::string> products;
	std::map<std::string, bool> prodMap;

	for (OrderBookEntry& e : orders)
	{
		prodMap[e.product] = true;
	}

	// flatten the map to a vector of strings
	for (auto const& e : prodMap)
	{
		products.push_back(e.first);
	}

	return products;
}

/* returns vector of Orders according to the sent filters */
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
	std::string product,
	std::string timestamp)
{
	std::vector<OrderBookEntry> orders_sub;

	for (OrderBookEntry& e : orders)
	{
		if (e.orderType == type &&
			e.product == product &&
			e.timestamp == timestamp)
		{
			orders_sub.push_back(e);
		}
	}

	return orders_sub;
}

/* returns the earliest time in the orderbook */
std::string OrderBook::getEarliestTime()
{
	return orders[0].timestamp;
}

/* returns the next time after the sent time in the orderbook. if there is no next timestamp, wraps around to the start. */
std::string OrderBook::getNextTime(std::string timestamp)
{
	std::string next_timestamp = "";

	for (OrderBookEntry& e : orders)
	{
		if (e.timestamp > timestamp)
		{
			next_timestamp = e.timestamp;
			break;
		}
	}

	if (next_timestamp == "")
	{
		next_timestamp = orders[0].timestamp;
	}

	return next_timestamp;
}

/* returns the previous time before the sent time in the orderbook. if there is no previous timestamp, wraps around to the end. */
std::string OrderBook::getPreviousTime(std::string timestamp)
{
	std::string previous_timestamp = "";

	for (unsigned int i = orders.size() - 1; i >= 0; --i)
	{
		if (i == 0)
		{
			previous_timestamp = orders[orders.size() - 1].timestamp;
			return previous_timestamp;
		}

		else if (orders[i].timestamp < timestamp)
		{
			previous_timestamp = orders[i].timestamp;
			return previous_timestamp;
		}
	}
}

/* returns maximum price of orders */
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
	double max = orders[0].price;

	for (OrderBookEntry& e : orders)
	{
		if (e.price > max) max = e.price;
	}

	return max;
}

/* returns minimum price of orders */
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
	double min = orders[0].price;

	for (OrderBookEntry& e : orders)
	{
		if (e.price < min) min = e.price;
	}

	return min;
}

/* returns average price of orders */
double OrderBook::getMean(std::vector<OrderBookEntry>& orders)
{
	double sum = 0;

	for (OrderBookEntry& e : orders)
	{
		sum += e.price;
	}

	double mean = sum / orders.size();

	return mean;
}

/* inserts new order */
void OrderBook::insertOrder(OrderBookEntry& order)
{
	orders.push_back(order);
	std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

/* matches asks to bids */
std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
	// get asks
	std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
		product,
		timestamp);

	// get bids
	std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
		product,
		timestamp);

	// create empty vector "sales"
	std::vector<OrderBookEntry> sales;

	// sort asks from lowest to highest
	std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);

	// sort bids from highest to lowest
	std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

	for (OrderBookEntry& ask : asks)
	{
		for (OrderBookEntry& bid : bids)
		{
			if (bid.price >= ask.price) // we have a match
			{
				// create sale (match ask price)
				OrderBookEntry sale{ ask.price, 0, timestamp, product, OrderBookType::asksale };

				if (bid.username == "simuser")
				{
					sale.username = "simuser";
					sale.orderType = OrderBookType::bidsale;
				}

				if (ask.username == "simuser")
				{
					sale.username = "simuser";
					sale.orderType = OrderBookType::asksale;
				}

				if (bid.amount == ask.amount)
				{
					sale.amount = ask.amount;
					sales.push_back(sale);

					// delete orders once they have been fulfilled
					orders.erase(std::remove(orders.begin(), orders.end(), bid), orders.end());
					orders.erase(std::remove(orders.begin(), orders.end(), ask), orders.end());

					// make sure the bid is not processed again
					bid.amount = 0;

					// can do no more with this ask, go onto the next ask
					break;
				}

				if (bid.amount > ask.amount)
				{
					sale.amount = ask.amount;
					sales.push_back(sale);

					// delete ask because it has been fulfilled
					orders.erase(std::remove(orders.begin(), orders.end(), ask), orders.end());

					// delete bid (and create bid with new amount)
					orders.erase(std::remove(orders.begin(), orders.end(), bid), orders.end());

					// we adjust the bid in place so it can be used to process the next ask
					bid.amount = bid.amount - ask.amount;

					// create bid with new amount
					OrderBookEntry obe{ bid.price, bid.amount, bid.timestamp, bid.product, bid.orderType, bid.username };
					orders.push_back(obe);
					
					// can do no more with this ask, go onto the next ask
					break;
				}

				if (bid.amount < ask.amount && bid.amount > 0) // partially sell it
				{
					sale.amount = bid.amount;
					sales.push_back(sale);

					// delete bid because it has been fulfilled
					orders.erase(std::remove(orders.begin(), orders.end(), bid), orders.end());

					// delete ask (and create ask with new amount)
					orders.erase(std::remove(orders.begin(), orders.end(), ask), orders.end());

					// update the ask and allow further bids to process the remaining amount
					ask.amount = ask.amount - bid.amount;

					// create ask with new amount
					OrderBookEntry obe{ ask.price, ask.amount, ask.timestamp, ask.product, ask.orderType, ask.username };
					orders.push_back(obe);

					// make sure the bid is not processed again
					bid.amount = 0;

					// some ask remains so go to the next bid
					continue;
				}
			}
		}
	}

	return sales;
}