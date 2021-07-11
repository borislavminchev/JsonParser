#include <stdexcept>

#include "JSON.h"
#include <iostream>

std::string JsonValue::name(JsonValueType t) const
{
	switch (t)
	{
	case 0: return "OBJECT";
	case 1: return "ARRAY";
	case 2: return "STRING";
	case 3: return "NUMBER";
	case 4: return "BOOLEAN";
	case 5: return "NULL_VALUE";

	default:throw std::runtime_error("Invalid JsonValueType!");
	}
}
std::string JsonValue::removeTailingZeros(std::string s) const
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

JsonValue::JsonValue() :type(NULL_VALUE) {}

JsonValue::JsonValue(const std::string& stringValue) : type(STRING)
{
	this->realValue.stringValue = stringValue;
}

JsonValue::JsonValue(const char* stringValue) : type(STRING)
{
	this->realValue.stringValue = stringValue;
}

JsonValue::JsonValue(bool boolValue) : type(BOOLEAN)
{
	this->realValue.boolValue = boolValue;
}

JsonValue::JsonValue(double numberValue) : type(NUMBER)
{
	this->realValue.numberValue = numberValue;
}

JsonValue::JsonValue(int integerValue) : type(NUMBER)
{
	this->realValue.numberValue = integerValue;
}

JsonValue::JsonValue(JsonArray arrayValue) : type(ARRAY)
{
	this->realValue.arrayValue = arrayValue;
}

JsonValue::JsonValue(JsonObject objectValue) : type(OBJECT)
{
	this->realValue.objectValue = objectValue;
}


JsonValue::JsonValue(const JsonValue& cpyValue)
{
	this->type = cpyValue.type;
	switch (cpyValue.type)
	{
	case OBJECT: {
		this->realValue.objectValue = cpyValue.realValue.objectValue;
		break;
	}
	case ARRAY: {
		this->realValue.arrayValue = cpyValue.realValue.arrayValue;
		break;
	}
	case STRING: {
		this->realValue.stringValue = cpyValue.realValue.stringValue;
		break;
	}
	case NUMBER: {
		this->realValue.numberValue = cpyValue.realValue.numberValue;
		break;
	}
	case BOOLEAN: {
		this->realValue.boolValue = cpyValue.realValue.boolValue;
		break;
	}
	case NULL_VALUE: {
		break;
	}
	}
}


JsonValue& JsonValue::operator=(const JsonValue& other)
{
	this->type = other.getType();
	switch (other.getType())
	{
	case OBJECT: {
		this->realValue.objectValue = other.realValue.objectValue;
		return *this;
	}
	case ARRAY: {
		this->realValue.arrayValue = other.realValue.arrayValue;
		return *this;
	}
	case STRING: {
		this->realValue.stringValue = other.realValue.stringValue;
		return *this;
	}
	case NUMBER: {
		this->realValue.numberValue = other.realValue.numberValue;
		return *this;
	}
	case BOOLEAN: {
		this->realValue.boolValue = other.realValue.boolValue;
		return *this;
	}
	case NULL_VALUE: {
		return *this;
	}
	}
}

std::string JsonValue::jsonArrToString(JsonArray arr, int deep) const
{
	std::string str;
	str += "[";
	std::string space;
	for (std::size_t i = 0; i < deep; i++)
	{
		space += "   ";
	}
	std::list<JsonValue*> a = arr.getArray();

	if (a.size() == 0)
	{
		str += "]";
		return str;
	}

	for (std::list<JsonValue*>::iterator i = a.begin(); i != a.end(); i++)
	{
		switch ((*i)->getType())
		{
		case OBJECT: {
			str += (deep == 0 ?
				"\n   " + space + jsonObjectToString((*i)->asJsonObject(), deep + 1) :
				jsonObjectToString((*i)->asJsonObject(), deep + 1));
			break;
		}
		case ARRAY:
			str += jsonArrToString((*i)->asJsonArray(), deep + 1);
			break;
		case STRING:
			str += (*i)->asString();
			break;
		case NUMBER:
			str += removeTailingZeros(std::to_string((*i)->asNumber()));
			break;
		case BOOLEAN:
			str += ((*i)->asBoolean() ? "true" : "false");
			break;
		case NULL_VALUE:
			str += "null";
			break;
		}
		str += ",";
	}

	if (deep == 0) {
		str.pop_back();
		str += "]";
	}
	else {
		str.pop_back();
		str += "]";
	}


	return str;
}
std::string JsonValue::jsonObjectToString(JsonObject object, int deep) const
{
	std::string str;
	std::string space;
	for (std::size_t i = 0; i < deep; i++)
	{
		space += "   ";
	}

	str += "{";
	std::map<std::string, JsonValue*> obj = object.getObject();

	for (std::map<std::string, JsonValue*>::iterator i = obj.begin(); i != obj.end(); ++i)
	{
		str += "\n   " + space + i->first + " : ";
		switch (i->second->getType())
		{
		case OBJECT:
			str += jsonObjectToString(i->second->asJsonObject(), deep + 1);
			break;
		case ARRAY:
			str += jsonArrToString(i->second->asJsonArray(), deep + 1);
			break;
		case STRING:
			str += i->second->asString();
			break;
		case NUMBER:
			str += removeTailingZeros(std::to_string(i->second->asNumber()));
			break;
		case BOOLEAN:
			str += (i->second->asBoolean() ? "true" : "false");
			break;
		case NULL_VALUE:
			str += "null";
			break;
		}
		str += ",";
	}

	str.pop_back();
	str += "\n" + space + "}";

	return str;
}


JsonValueType JsonValue::getType() const
{
	return this->type;
}

bool& JsonValue::asBoolean()
{
	if (this->type != BOOLEAN)
	{
		throw std::runtime_error("Can't get non-boolean value as bool");
	}
	return this->realValue.boolValue;
}

double& JsonValue::asNumber()
{
	if (this->type != NUMBER)
	{
		throw std::runtime_error("Can't get non-number value as number");
	}
	return this->realValue.numberValue;
}

std::string& JsonValue::asString()
{
	if (this->type != STRING)
	{
		throw std::runtime_error("Can't get non-string value as string");
	}

	return this->realValue.stringValue;
}

JsonObject& JsonValue::asJsonObject()
{
	if (this->type != OBJECT)
	{
		throw std::runtime_error("Can't get non-object value as json object");
	}

	return this->realValue.objectValue;
}

JsonArray& JsonValue::asJsonArray()
{
	if (this->type != ARRAY)
	{
		throw std::runtime_error("Can't get non-array value as json array");
	}

	return this->realValue.arrayValue;
}


std::string JsonValue::getValueString() const
{
	std::string s;
	switch (type)
	{
	case OBJECT: {
		s = jsonObjectToString(this->realValue.objectValue);
		break;
	}
	case ARRAY: {
		s = jsonArrToString(this->realValue.arrayValue);
		break;
	}
	case STRING:
		s = this->realValue.stringValue;
		break;
	case NUMBER:
		s = removeTailingZeros(std::to_string(this->realValue.numberValue));
		break;
	case BOOLEAN:
		s = (this->realValue.boolValue ? "true" : "false");
		break;
	case NULL_VALUE:
		s = "null";
		break;
	}
	return s;
}