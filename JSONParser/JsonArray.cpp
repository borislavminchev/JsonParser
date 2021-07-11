#include "JSON.h"
#include <stdexcept>


JsonArray::JsonArray() :arrSize(0) {}
JsonArray::JsonArray(std::list<JsonValue*> a)
{
	this->arrSize = a.size();
	this->jsonArray = a;
}
JsonArray& JsonArray::operator=(const JsonArray& other)
{
	this->arrSize = other.arrSize;
	this->jsonArray = other.jsonArray;
	return *this;
}

JsonArray::JsonArray(const JsonArray& cpy)
{
	this->arrSize = cpy.arrSize;
	std::list<JsonValue*> a = cpy.jsonArray;
	for (std::list<JsonValue*>::iterator i = a.begin(); i != a.end(); ++i)
	{
		this->jsonArray.push_back(*i);
	}
}

std::size_t JsonArray::size() const
{
	return this->arrSize;
}

JsonValue& JsonArray::getElementById(std::size_t id)
{
	JsonValue* ptr = nullptr;
	if(id < jsonArray.size())
	{
		std::list<JsonValue*>::iterator it = std::next(jsonArray.begin(), id);
		return **it;
	}
	throw std::runtime_error("Invalid index: " + std::to_string(id));
}

void JsonArray::removeElementById(std::size_t id)
{
	JsonValue* ptr = &this->getElementById(id);
	this->jsonArray.remove(ptr);
	this->arrSize--;
}

void JsonArray::add(JsonValue& elem)
{
	this->jsonArray.push_back(&elem);
	this->arrSize++;
}

std::list<JsonValue*>& JsonArray::getArray()
{
	;
	return this->jsonArray;
}
