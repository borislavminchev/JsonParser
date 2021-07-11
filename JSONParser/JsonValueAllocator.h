#pragma once
#include "JSON.h"
#include <deque>
#include <map>
#include <string>

/// <summary>
/// Allocates new Json values
/// </summary>
class JsonValueAllocator
{
	//place where new values are stored
	std::list<JsonValue> values;
public:
	/// <summary>
	/// Allocates new number(JsonValue)
	/// </summary>
	/// <param name="d">Number to initialize json value</param>
	/// <returns>Pointer to allocated value</returns>
	JsonValue* newNumber(double d);

	/// <summary>
	/// Allocates new boolean(JsonValue)
	/// </summary>
	/// <param name="b">Boolean to initialize json value</param>
	/// <returns>Pointer to allocated value</returns>
	JsonValue* newBoolean(bool b);

	/// <summary>
	/// Allocates new string(JsonValue)
	/// </summary>
	/// <param name="str">String to initialize json value</param>
	/// <returns>Pointer to allocated value</returns>
	JsonValue* newString(std::string str);

	/// <summary>
	/// Allocates new null(JsonValue)
	/// </summary>
	/// <returns>Pointer to allocated value</returns>
	JsonValue* newNull();

	/// <summary>
	/// Allocates new json array(JsonValue)
	/// </summary>
	/// <param name="arr">List where are stored values of the array that will be created</param>
	/// <returns>Pointer to allocated value</returns>
	JsonValue* newArray(std::list<JsonValue*>& arr);

	/// <summary>
	///  Allocates new json object(JsonValue)
	/// </summary>
	/// <param name="map"></param>
	/// <returns>Map where are stored keys and values of the object that will be created</returns>
	JsonValue* newObject(std::map<std::string, JsonValue*>& map);

	/// <summary>
	/// Get all	allocated values
	/// </summary>
	/// <returns>Collection of all allocated values</returns>
	std::list<JsonValue>& getList();

	/// <summary>
	/// Clears allocators data
	/// </summary>
	void clearAllocator();
};