#pragma once

#include "JSON.h"
#include "JsonValueAllocator.h"
#include <string>
#include <vector>

/// <summary>
/// Manager class used for executing users commands in computer's memory
/// </summary>
class JsonManager
{
	JsonValueAllocator* allocator;//allocator pointer for allocating new values if it is needed

	/// <summary>
	/// Splits json path's arguments
	/// </summary>
	/// <param name="path">Json path that will be splited </param>
	/// <returns>Collection of json path's arguments</returns>
	std::vector<std::string> splitArguments(std::string path) const;

	/// <summary>
	/// Recursive helper-function for search() method
	/// </summary>
	/// <param name="searchKey">Key used for search</param>
	/// <param name="srcJson">Source Value in which will be searched for given key</param>
	/// <param name="resultSet">Collection of found values</param>
	void searchInDepth(std::string searchKey, JsonValue* srcJson, std::vector<JsonValue*>& resultSet);
public:
	/// <summary>
	/// Associates allocator to given allocator object.
	/// </summary>
	/// <param name="a">Allocator object</param>
	JsonManager(JsonValueAllocator& a);
	
	/// <summary>
	/// Finds value that is located in given path
	/// </summary>
	/// <param name="path">Json path of value</param>
	/// <param name="srcJson">Source Json object (where to search)</param>
	/// <returns>Pointer to value that is located in given path or nullptr if value is not found</returns>
	JsonValue* find(std::string path, JsonValue& srcJson);

	/// <summary>
	/// Prints given json value
	/// </summary>
	/// <param name="value"></param>
	void print(const JsonValue value) const;

	/// <summary>
	/// Searches for all values with given key
	/// </summary>
	/// <param name="searchKey">Key used for search</param>
	/// <param name="srcJson">Source Value in which will be searched for the given key</param>
	/// <returns>Json array with found values</returns>
	JsonValue* search(std::string searchKey, JsonValue* srcJson);

	/// <summary>
	/// Edits value at given json path in given object
	/// </summary>
	/// <param name="path">Json path where old value is lovated</param>
	/// <param name="srcJson">Source Value in wich will be changed a value</param>
	/// <param name="newValue">New value for overriding the old one</param>
	void edit(std::string path, JsonValue& srcJson, JsonValue& newValue);

	/// <summary>
	/// Adds new value at given json path of given object 
	/// </summary>
	/// <param name="path">Path where new value has to be located at the end of the method</param>
	/// <param name="srcJson">Source Value in wich will be added new value</param>
	/// <param name="val">New Value</param>
	void create(std::string path, JsonValue& srcJson, JsonValue& val);

	/// <summary>
	/// Removes valuelocated at given path of given object
	/// </summary>
	/// <param name="path">Path to the value that will be deleted</param>
	/// <param name="srcJson">Source Value in wich will be removed value</param>
	void remove(std::string path, JsonValue& srcJson);

	/// <summary>
	/// Moves json value located on first path and adds/overrides it on second path.
	/// Everyting happens in given object.
	/// </summary>
	/// <param name="path1">Json path of value that is going to be moved</param>
	/// <param name="path2">Location where value has to be after execution of the fuction</param>
	/// <param name="value">Source Value in wich will be moved given value</param>
	void move(std::string path1, std::string path2, JsonValue& value);

	/// <summary>
	/// Clears allocators data
	/// </summary>
	void clearManager();
};