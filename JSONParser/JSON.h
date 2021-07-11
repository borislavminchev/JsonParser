#pragma once

#include <list>
#include <map>
#include <string>

class JsonValue;

/// <summary>
/// Representation of json object
/// </summary>
class JsonObject
{
	std::map<std::string, JsonValue*> object;
	std::size_t objSize;
public:
	/// <summary>
	/// Default Constructor. Creates empty map and sets size to zero
	/// </summary>
	JsonObject();

	/// <summary>
	/// Contructor that copies information stored in given map
	/// </summary>
	/// <param name="">Source map</param>
	JsonObject(std::map<std::string, JsonValue*>);

	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="other">Json object to be copied</param>
	JsonObject(const JsonObject& other);

	/// <summary>
	/// Copy assignment operator
	/// </summary>
	/// <param name="other">Json object to be copied</param>
	/// <returns>New value of the object</returns>
	JsonObject& operator=(const JsonObject& other);

	/// <summary>
	/// Gets size of map
	/// </summary>
	/// <returns>Size of the map</returns>
	std::size_t size() const;

	/// <summary>
	/// Gets element(with given property) from the map 
	/// </summary>
	/// <param name="prop">Property to search in map</param>
	/// <returns>value with given key or throws error if ther isn't value with that poperty</returns>
	JsonValue& getElementWithProperty(std::string prop);

	/// <summary>
	/// Removes element with given property from the map
	/// </summary>
	/// <param name="prop">Property of the elementt that's going to be removed</param>
	void removeElementByProperty(std::string prop);

	/// <summary>
	/// Inserts new key-value pair in map
	/// </summary>
	/// <param name="prop">Key of the new pair</param>
	/// <param name="value">Value of the new pair</param>
	void insert(std::string prop, JsonValue& value);

	/// <summary>
	/// Gets object's equivalent map(class property)  
	/// </summary>
	/// <returns>object's equivalent map(class property)</returns>
	std::map<std::string, JsonValue*>& getObject();
};

class JsonArray
{
	std::list<JsonValue*> jsonArray;
	std::size_t arrSize;
public:
	/// <summary>
	/// Default Constructor. Creates empty list and sets size to zero
	/// </summary>
	JsonArray();

	/// <summary>
	/// Contructor that copies information stored in given list
	/// </summary>
	/// <param name="list">Source list</param>
	JsonArray(std::list<JsonValue*> list);
	
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="other">Json object to be copied</param>
	JsonArray(const JsonArray& other);

	/// <summary>
	/// Copy assignment operator
	/// </summary>
	/// <param name="other">Json array to be copied</param>
	/// <returns>New value of the json array</returns>
	JsonArray& operator=(const JsonArray& other);

	/// <summary>
	/// Gets size of list
	/// </summary>
	/// <returns>size of the list</returns>
	std::size_t size() const;

	/// <summary>
	/// Gets element at given position
	/// </summary>
	/// <param name="id">Position</param>
	/// <returns>element at given position or throws error if index is invalid</returns>
	JsonValue& getElementById(std::size_t id);

	/// <summary>
	/// Removes element at given position
	/// </summary>
	/// <param name="id">Index of element to be deleted</param>
	void removeElementById(std::size_t id);

	/// <summary>
	/// Adds new element
	/// </summary>
	/// <param name="elem">Element to be added</param>
	void add(JsonValue& elem);

	/// <summary>
	/// Gets array's equivalent list(class property)  
	/// </summary>
	/// <returns>array's equivalent list(class property)</returns>
	std::list<JsonValue*>& getArray();
};

/// <summary>
/// Type of Json value
/// </summary>
enum JsonValueType
{
	OBJECT,
	ARRAY,
	STRING,
	NUMBER,
	BOOLEAN,
	NULL_VALUE
};

/// <summary>
/// Union that sores every possible value of JsonValue object(Only one real value is possible)
/// </summary>
union Evaluated
{
	bool boolValue;
	double numberValue;
	std::string stringValue;
	JsonArray arrayValue;
	JsonObject objectValue;

	Evaluated() :objectValue(std::map<std::string, JsonValue*>()) {}
	~Evaluated() {}
};


class JsonValue {
	JsonValueType type;
	Evaluated realValue;

	/// <summary>
	/// Get String representation of JsonValue's type
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::string name(JsonValueType) const;

	/// <summary>
	/// Gets string representation of Json Array(recursive function)
	/// </summary>
	/// <param name="arr">Initial array</param>
	/// <param name="deep">In what level of recursion is current execution</param>
	/// <returns>String representation of Json Array</returns>
	std::string jsonArrToString(JsonArray arr, int deep = 0) const;

	/// <summary>
	/// Gets string representation of Json Object(recursive function)
	/// </summary>
	/// <param name="obj">Initial object</param>
	/// <param name="deep">In what level of recursion is current execution</param>
	/// <returns>String representation of Json Object</returns>
	std::string jsonObjectToString(JsonObject obj, int deep = 0) const;

	/// <summary>
	/// Removes tailing zeros after decimal point of given number(as string)
	/// Needed because std::to_string() method works like that. 
	/// </summary>
	/// <param name="s">Number(as string) where will be removed unnecessary zeros after decimal point</param>
	/// <returns>Number(as string) without unnecessary zeros</returns>
	std::string removeTailingZeros(std::string s) const;

public:

	/// <summary>
	/// Default Constructor. Creates empty list and sets size to zero
	/// </summary>
	JsonValue();

	/// <summary>
	/// Constructor that creates json string
	/// </summary>
	/// <param name="stringValue">C-String value of object</param>
	JsonValue(const char* stringValue);

	/// <summary>
	/// Constructor that creates json string
	/// </summary>
	/// <param name="stringValue">String value of object</param>
	JsonValue(const std::string& stringValue);

	/// <summary>
	/// Constructor that creates json boolen
	/// </summary>
	/// <param name="stringValue">Bool value of object</param>
	JsonValue(bool boolValue);

	/// <summary>
	/// Constructor that creates json number
	/// </summary>
	/// <param name="stringValue">Number(double) value of object</param>
	JsonValue(double numberValue);

	/// <summary>
	/// Constructor that creates json number
	/// </summary>
	/// <param name="stringValue">Number(int) value of object</param>
	JsonValue(int integerValue);

	/// <summary>
	/// Constructor that creates json array(as JsonValue)
	/// </summary>
	/// <param name="stringValue">JsonArray value of object</param>
	JsonValue(JsonArray arrayValue);

	/// <summary>
	/// Constructor that creates json array(as JsonObject)
	/// </summary>
	/// <param name="stringValue">JsonObject value of object</param>
	JsonValue(JsonObject objectValue);

	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="other">Json object to be copied</param>
	JsonValue(const JsonValue& cpyValue);

	/// <summary>
	/// Copy assignment operator
	/// </summary>
	/// <param name="other">Json value to be copied</param>
	/// <returns>New value of the json value</returns>
	JsonValue& operator=(const JsonValue& other);

	/// <summary>
	/// Gets bool representation of json value
	/// </summary>
	/// <returns>bool representation of json value</returns>
	bool& asBoolean();

	/// <summary>
	/// Gets number representation of json value
	/// </summary>
	/// <returns>number representation of json value</returns>
	double& asNumber();

	/// <summary>
	/// Gets string representation of json value
	/// </summary>
	/// <returns>String representation of json value</returns>
	std::string& asString();

	/// <summary>
	/// Gets JsonObject representation of json value
	/// </summary>
	/// <returns>JsonObject representation of json value</returns>
	JsonObject& asJsonObject();

	/// <summary>
	/// Gets JsonArray representation of json value
	/// </summary>
	/// <returns>JsonArray representation of json value</returns>
	JsonArray& asJsonArray();

	/// <summary>
	/// Gets type of json value
	/// </summary>
	/// <returns></returns>
	JsonValueType getType() const;

	/// <summary>
	/// Gets string representation of JsonValue object
	/// </summary>
	/// <returns></returns>
	std::string getValueString() const;
};
