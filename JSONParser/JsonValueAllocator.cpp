#include "JsonValueAllocator.h"


JsonValue* JsonValueAllocator::newNumber(double d)
{
	std::size_t s = this->values.size();
	this->values.push_back(JsonValue(d));
	return &values.back();
}

JsonValue* JsonValueAllocator::newBoolean(bool b)
{
	this->values.push_back(JsonValue(b));
	return &values.back();
}

JsonValue* JsonValueAllocator::newString(std::string str)
{
	this->values.push_back(JsonValue(str));
	return &values.back();
}

JsonValue* JsonValueAllocator::newNull()
{
	this->values.push_back(JsonValue());
	return &values.back();
}

JsonValue* JsonValueAllocator::newArray(std::list<JsonValue*>& arr)
{
	JsonArray a;
	for (std::list<JsonValue*>::iterator i = arr.begin(); i != arr.end(); i++)
	{
		a.add(**i);
	}
	this->values.push_back(JsonValue(a));

	return &values.back();
}

JsonValue* JsonValueAllocator::newObject(std::map<std::string, JsonValue*>& map)
{
	JsonObject obj;
	for (std::map<std::string, JsonValue*>::iterator i = map.begin(); i != map.end(); i++)
	{
		obj.insert(i->first, *i->second);
	}
	this->values.push_back(JsonValue(obj));

	return &values.back();
	return nullptr;
}


std::list<JsonValue>& JsonValueAllocator::getList()
{
	return this->values;
}

void JsonValueAllocator::clearAllocator()
{
	this->values.clear();
}