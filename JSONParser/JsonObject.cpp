#include <stdexcept>
#include <iostream>
#include "JSON.h"

JsonObject::JsonObject() :objSize(0) {}
JsonObject::JsonObject(std::map<std::string, JsonValue*> m)
{
	this->objSize = m.size();
	this->object = m;
}
JsonObject::JsonObject(const JsonObject& other)
{
	this->objSize = other.objSize;
	this->object = other.object;
}
JsonObject& JsonObject::operator=(const JsonObject& other)
{
	this->object = other.object;
	this->objSize = other.objSize;
	return *this;
}

std::size_t JsonObject::size() const
{
	return this->objSize;
}

JsonValue& JsonObject::getElementWithProperty(std::string prop)
{
	for (std::map<std::string, JsonValue*>::iterator i = this->object.begin(); i != this->object.end(); ++i)
	{
		if (i->first == prop)
		{
			return *this->object[prop];
		}
	}
	throw std::runtime_error("Element with property not found:" + prop);
}

void JsonObject::removeElementByProperty(std::string prop)
{
	for (std::map<std::string, JsonValue*>::iterator i = this->object.begin(); i != this->object.end(); i++)
	{
		if (i->first == prop)
		{
			this->object.erase(i);
			this->objSize--;
			return;
		}
	}
	throw std::runtime_error("Element with property not found:" + prop);
}

void JsonObject::insert(std::string prop, JsonValue& value)
{
	this->object[prop] = &value;
	this->objSize++;
}

std::map<std::string, JsonValue*>& JsonObject::getObject()
{
	return this->object;
}