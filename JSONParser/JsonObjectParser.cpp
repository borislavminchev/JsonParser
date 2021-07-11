#include "JsonObjectParser.h"
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <iostream>
#include <map>


JsonObjectParser::JsonObjectParser(JsonValueAllocator& a)
{
	this->values = &a;
}

std::string JsonObjectParser::extractString(const char* str)
{
	std::size_t i = 0;
	if (str[i] == '\"')
	{
		i++;
		while (str[i] != '\"' && i < std::strlen(str))
		{
			if (str[i] != '\\')
			{
				i++;
			}
			else if (str[i] == '\\')
			{
				if (i + 1 >= std::strlen(str) - 1)
				{
					return "";
				}

				if (str[i + 1] == '\"' || str[i + 1] == '\\' || str[i + 1] == '/' ||
					str[i + 1] == 'b' || str[i + 1] == 'f' || str[i + 1] == 'n' ||
					str[i + 1] == 'r' || str[i + 1] == 't')
				{
					i += 2;
				}
				else if (str[i + 1] == 'u')
				{
					if (i + 1 + 4 >= std::strlen(str) - 1)
					{
						return "";
					}

					for (size_t j = 0; j < 4; j++)
					{
						if (!(str[i + 2 + j] >= '0' || str[i + 2 + j] <= '9' ||
							str[i + 2 + j] == 'a' || str[i + 2 + j] == 'b' || str[i + 2 + j] == 'c'
							|| str[i + 2 + j] == 'd' || str[i + 2 + j] == 'e' || str[i + 2 + j] == 'f' ||
							str[i + 2 + j] == 'A' || str[i + 2 + j] == 'B' || str[i + 2 + j] == 'C'
							|| str[i + 2 + j] == 'D' || str[i + 2 + j] == 'E' || str[i + 2 + j] == 'F'))
						{
							return "";
						}
					}

					i += 5;
				}
				else
				{
					return "";
				}
			}
		}
		if (str[i] == '\"')
		{
			std::string s;
			for (std::size_t j = 0; j < i + 1; j++)
			{
				s += str[j];
			}
			return s;
		}
		else
		{
			return "";
		}
		
	}
	else
	{
		return "";
	}
}

std::string JsonObjectParser::removeTailingZeros(std::string s)
{
	std::size_t idx = s.size() - 1;
	for (int i = s.size() - 1; i >= 0; i--)
	{
		if (s[i] == '.')
		{
			idx--;
			break;
		}
		if (s[i] == '0')
		{
			idx--;
		}
		else
		{
			break;
		}
	}
	std::string res;
	for (size_t i = 0; i <= idx; i++)
	{
		res.push_back(s[i]);
	}
	return res;
}

std::string JsonObjectParser::extractNumber(const char* str)
{
	std::string s = str;
	std::stringstream stream(s);
	double d;
	stream >> d;
	
	return removeTailingZeros(std::to_string(d)).c_str();
}

std::string JsonObjectParser::extractBoolean(const char* str)
{
	std::string s;
	if (str[0] == 't' && strlen(str)>=4)
	{
		for (size_t i = 0; i < 4; i++)
		{
			s += str[i];
		}
	}
	else if (str[0] == 'f' && strlen(str) >= 5)
	{
		for (size_t i = 0; i < 5; i++)
		{
			s += str[i];
		}
	}
	
	if (s == "true" || s == "false")
	{
		return s;
	}

	return "";
	
}

std::string JsonObjectParser::extractNull(const char* str)
{
	std::string s;
	if (str[0] == 'n' && std::strlen(str) >= 4)
	{
		for (size_t i = 0; i < 4; i++)
		{
			s += str[i];
		}
	}

	return (s == "null" ? "null" : "");
}

std::string trimWhitespace(std::string str)
{
	std::string result;
	for (size_t i = 0; i < str.length(); i++)
	{

		if (!(str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f'))
		{
			result += str[i];
		}
	}
	return result;
}

JsonValue* JsonObjectParser::parseNumber(const char* number)
{

	std::stringstream stream(trimWhitespace(number));
	if (stream.str()[0] == '0') 
	{
		if ((stream.str()[1] >= '1' && stream.str()[1] <= '9') || stream.str()[1] == '.' || stream.str()[1] == 'e' || stream.str()[1] == 'E')
		{
			double d;
			stream >> d;
			return (values->newNumber(d));
		}
	}
	else
	{
		double d;
		stream >> d;
		if (d != 0)
		{
			return (values->newNumber(d));
		}
	}
	throw std::runtime_error("Coldn't parse number: " + std::string(stream.str()));
}

JsonValue* JsonObjectParser::parseString(const char* str)
{
	if (str[0] == '\"' && str[std::strlen(str) - 1] == '\"')
	{
		std::size_t i = 1;
		while(i < std::strlen(str) - 1)
		{
			if (str[i] != '\"' && str[i] != '\\')
			{
				i++;
			}
			else if (str[i] == '\\')
			{
				if (i + 1 >= std::strlen(str) - 1)
				{
					throw std::runtime_error("Coudnt parse string: " + std::string(str));
				}

				if (str[i + 1] == '\"' || str[i + 1] == '\\' || str[i + 1] == '/' ||
					str[i + 1] == 'b' || str[i + 1] == 'f' || str[i + 1] == 'n' ||
					str[i + 1] == 'r' || str[i + 1] == 't')
				{
					i+=2;
				}
				else if (str[i + 1] == 'u')
				{
					if (i + 1 + 4 >= std::strlen(str) - 1)
					{
						throw std::runtime_error("Coudnt parse string: " + std::string(str));
					}

					for (size_t j = 0; j < 4; j++)
					{
						if (!(str[i + 2 + j] >= '0' || str[i + 2 + j] <= '9' ||
							str[i + 2 + j] == 'a' || str[i + 2 + j] == 'b' || str[i + 2 + j] == 'c'
							|| str[i + 2 + j] == 'd' || str[i + 2 + j] == 'e' || str[i + 2 + j] == 'f' ||
							str[i + 2 + j] == 'A' || str[i + 2 + j] == 'B' || str[i + 2 + j] == 'C'
							|| str[i + 2 + j] == 'D' || str[i + 2 + j] == 'E' || str[i + 2 + j] == 'F'))
						{
							throw std::runtime_error("Coudnt parse string: " + std::string(str));
						}
					}

					i += 5;
				}
				else 
				{
					throw std::runtime_error("Coudnt parse string: " + std::string(str));
				}
			}
			else 
			{
				throw std::runtime_error("Coudnt parse string: " + std::string(str));
			}
		}

		return (values->newString(str));
	}
	throw std::runtime_error("Coudnt parse string: " + std::string(str));
}

JsonValue* JsonObjectParser::parseBoolean(const char* b)
{
	if (!std::strcmp(b, "true"))
	{
		return (values->newBoolean(true));
	}
	else if (!std::strcmp(b, "false"))
	{
		return (values->newBoolean(false));
	}
	throw std::runtime_error("Couldn't parse boolean: " + std::string(b));
}
JsonValue* JsonObjectParser::parsenNull(const char* nval)
{
	
	if (!std::strcmp(nval, "null"))
	{
		return (values->newNull());
	}

	throw std::runtime_error("Couldn't parse null value: " + std::string(nval));
}
JsonValue* JsonObjectParser::parseArray(const char* arr, std::size_t& start)
{
	std::size_t& i = start;
	if (arr[i] == '[')
	{
		i++;
		std::list<JsonValue*> list;
		bool firstElem = true;
		JsonValue* value = nullptr;
		while (arr[i] != ']' && i < strlen(arr))
		{
			while (arr[i] == ' ' || arr[i] == '\b' || arr[i] == '\f' || arr[i] == '\n' ||
				arr[i] == '\r' || arr[i] == '\t')
			{
				i++;
			}
			if (!firstElem)
			{
				if (arr[i] == ',')
				{
					i++;
					while (arr[i] == ' ' || arr[i] == '\b' || arr[i] == '\f' || arr[i] == '\n' ||
						arr[i] == '\r' || arr[i] == '\t')
					{
						i++;
					}
				}
				else
				{
					return nullptr;
				}
				
			}


			std::string valueStr;
			if (arr[i] == '-' || (arr[i] >= '0' && arr[i] <= '9'))
			{
				valueStr = extractNumber(arr + i);
				if (valueStr != "")
				{
					std::stringstream stream(valueStr.c_str());
					double d;
					stream >> d;
					value = values->newNumber(d);

					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (arr[i] == '\"')
			{
				valueStr = extractString(arr + i);
				if (valueStr != "")
				{
					value = values->newString(valueStr);
					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (arr[i] == 't' || arr[i] == 'f')
			{
				valueStr = extractBoolean(arr + i);
				if (valueStr != "")
				{
					value = (valueStr == "true" ? (values->newBoolean(true)) : (values->newBoolean(false)));
					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (arr[i] == 'n')
			{
				valueStr = extractNull(arr + i);
				if (valueStr != "")
				{
					value = values->newNull();
					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (arr[i] == '[')
			{
				value = parseArray(arr, i);
			}
			else if (arr[i] == '{')
			{
			 	value = parseObject(arr, i);
			}
			else
			{
				return nullptr;
			}

			while (arr[i] == ' ' || arr[i] == '\b' || arr[i] == '\f' || arr[i] == '\n' ||
				arr[i] == '\r' || arr[i] == '\t')
			{
				i++;
			}
			
			list.push_back(value);
			firstElem = false;
		}
		i++;
		return (values->newArray(list));
	}
	return nullptr;
}
JsonValue* JsonObjectParser::parseObject(const char* obj, std::size_t& start)
{
	std::size_t& i = start;
	if (obj[i] == '{')
	{
		i++;
		std::map<std::string, JsonValue*> map;
		bool firstElem = true;
		JsonValue* value = nullptr;
		while (obj[i] != '}' && i < strlen(obj))
		{
			while (obj[i] == ' ' || obj[i] == '\b' || obj[i] == '\f' || obj[i] == '\n' ||
				obj[i] == '\r' || obj[i] == '\t')
			{
				i++;
			}
			if (!firstElem)
			{
				if (obj[i] == ',')
				{
					i++;
					while (obj[i] == ' ' || obj[i] == '\b' || obj[i] == '\f' || obj[i] == '\n' ||
						obj[i] == '\r' || obj[i] == '\t')
					{
						i++;
					}
				}
				else
				{
					return nullptr;
				}

			}

			std::string keyStr = extractString(obj+i);
			i += keyStr.size();
			if (keyStr == "")
			{
				return nullptr;
			}

			while (obj[i] == ' ' || obj[i] == '\b' || obj[i] == '\f' || obj[i] == '\n' ||
				obj[i] == '\r' || obj[i] == '\t')
			{
				i++;
			}
			if (obj[i] != ':')
			{
				return nullptr;
			}
			else
			{
				i++;
			}
			while (obj[i] == ' ' || obj[i] == '\b' || obj[i] == '\f' || obj[i] == '\n' ||
				obj[i] == '\r' || obj[i] == '\t')
			{
				i++;
			}




			std::string valueStr;
			if (obj[i] == '-' || (obj[i] >= '0' && obj[i] <= '9'))
			{
				valueStr = extractNumber(obj + i);
				if (valueStr != "")
				{
					std::stringstream stream(valueStr.c_str());
					double d;
					stream >> d;
					value = values->newNumber(d);

					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (obj[i] == '\"')
			{
				valueStr = extractString(obj + i);
				if (valueStr != "")
				{
					value = values->newString(valueStr);
					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (obj[i] == 't' || obj[i] == 'f')
			{
				valueStr = extractBoolean(obj + i);
				if (valueStr != "")
				{
					value = (valueStr == "true" ? (values->newBoolean(true)) : (values->newBoolean(false)));
					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (obj[i] == 'n')
			{
				valueStr = extractNull(obj + i);
				if (valueStr != "")
				{
					value = values->newNull();
					i += valueStr.length();
				}
				else
				{
					return nullptr;
				}
			}
			else if (obj[i] == '[')
			{
				value = parseArray(obj, i);
			}
			else if (obj[i] == '{')
			{
				value = parseObject(obj, i);
				
				int m = 0;
			}
			else
			{
				return nullptr;
			}
			while (obj[i] == ' ' || obj[i] == '\b' || obj[i] == '\f' || obj[i] == '\n' ||
				obj[i] == '\r' || obj[i] == '\t')
			{
				i++;
			}
			map[keyStr] = value;
			firstElem = false;
		}
		
		i++;
		JsonValue* v = values->newObject(map);
		return v;
		
	}


	return nullptr;
}


JsonValue* JsonObjectParser::parse(const char* value)
{
	JsonValue* v = nullptr;
	if (value[0] == '-' || (value[0] >= '0' && value[0] <= '9'))
	{
		v = parseNumber(value);
	}
	else if (value[0] == '\"')
	{
		v = parseString(value);
	}
	else if (!std::strcmp(value, "true") || !std::strcmp(value, "false"))
	{
		v = parseBoolean(value);
	}
	else if (!std::strcmp(value, "null"))
	{
		v = parsenNull(value);
	}
	else if (value[0] == '[')
	{
		std::size_t start = 0;
		v = parseArray(value, start);
	}
	else if (value[0] == '{')
	{
		std::size_t start = 0;
		v = parseObject(value, start);
	}

	return v;
}

void JsonObjectParser::clearParser()
{
	this->values->clearAllocator();
}