#include "Wallet.h"
#include "CSVReader.h"
#include <iostream>

Wallet::Wallet()
{

}

void Wallet::insertCurrency(std::string type, double amount)
{
	double balance;

	if (amount < 0)
	{
		throw std::exception{};
	}

	if (currencies.count(type) == 0) // not there yet
	{
		balance = 0;
	}

	else // is there
	{
		balance = currencies[type];
	}

	balance += amount;
	currencies[type] = balance;
}

bool Wallet::removeCurrency(std::string type, double amount)
{
	if (amount < 0)
	{
		return false;
	}

	if (currencies.count(type) == 0) // not there yet
	{
		std::cout << "No currency for " << type << std::endl;
		return false;
	}

	else // is there - do we have enough
	{
		if (containsCurrency(type, amount)) // we have enough
		{
			std::cout << "Removing " << type << ": " << amount << std::endl;
			currencies[type] -= amount;
			return true;
		}

		else // they have it but not enough
		{
			return false;
		}
	}
}

bool Wallet::containsCurrency(std::string type, double amount)
{
	if (currencies.count(type) == 0)
	{
		return false;
	}

	else
	{
		return currencies[type] >= amount;
	}
}

bool Wallet::canFulfillOrder(OrderBookEntry order)
{
	std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');

	// ask
	if (order.orderType == OrderBookType::ask)
	{
		std::string currency = currs[0]; // i have currs[0]
		double amount = order.amount; // i need to have the amount in order to sell
		
		return containsCurrency(currency, amount);
	}

	// bid
	if (order.orderType == OrderBookType::bid)
	{
		std::string currency = currs[1]; // i have currs[1]
		double amount = order.price * order.amount; // i need to have the price * amount i want to buy
		
		return containsCurrency(currency, amount);
	}

	return false;
}

void Wallet::processSale(OrderBookEntry& sale)
{
	std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');

	// ask
	if (sale.orderType == OrderBookType::asksale)
	{
		std::string outgoingCurrency = currs[0];
		double outgoingAmount = sale.amount;

		std::string incomingCurrency = currs[1];
		double incomingAmount = sale.price * sale.amount;

		currencies[incomingCurrency] += incomingAmount;
		currencies[outgoingCurrency] -= outgoingAmount;
	}

	// bid
	if (sale.orderType == OrderBookType::bidsale)
	{
		std::string incomingCurrency = currs[0];
		double incomingAmount = sale.amount;

		std::string outgoingCurrency = currs[1];
		double outgoingAmount = sale.price * sale.amount;

		currencies[incomingCurrency] += incomingAmount;
		currencies[outgoingCurrency] -= outgoingAmount;
	}
}

std::string Wallet::toString()
{
	std::string s = "advisorbot> Your wallet contains:";

	for (std::pair<std::string, double> pair : currencies)
	{
		std::string currency = pair.first;
		double amount = pair.second;
		s += "\n" + currency + ": " + std::to_string(amount);
	}

	return s;
}