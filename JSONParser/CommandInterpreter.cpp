#include "CommandInterpreter.h"
#include <fstream>
#include <iostream>

std::string CommandInterpreter::toLowerCase(std::string s)
{
	std::string res;
	for (size_t i = 0; i < s.length(); i++)
	{
		res += std::tolower(s[i]);
	}

	return res;
}

void CommandInterpreter::clearData()
{
	this->manager.clearManager();
	this->parser.clearParser();
	this->allocator->clearAllocator();
}

std::vector<std::string> CommandInterpreter::splitArguments(std::string s)
{
	std::vector<std::string> res;
	std::string temp;
	bool seenSpace = false;

	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ' && !seenSpace)
		{
			res.push_back(temp);
			seenSpace = true;
			temp.clear();
		}
		else if (s[i] != ' ')
		{
			temp += s[i];
			seenSpace = false;
		}
	}
	if (!temp.empty())
	{
		res.push_back(temp);
	}

	return res;
}

bool CommandInterpreter::isNumber(std::string num)
{
	for (size_t i = 0; i < num.length(); i++)
	{
		if (!std::isdigit(num[i]))
		{
			return false;
		}
	}

	return true;
}

void CommandInterpreter::writeToFile(std::string path, JsonValue& value) const
{
	std::ofstream file(path);
	std::string s = value.getValueString();
	file << s;
	file.close();
}

CommandInterpreter::CommandInterpreter(JsonValueAllocator& a) :manager(a), parser(a)
{
	this->allocator = &a;

	this->value = nullptr;
	this->opened = false;
	this->changed = false;
}
CommandInterpreter::~CommandInterpreter()
{
	this->clearData();
}

bool CommandInterpreter::executeOpen(std::string filePath)
{
	if (opened)
	{
		this->executeClose();
	}

	std::ifstream in(filePath);
	if (!in)
	{
		std::cout << ("File path not found: " + filePath) << std::endl;
		return false;
	}

	std::string full = "";
	std::string buffer;

	while (std::getline(in, buffer))
	{
		if (!buffer.empty())
		{
			full += buffer + "\n";
		}
	}
	JsonValue* v = nullptr;
	v = this->parser.parse(full.c_str());

	if(!v)
	{	
		in.close();
		std::cout << "File object error! Couldn't parse" << std::endl;
		return false;
	}
	in.close();
	
	this->value = v;
	this->file = filePath;
	this->opened = true;
	this->changed = false;
	this->searchSet = nullptr;
	return true;
}

bool CommandInterpreter::executeClose()
{
	if (!opened)
	{
		std::cout << "No file to be closed!" << std::endl;
		return false;
	}

	if (changed)
	{
		std::cout << "(!)FILE IS NOT SAVED. DO YOU WANT TO SAVE THE CHANGES?\n(yes/no/cancel): " << std::endl;
		std::string ans;
		std::cin >> ans;
		if (ans == "yes")
		{
			this->executeSave();
		}
		else if (ans == "no")
		{
			std::cout << "(!)FILE WAS NOT SAVED!" << std::endl;
		}
		else if (ans == "cancel")
		{
			std::cout << "Colsing file was canceled." << std::endl;
			return false;
		}
		else
		{
			std::cout << "Invalid command. File failed to close." << std::endl;
			return false;
		}
	}


	if (this->file == "")
	{
		std::cout << "No file name was given! (use saveas)" << std::endl;
		return false;

	}

	this->opened = false;
	this->file = "";
	this->clearData();
	this->value = nullptr;
	this->searchSet = nullptr;
	std::cout << "File closed successfully!: " + file << std::endl;
	return true;
}

bool CommandInterpreter::executeNew()
{
	if (opened)
	{
		this->executeClose();
	}

	this->clearData();
	this->changed = false;
	this->file = "";
	this->value = this->allocator->newNull();
	this->opened = true;
	this->searchSet = nullptr;
	return true;
}

bool CommandInterpreter::executeSave()
{
	if (!this->changed)
	{
		std::cout << "File alredy was saved!" << std::endl;
		return false;
	}

	if (!opened)
	{
		std::cout << "No file to save!" << std::endl;
		return false;
	}

	if (this->file == "")
	{
		std::cout << "Failed to save file! (No file name was given) " << std::endl;
		return false;

	}

	this->writeToFile(this->file, *this->value);
	this->changed = false;

	return true;
}

bool CommandInterpreter::executeSaveAs(std::string path)
{
	if (!opened)
	{
		std::cout << "No file to save" << std::endl;
		return false;
	}


	this->writeToFile(path, *this->value);
	this->changed = false;
	this->file = path;

	return true;
}

bool CommandInterpreter::printSearchSet()
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}
	if (!this->searchSet)
	{
		std::cout << "No search set found!" << std::endl;
		return false;
	}

	std::cout << this->searchSet->getValueString() << std::endl;
	return true;
}

bool CommandInterpreter::saveFullSearchSet(std::string filePath)
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}
	if (!this->searchSet)
	{
		std::cout << "No search set found!" << std::endl;
		return false;
	}

	this->writeToFile(filePath, *this->searchSet);

	return true;
}

bool CommandInterpreter::saveElementFromSearchSet(std::size_t idx, std::string filePath)
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}
	if (!this->searchSet)
	{
		std::cout << "No search set found!" << std::endl;
		return false;
	}
	try
	{
		JsonValue* v = &this->searchSet->asJsonArray().getElementById(idx);
		this->writeToFile(filePath, *v);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	return true;
}

bool CommandInterpreter::executePrint()
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}

	std::cout << this->value->getValueString() << std::endl;
	return true;
}

bool CommandInterpreter::executeSearch(std::string searchKey)
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}
	JsonValue* res;
	try 
	{
		res = this->manager.search(searchKey, this->value);
		this->searchSet = res;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	return true;
}

bool CommandInterpreter::executeEdit(std::string path, std::string newValue)
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}
	
	JsonValue* newV = this->parser.parse(newValue.c_str());
	if (!newV)
	{
		std::cout << "File object error! Couldn't parse." << std::endl;
		return false;
	}
	try
	{
		this->manager.edit(path, *this->value, *newV);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	
	this->changed = true;
	return true;
}

bool CommandInterpreter::executeCreate(std::string path, std::string val)
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}

	JsonValue* newV = this->parser.parse(val.c_str());
	if (!newV)
	{
		std::cout << "File object error! Couldn't parse." << std::endl;
		return false;
	}

	try
	{
		this->manager.create(path, *this->value, *newV);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	this->changed = true;
	return true;
}

bool CommandInterpreter::executeRemove(std::string path)
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}
	try
	{
		this->manager.remove(path, *this->value);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	this->changed = true;
	return true;
}

bool CommandInterpreter::executeMove(std::string path1, std::string path2)
{
	if (!this->value)
	{
		std::cout << "No value loaded! Please open a file." << std::endl;
		return false;
	}
	try
	{
		this->manager.move(path1, path2, *this->value);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	this->changed = true;
	return true;
}
std::string CommandInterpreter::startJsonValueConversation() const
{
	std::cout << "Enter your json value here: \nNOTE: to end typing write <!END>:" << std::endl;
	std::string res;
	while (true)
	{
		//std::cin.ignore(std::cin.rdbuf()->in_avail());
		char c = std::cin.get();
		if (c == '<')
		{
			//std::cin.ignore(std::cin.rdbuf()->in_avail());
			std::string check = "<";
			for (size_t i = 0; i < 5; i++)
			{
				check += std::cin.get();
			}
			if (check == "<!END>")
			{
				return res;
			}
			else
			{
				res += check;
			}
		}
		else
		{
			res += c;
		}
	}

	return res;
}
void CommandInterpreter::executeCommand(std::string command)
{
	std::vector<std::string> arguments = splitArguments(command);

	std::string cmd = toLowerCase(arguments[0]);
	if (cmd == "open" && arguments.size() == 2)
	{
		executeOpen(arguments[1]);
	}
	else if (cmd == "close" && arguments.size() == 1)
	{
		executeClose();
	}
	else if (cmd == "new" && arguments.size() == 1)
	{
		executeNew();
	}
	else if (cmd == "save" && arguments.size() == 1)
	{
		executeSave();
	}
	else if (cmd == "saveas" && arguments.size() == 2)
	{
		executeSaveAs(arguments[1]);
	}
	else if (cmd == "searchset" )
	{
		if (toLowerCase(arguments[1]) == "print" && arguments.size() == 2)
		{
			printSearchSet();
		}
		else if (toLowerCase(arguments[1]) == "save" && arguments.size() == 3)
		{
			saveFullSearchSet(arguments[2]);
		}
		else if (toLowerCase(arguments[1]) == "saveat" && arguments.size() == 4)
		{
			if (isNumber(arguments[2]))
			{
				saveElementFromSearchSet(std::stoul(arguments[2]), arguments[3]);
			}
			else
			{
				std::cout << "Invalid command: " + command << std::endl;
			}
		}
		else 
		{
			std::cout << "Invalid command: " + command << std::endl;
		}
	}
	else if (cmd == "print")
	{
		executePrint();
	}
	else if (cmd == "search" && arguments.size() == 2)
	{
		executeSearch(arguments[1]);
	}
	else if (cmd == "edit" && arguments.size() == 2)
	{
		std::string myValue = startJsonValueConversation();
		executeEdit(arguments[1], myValue);
	}
	else if (cmd == "create" && arguments.size() == 2)
	{
		std::string myValue = startJsonValueConversation();
		executeCreate(arguments[1], myValue);
	}
	else if (cmd == "remove" && arguments.size() == 2)
	{
		executeRemove(arguments[1]);
	}
	else if (cmd == "move" && arguments.size() == 3)
	{
		executeMove(arguments[1], arguments[2]);
	}
	else
	{
		std::cout << "Invalid command: " + command << std::endl;
	}

}
