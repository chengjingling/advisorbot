#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "AdvisorBot.h"
#include "CSVReader.h"

AdvisorBot::AdvisorBot()
{

}

void AdvisorBot::init()
{
	std::string input;
	currentTime = orderBook.getEarliestTime();
	
	wallet.insertCurrency("BTC", 10);

	while (true)
	{
		input = getUserOption();
		processUserOption(input);
	}
}

/* get user option */
std::string AdvisorBot::getUserOption()
{
	std::string userOption;
	std::cout << "advisorbot> Please enter a command, or help for a list of commands\nuser> ";
	std::getline(std::cin, userOption);

	return userOption;
}

/* run specific function based on user option */
void AdvisorBot::processUserOption(std::string userOption)
{
	if (userOption == "help")
	{
		help();
	}
	else if (userOption.find("help") == 0)
	{
		helpCmd(userOption);
	}
	else if (userOption == "prod")
	{
		prod();
	}
	else if (userOption.find("min") == 0)
	{
		min(userOption);
	}
	else if (userOption.find("max") == 0)
	{
		max(userOption);
	}
	else if (userOption.find("avg") == 0)
	{
		avg(userOption);
	}
	else if (userOption.find("predict") == 0)
	{
		predict(userOption);
	}
	else if (userOption == "time")
	{
		time();
	}
	else if (userOption == "step")
	{
		step();
	}
	else if (userOption.find("enter") == 0)
	{
		enter(userOption);
	}
	else if (userOption == "retrieve")
	{
		retrieve();
	}
	else if (userOption.find("delete") == 0)
	{
		del(userOption);
	}
	else if (userOption == "wallet")
	{
		printWallet();
	}
	else if (userOption == "sales")
	{
		processSales();
	}
	else
	{
		badInput();
	}
}

/* list all available commands */
void AdvisorBot::help()
{
	std::cout << "advisorbot> The available commands are help, help <cmd>, prod, min, max, avg, predict, time, step, enter, retrieve, delete, wallet and sales" << std::endl;
}

/* output help for the specified command */
void AdvisorBot::helpCmd(std::string userOption)
{
	if (userOption == "help prod")
	{
		std::cout << "advisorbot> prod -> list available products" << std::endl;
	}

	if (userOption == "help min")
	{
		std::cout << "advisorbot> min ETH/BTC ask -> minimum ETH/BTC ask in the current time step" << std::endl;
	}

	if (userOption == "help max")
	{
		std::cout << "advisorbot> max ETH/BTC ask -> maximum ETH/BTC ask in the current time step" << std::endl;
	}

	if (userOption == "help avg")
	{
		std::cout << "advisorbot> avg ETH/BTC ask 10 -> average ETH/BTC ask over the last 10 time steps" << std::endl;
	}

	if (userOption == "help predict")
	{
		std::cout << "advisorbot> predict min ETH/BTC ask -> minimum ETH/BTC ask in the next time step" << std::endl;
	}

	if (userOption == "help time")
	{
		std::cout << "advisorbot> time -> state current time in dataset, i.e. which timeframe we are looking at" << std::endl;
	}

	if (userOption == "help step")
	{
		std::cout << "advisorbot> step -> move to next time step" << std::endl;
	}

	if (userOption == "help enter")
	{
		std::cout << "advisorbot> enter ask ETH/BTC 0.02 10 -> enter ETH/BTC ask with price 0.02 and amount 10" << std::endl;
	}

	if (userOption == "help retrieve")
	{
		std::cout << "advisorbot> retrieve -> list entered asks/bids" << std::endl;
	}

	if (userOption == "help delete")
	{
		std::cout << "advisorbot> delete ask ETH/BTC 0.02 10 -> delete entered ask/bid" << std::endl;
	}

	if (userOption == "help wallet")
	{
		std::cout << "advisorbot> wallet -> list currencies in wallet" << std::endl;
	}

	if (userOption == "help sales")
	{
		std::cout << "advisorbot> sales -> process entered asks/bids" << std::endl;
	}
}

/* list available products */
void AdvisorBot::prod()
{
	std::string string = "advisorbot> ";

	for (std::string const& p : orderBook.getKnownProducts())
	{
		if (string == "advisorbot> ")
		{
			string = string + p;
		}

		else
		{
			string = string + ", " + p;
		}
	}

	std::cout << string << std::endl;
}

/* find minimum bid or ask for product in current time step */
void AdvisorBot::min(std::string userOption)
{
	std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');
	std::vector<std::string> products = orderBook.getKnownProducts();

	if (tokens.size() == 3 && std::count(products.begin(), products.end(), tokens[1]) && (tokens[2] == "ask" || tokens[2] == "bid"))
	{
		OrderBookType type;

		if (tokens[2] == "ask")
		{
			type = OrderBookType::ask;
		}

		else
		{
			type = OrderBookType::bid;
		}

		std::vector<OrderBookEntry> entries = orderBook.getOrders(type, tokens[1], currentTime);

		if (entries.empty() == false)
		{
			std::cout << "advisorbot> The min " << tokens[2] << " for " << tokens[1] << " is " << orderBook.getLowPrice(entries) << std::endl;
		}
	}

	else
	{
		std::cout << "advisorbot> Bad input: " << userOption << std::endl;
	}
}

/* find maximum bid or ask for product in current time step */
void AdvisorBot::max(std::string userOption)
{
	std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');
	std::vector<std::string> products = orderBook.getKnownProducts();

	if (tokens.size() == 3 && std::count(products.begin(), products.end(), tokens[1]) && (tokens[2] == "ask" || tokens[2] == "bid"))
	{
		OrderBookType type;

		if (tokens[2] == "ask")
		{
			type = OrderBookType::ask;
		}

		else
		{
			type = OrderBookType::bid;
		}

		std::vector<OrderBookEntry> entries = orderBook.getOrders(type, tokens[1], currentTime);

		if (entries.empty() == false)
		{
			std::cout << "advisorbot> The max " << tokens[2] << " for " << tokens[1] << " is " << orderBook.getHighPrice(entries) << std::endl;
		}
	}

	else
	{
		std::cout << "advisorbot> Bad input: " << userOption << std::endl;
	}
}

/* compute average ask or bid for the sent product over the sent number of time steps */
void AdvisorBot::avg(std::string userOption)
{
	std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');
	std::vector<std::string> products = orderBook.getKnownProducts();

	if (tokens.size() == 4 && std::count(products.begin(), products.end(), tokens[1]) && (tokens[2] == "ask" || tokens[2] == "bid") && (tokens[3].find_first_not_of("0123456789") == std::string::npos) == true)
	{
		OrderBookType type;

		if (tokens[2] == "ask")
		{
			type = OrderBookType::ask;
		}

		else
		{
			type = OrderBookType::bid;
		}

		std::string tempTimeStep = orderBook.getNextTime(currentTime);
		std::vector<OrderBookEntry> allEntries;

		for (int i = 0; i < std::stoi(tokens[3]); i++)
		{
			std::string prevTimeStep = orderBook.getPreviousTime(tempTimeStep);
			std::vector<OrderBookEntry> currentEntries = orderBook.getOrders(type, tokens[1], prevTimeStep);

			if (currentEntries.empty() == false)
			{
				allEntries.insert(allEntries.end(), currentEntries.begin(), currentEntries.end());
			}

			tempTimeStep = prevTimeStep;
		}

		std::cout << "advisorbot> The average " << tokens[1] << " " << tokens[2] << " price over the last " << tokens[3] << " time steps was " << orderBook.getMean(allEntries) << std::endl;
	}

	else
	{
		std::cout << "advisorbot> Bad input: " << userOption << std::endl;
	}
}

/* predict max or min ask or bid for the sent product for the next time step */
void AdvisorBot::predict(std::string userOption)
{
	std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');
	std::vector<std::string> products = orderBook.getKnownProducts();

	if (tokens.size() == 4 && (tokens[1] == "min" || tokens[1] == "max") && std::count(products.begin(), products.end(), tokens[2]) && (tokens[3] == "ask" || tokens[3] == "bid"))
	{
		OrderBookType type;

		if (tokens[3] == "ask")
		{
			type = OrderBookType::ask;
		}

		else
		{
			type = OrderBookType::bid;
		}

		std::string tempTimeStep = orderBook.getNextTime(currentTime);
		double sum = 0;
		int count = 0;

		while (count < 10)
		{
			std::string prevTimeStep = orderBook.getPreviousTime(tempTimeStep);
			std::vector<OrderBookEntry> currentEntries = orderBook.getOrders(type, tokens[2], prevTimeStep);

			if (currentEntries.empty() == false)
			{
				count += 1;

				if (tokens[1] == "min")
				{
					std::cout << count << ". Min at " << prevTimeStep << ": " << orderBook.getLowPrice(currentEntries) << std::endl;
					sum += orderBook.getLowPrice(currentEntries);
				}
				
				else
				{
					std::cout << count << ". Max at " << prevTimeStep << ": " << orderBook.getHighPrice(currentEntries) << std::endl;
					sum += orderBook.getHighPrice(currentEntries);
				}
			}

			else
			{
				std::cout << "-SKIPPED- " << prevTimeStep << " has no entries for " << tokens[2] << std::endl;
			}

			tempTimeStep = prevTimeStep;
		}

		double mean = sum / count;

		std::cout << "advisorbot> The average " << tokens[1] << " " << tokens[2] << " " << tokens[3] << " price over the last " << count << " time steps was " << mean << std::endl;
	}

	else
	{
		std::cout << "advisorbot> Bad input: " << userOption << std::endl;
	}
}

/* state current time in dataset, i.e. which timeframe are we looking at */
void AdvisorBot::time()
{
	std::cout << "advisorbot> " << currentTime << std::endl;
}

/* move to next time step */
void AdvisorBot::step()
{
	currentTime = orderBook.getNextTime(currentTime);
	std::cout << "advisorbot> Now at " << currentTime << std::endl;
}

/* enter ask/bid */
void AdvisorBot::enter(std::string userOption)
{
	std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');
	std::vector<std::string> products = orderBook.getKnownProducts();

	if (tokens.size() == 5 && (tokens[1] == "ask" || tokens[1] == "bid") && std::count(products.begin(), products.end(), tokens[2]) && (tokens[3].find_first_not_of("0123456789.") == std::string::npos) == true && (tokens[4].find_first_not_of("0123456789.") == std::string::npos) == true)
	{
		try
		{
			OrderBookEntry obe = CSVReader::stringsToOBE(
				tokens[3],
				tokens[4],
				currentTime,
				tokens[2],
				OrderBookEntry::stringToOrderBookType(tokens[1])
			);

			obe.username = "simuser";

			if (wallet.canFulfillOrder(obe))
			{
				std::cout << "advisorbot> Wallet looks good" << std::endl;
				orderBook.insertOrder(obe);
			}

			else
			{
				std::cout << "advisorbot> Wallet has insufficient funds" << std::endl;
			}
		}

		catch (const std::exception& e)
		{
			std::cout << "advisorbot> Bad input: " << userOption << std::endl;
		}
	}

	else
	{
		std::cout << "advisorbot> Bad input: " << userOption << std::endl;
	}
}

/* list entered asks/bids */
void AdvisorBot::retrieve()
{
	for (OrderBookEntry& order : orderBook.orders)
	{
		if (order.timestamp == currentTime && order.username == "simuser")
		{
			if (order.orderType == OrderBookType::bid)
			{
				std::cout << "bid " << order.product << " " << order.price << " " << order.amount << std::endl;
			}

			else if (order.orderType == OrderBookType::ask)
			{
				std::cout << "ask " << order.product << " " << order.price << " " << order.amount << std::endl;
			}
		}
	}
}

/* delete entered ask/bid */
void AdvisorBot::del(std::string userOption)
{
	std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');
	std::vector<std::string> products = orderBook.getKnownProducts();

	if (tokens.size() == 5 
		&& (tokens[1] == "ask" || tokens[1] == "bid") 
		&& std::count(products.begin(), products.end(), tokens[2]) 
		&& (tokens[3].find_first_not_of("0123456789.") == std::string::npos) == true 
		&& (tokens[4].find_first_not_of("0123456789.") == std::string::npos) == true)
	{
		for (OrderBookEntry& order : orderBook.orders)
		{
			if (order.price == std::stod(tokens[3]) 
				&& order.amount == std::stod(tokens[4]) 
				&& order.timestamp == currentTime 
				&& order.product == tokens[2] 
				&& order.orderType == OrderBookEntry::stringToOrderBookType(tokens[1]) 
				&& order.username == "simuser")
			{
				orderBook.orders.erase(std::remove(orderBook.orders.begin(), orderBook.orders.end(), order), orderBook.orders.end());
				std::cout << "advisorbot> Delete successful" << std::endl;
			}
		}
	}

	else
	{
		std::cout << "advisorbot> Bad input: " << userOption << std::endl;
	}
}

/* list all currencies in wallet */
void AdvisorBot::printWallet()
{
	std::cout << wallet.toString() << std::endl;
}

/* process entered asks/bids */
void AdvisorBot::processSales()
{
	for (std::string p : orderBook.getKnownProducts())
	{
		std::cout << "advisorbot> Matching " << p << std::endl;
		std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
		std::cout << "Sales: " << sales.size() << std::endl;

		for (OrderBookEntry& sale : sales)
		{
			std::cout << "Sale price: " << sale.price << ", Sale amount: " << sale.amount << std::endl;

			if (sale.username == "simuser")
			{
				wallet.processSale(sale);
			}
		}
	}
}

/* bad input */
void AdvisorBot::badInput()
{
	std::cout << "advisorbot> Invalid choice. Please try again" << std::endl;
}