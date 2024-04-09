#include <string>
#include "OrderBookEntry.h"

OrderBookEntry::OrderBookEntry(double _price,
	double _amount,
	std::string _timestamp,
	std::string _product,
	OrderBookType _orderType,
	std::string _username)
	: price(_price),
	amount(_amount),
	timestamp(_timestamp),
	product(_product),
	orderType(_orderType),
	username(_username)
{

}

OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
	if (s == "ask")
	{
		return OrderBookType::ask;
	}

	if (s == "bid")
	{
		return OrderBookType::bid;
	}

	return OrderBookType::unknown;
}

bool OrderBookEntry::operator==(OrderBookEntry e)
{
	if (price == e.price && amount == e.amount && timestamp == e.timestamp && product == e.product && orderType == e.orderType && username == e.username)
	{
		return true;
	}

	return false;
}