#pragma once

#include "JSON.h"
#include <string>
#include <deque>
#include "JsonValueAllocator.h"


class JsonObjectParser
{
	JsonValueAllocator* values;

	/// <summary>
	/// Removes tailing zeros after decimal point of given number(as string)
	/// Needed because std::to_string() method works like that. 
	/// </summary>
	/// <param name="s">Number(as string) where will be removed unnecessary zeros after decimal point</param>
	/// <returns>Number(as string) without unnecessary zeros</returns>
	std::string removeTailingZeros(std::string s);

	/// <summary>
	/// Parses to boolean(JsonValue) given string 
	/// </summary>
	/// <param name="b">String to be parsed</param>
	/// <returns>Pointer to parsed value or nullptr if method failed to parse boolean</returns>
	JsonValue* parseBoolean(const char* b);

	/// <summary>
	/// Parses to null(JsonValue) given string 
	/// </summary>
	/// <param name="nval">String to be parsed</param>
	/// <returns>Pointer to parsed value or nullptr if method failed to parse null</returns>
	JsonValue* parsenNull(const char* nval);

	/// <summary>
	/// Parses to string(JsonValue) given string(c-string)
	/// </summary>
	/// <param name="string">String to be parsed</param>
	/// <returns>Pointer to parsed value or nullptr if method failed to parse string</returns>
	JsonValue* parseString(const char* string);

	/// <summary>
	/// Parses to number(JsonValue) given string
	/// </summary>
	/// <param name="number">String to be parsed</param>
	/// <returns>Pointer to parsed value or nullptr if method failed to parse number</returns>
	JsonValue* parseNumber(const char* number);

	/// <summary>
	/// Parses to null(JsonValue) given string.
	/// Function is recursive
	/// </summary>
	/// <param name="arr">String to be parsed</param>
	/// <param name="start">Starting index of parsing</param>
	/// <returns>Pointer to parsed value or nullptr if method failed to parse array</returns>
	JsonValue* parseArray(const char* arr, std::size_t& start);

	/// <summary>
	/// Parses to object(JsonValue) given string.
	/// Function is recursive
	/// </summary>
	/// <param name="obj">String to be parsed</param>
	/// <param name="start">Starting index of parsing</param>
	/// <returns>Pointer to parsed value or nullptr if method failed to parse object</returns>
	JsonValue* parseObject(const char* obj, std::size_t& start);
	

	/// <summary>
	/// Extracts leftmost string(quoted) from given string
	/// If given string starts incorrectly returns ""
	/// </summary>
	/// <param name="str">String to search</param>
	/// <returns>Found value as string</returns>
	std::string extractString(const char* str);

	/// <summary>
	/// Extracts leftmost number from given string
	/// If given string starts incorrectly returns ""
	/// </summary>
	/// <param name="str">String to search</param>
	/// <returns>Found value as string</returns>
	std::string extractNumber(const char* str);

	/// <summary>
	/// Extracts leftmost bool from given string
	/// If given string starts incorrectly returns ""
	/// </summary>
	/// <param name="str">String to search</param>
	/// <returns>Found value as string</returns>
	std::string extractBoolean(const char* str);

	/// <summary>
	/// Extracts leftmost null from given string
	/// If given string starts incorrectly returns ""
	/// </summary>
	/// <param name="str">String to search</param>
	/// <returns>Found value as string</returns>
	std::string extractNull(const char* str);

public:
	/// <summary>
	/// Associates allocator to given allocator object.
	/// </summary>
	/// <param name="a">Allocator object</param>
	JsonObjectParser(JsonValueAllocator& a);

	/// <summary>
	/// Parses given string to every possible json value
	/// </summary>
	/// <param name="value">String to parse</param>
	/// <returns>Pointer to parsed value or nullptr if method failed to parse</returns>
	JsonValue* parse(const char* value);

	/// <summary>
	/// Clears allocators data
	/// </summary>
	void clearParser();
};

