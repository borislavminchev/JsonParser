#include "JsonManager.h"
#include <iostream>

std::vector<std::string> JsonManager::splitArguments(std::string command) const
{
	std::vector<std::string> result;
	std::string temp;
	for (std::size_t i = 0; i < command.length(); i++)
	{
		if (command[i] == '.')
		{
			result.push_back(temp);
			temp.clear();
		}
		else if (command[i] == '[')
		{
			result.push_back(temp);
			temp.clear();
			do
			{
				temp += command[i];
				i++;
			} while (command[i] != ']' && i < command.length());
			if (command[i] == ']')
				temp += command[i];
		}
		else
		{
			temp += command[i];
		}
	}
	if (!temp.empty())
		result.push_back(temp);
	return result;
}

void JsonManager::searchInDepth(std::string searchKey, JsonValue* srcJson, std::vector<JsonValue*>& resultSet)
{
	JsonValue* v = nullptr;
	if (srcJson->getType() == OBJECT)
	{
		try
		{
			v = &srcJson->asJsonObject().getElementWithProperty(searchKey);
			resultSet.push_back(v);
		}
		catch (...) {}

		for (std::map<std::string, JsonValue*>::iterator i = srcJson->asJsonObject().getObject().begin();
			i != srcJson->asJsonObject().getObject().end();
			i++)
		{
			searchInDepth(searchKey, i->second, resultSet);
		}

	}
	else if (srcJson->getType() == ARRAY)
	{
		for (std::size_t i = 0; i < srcJson->asJsonArray().size(); i++)
		{
			searchInDepth(searchKey, &srcJson->asJsonArray().getElementById(i), resultSet);
		}
	}
	else
	{
		return;
	}
}

JsonManager::JsonManager(JsonValueAllocator& a)
{
	this->allocator = &a;
}

void JsonManager::print(const JsonValue value) const
{
	std::cout << value.getValueString();
}

JsonValue* JsonManager::find(std::string path, JsonValue& srcJson)
{
	JsonValue* ptr = &srcJson;
	std::vector<std::string> cmd = splitArguments(path);
	std::size_t start;
	if (cmd[0] == "$")
	{
		
		start = 1;
	}
	else
	{
		
		start = 0;
	}
	
	std::size_t sz = cmd.size();
	for (size_t i = start; i < sz; i++)
	{
		if (cmd[i][0] == '[')
		{
			std::size_t idx = 0;
			for (size_t j = 1; j < cmd[i].length()-1; j++)
			{
				if (cmd[i][j] < '0' || cmd[i][j] > '9')
				{
					throw std::runtime_error("Array index error (Not number): " + cmd[i]);
				}
				idx += 10 * idx + (cmd[i][j] - '0');
			}
			try
			{
				ptr = &ptr->asJsonArray().getElementById(idx);
			}
			catch (...)
			{
				return nullptr;
			}
			
		}
		else
		{
			try
			{
				ptr = &ptr->asJsonObject().getElementWithProperty(std::string("\"") + cmd[i] + "\"");
			}
			catch (...)
			{
				return nullptr;
			}
			
		}
	}

	return ptr;
}


JsonValue* JsonManager::search(std::string searchKey, JsonValue* srcJson)
{
	std::vector<JsonValue*> res;
	searchInDepth(std::string("\"") + searchKey + "\"", srcJson, res);

	std::list<JsonValue*> list;
	for (size_t i = 0; i < res.size(); i++)
	{
		list.push_back(res[i]);
	}

	return (this->allocator->newArray(list));
}

void JsonManager::edit(std::string path, JsonValue& srcJson, JsonValue& newValue)
{
	JsonValue* old = this->find(path, srcJson);
	if (old)
	{
		(*old) = newValue;
	}
	else
	{
		throw std::runtime_error("Couldn't find value with path: " + path);
	}
	
}

void JsonManager::create(std::string path, JsonValue& srcJson, JsonValue& val)
{
	if (this->find(path, srcJson))
	{
		throw std::runtime_error("Element with this path already exists: " + path);
	}
	std::vector<std::string> elems = this->splitArguments(path);
	
	JsonValue* v = nullptr;
	std::size_t start = 0;
	if (elems[0] == "$")
	{
		v = &srcJson;
		start = 1;
	}

	std::string p = "$";
	while(start < elems.size())
	{
		JsonValue* check = this->find(p + (elems[start][0] == '[' ? elems[start] : ("." + elems[start])),
										srcJson);
		if (!check)
		{
			check = this->find(p, srcJson);
			if ((check->getType() == OBJECT && elems[start][0] == '[') || 
				(check->getType() == ARRAY && elems[start][0] != '['))
			{
				throw std::runtime_error("Unable to insert element at give path: " + path);
				start--;
			}
			break;
		}

		p += (elems[start][0] == '[' ? elems[start] : ("." + elems[start]));
		start++;
	}

	v = this->find(p, srcJson);
	for (size_t i = start; i < elems.size(); i++)
	{
		JsonValue* temp = this->allocator->newNull();
		if (elems[i][0] == '[')
		{
			std::size_t idx = 0;
			for (size_t j = 1; j < elems[i].length() - 1; j++)
			{
				if (elems[i][j] < '0' || elems[i][j] > '9')
				{
					throw std::runtime_error("Array index error (Not number): " + elems[i]);
				}
				idx += 10 * idx + (elems[i][j] - '0');
			}

			if (v->asJsonArray().getArray().size() == idx)
			{
				if (i == elems.size() - 1)
				{
					v->asJsonArray().add(val);
				}
				else
				{
					*temp = (elems[i + 1][0] == '[' ? JsonValue(JsonArray()) : JsonValue(JsonObject()));
					v->asJsonArray().add(*temp);
					v = temp;
				}
				
			}
			else
			{
				throw std::runtime_error("Invalid array index to insert new object");
			}
		}
		else
		{
			if (i == elems.size() - 1)
			{
				v->asJsonObject().insert("\"" + elems[i] + "\"", val);
			}
			else
			{
				*temp = (elems[i + 1][0] == '[' ? JsonValue(JsonArray()) : JsonValue(JsonObject()));
				v->asJsonObject().insert("\"" + elems[i] + "\"", *temp);
				v = temp;
			}
			
		}
	}
}



void JsonManager::remove(std::string path, JsonValue& srcJson)
{
	JsonValue* toBeDeleted = &srcJson;
	JsonValue* ptr = &srcJson;
	std::vector<std::string> cmd = splitArguments(path);
	std::size_t start = (cmd[0] == "$" ? 1 : 0);

	for (size_t i = start; i < cmd.size() - 1; i++)
	{
		if (cmd[i][0] == '[')
		{
			std::size_t idx = 0;
			for (size_t j = 1; j < cmd[i].length() - 1; j++)
			{
				if (cmd[i][j] < '0' || cmd[i][j] > '9')
				{
					throw std::runtime_error("Array index error (Not number): " + cmd[i]);
				}
				idx += 10 * idx + (cmd[i][j] - '0');
			}
			ptr = &ptr->asJsonArray().getElementById(idx);
		}
		else
		{
			ptr = &ptr->asJsonObject().getElementWithProperty(std::string("\"") + cmd[i] + "\"");
		}
	}

	if (cmd[cmd.size() - 1][0] == '[')
	{
		std::size_t idx = 0;
		for (size_t j = 1; j < cmd[cmd.size() - 1].length() - 1; j++)
		{
			if (cmd[cmd.size() - 1][j] < '0' || cmd[cmd.size() - 1][j] > '9')
			{
				throw std::runtime_error("Array index error (Not number): " + cmd[cmd.size() - 1]);
			}
			idx += 10 * idx + (cmd[cmd.size() - 1][j] - '0');
		}
		ptr->asJsonArray().removeElementById(idx);
	}
	else 
	{
		ptr->asJsonObject().removeElementByProperty(std::string("\"") + cmd[cmd.size() - 1] + "\"");
	}
}

void JsonManager::move(std::string path1, std::string path2, JsonValue& value)
{
	JsonValue* v1 = find(path1, value);
	JsonValue* v2 = find(path2, value);

	if (!v2)
	{
		this->create(path2, value, *v1);
	}
	else
	{
		this->edit(path2, value, *v1);
	}
	
	this->remove(path1, value);
}

void JsonManager::clearManager()
{
	this->allocator->clearAllocator();
}