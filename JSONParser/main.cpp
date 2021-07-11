#include <iostream>
#include <stdexcept>
#include "JsonObjectParser.h"
#include "JSON.h"
#include "JsonValueAllocator.h"
#include "JsonManager.h"
#include "CommandInterpreter.h"


/// <summary>
/// Prints all available commands
/// </summary>
void printHelp()
{
	std::cout << "List of available commands:\n" << std::endl;
	std::cout << "open <file_path>" << std::endl;
	std::cout << "close" << std::endl;
	std::cout << "new" << std::endl;
	std::cout << "save" << std::endl;
	std::cout << "saveas <file_path>" << std::endl;
	std::cout << "searchset print" << std::endl;
	std::cout << "searchset save <file_path>" << std::endl;
	std::cout << "searchset saveat <index> <file_path>" << std::endl;
	std::cout << "print" << std::endl;
	std::cout << "search <search_key>" << std::endl;
	std::cout << "edit <json_path>" << std::endl;
	std::cout << "create <json_path>" << std::endl;
	std::cout << "remove <json_path>" << std::endl;
	std::cout << "move <json_path> <json_path>" << std::endl;
	std::cout << std::endl;
}

/// <summary>
/// Starts conversation wth user and for executing user commands uses interpreter argument.
/// </summary>
/// <param name="interpreter">Used for executing user commands</param>
void startConversation(CommandInterpreter& interpreter)
{
	std::cout << "For available commands type \"help\"\n";
	std::string command;
	while (true)
	{
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cout << ">>";
		std::getline(std::cin, command);
		if (command == "exit")
		{
			std::cout << "Thank you! Goodbye! :)";
			break;
		}
		else if (command == "help")
		{
			printHelp();
		}
		else
		{
			interpreter.executeCommand(command);
		}
	}
}

/// <summary>
/// Main function
/// </summary>
/// <returns></returns>
int main()
{
	try 
	{
		JsonValueAllocator allocator;
		CommandInterpreter cmd(allocator);
		startConversation(cmd);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	
    return 0;
}
