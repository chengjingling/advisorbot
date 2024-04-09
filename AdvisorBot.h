#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class AdvisorBot
{
public:
	AdvisorBot();
	void init();

private:
	/* get user option */
	std::string getUserOption();

	/* run specific function based on user option */
	void processUserOption(std::string userOption);

	/* list all available commands */
	void help();

	/* output help for the specified command */
	void helpCmd(std::string userOption);

	/* list available products */
	void prod();

	/* find minimum bid or ask for product in current time step */
	void min(std::string userOption);

	/* find maximum bid or ask for product in current time step */
	void max(std::string userOption);

	/* compute average ask or bid for the sent product over the sent number of time steps */
	void avg(std::string userOption);

	/* predict max or min ask or bid for the sent product for the next time step */
	void predict(std::string userOption);

	/* state current time in dataset, i.e. which timeframe are we looking at */
	void time();

	/* move to next time step */
	void step();

	/* enter ask/bid */
	void enter(std::string userOption);

	/* list entered asks/bids */
	void retrieve();

	/* delete entered ask/bid */
	void del(std::string userOption);

	/* list all currencies in wallet */
	void printWallet();

	/* process entered asks/bids */
	void processSales();

	/* bad input */
	void badInput();

	std::string currentTime;
	OrderBook orderBook{ "20200601.csv" };
	Wallet wallet;
};