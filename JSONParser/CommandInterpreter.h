#pragma once

#include "JsonValueAllocator.h"
#include "JsonObjectParser.h"
#include "JsonManager.h"

/// <summary>
/// Class used for interpreting and execution of user's commands.
/// </summary>
class CommandInterpreter
{
	
	JsonValueAllocator* allocator;
	JsonManager manager;
	JsonObjectParser parser;
	
	JsonValue* value;//loaded value
	std::string file;//source file path
	bool opened;//is it open?
	bool changed;//is it changed since last save
	JsonValue* searchSet;//search result

	/// <summary>
	/// Makes every letter from given string to lower case
	/// </summary>
	/// <param name="s">Given string</param>
	/// <returns>Copy of (s) but every letter is lower case</returns>
	std::string toLowerCase(std::string s);

	/// <summary>
	/// Splits argumenst of given command
	/// </summary>
	/// <param name="s">Given command</param>
	/// <returns>Collection of command's argumens</returns>
	std::vector<std::string> splitArguments(std::string s);

	/// <summary>
	/// Checks if given string is number
	/// </summary>
	/// <param name="num">String to check</param>
	/// <returns>true if string is number and false otherwise</returns>
	bool isNumber(std::string num);

	/// <summary>
	/// Writes given value to file(it overrides the file)
	/// </summary>
	/// <param name="path">Path to file where value will be saved</param>
	/// <param name="value">Value to save to the file</param>
	void writeToFile(std::string path, JsonValue& value) const;

	/// <summary>
	/// Clears all content of allocator and manager and parser that are connected with.
	/// </summary>
	void clearData();

	/// <summary>
	/// Asks user to enter json value.
	/// Input of the user has to end with <!END>
	/// </summary>
	/// <returns>Users json value as string</returns>
	std::string startJsonValueConversation() const;

	/// <summary>
	/// Opens and loads contents of give file
	/// </summary>
	/// <param name="filePath">Path to file for reading.</param>
	/// <returns>True if opening file is successful and false otherwise</returns>
	bool executeOpen(std::string filePath);

	/// <summary>
	/// Closes file and unloads old object from computer't memory.
	/// If file is not saved it asks user if he wants to save changes
	/// </summary>
	/// <returns>True if closing file is executed successfuly and 
	/// false if error is occured or user canceled saving changes</returns>
	bool executeClose();

	/// <summary>
	/// Creates new json value with initial value of null.
	/// No file path is given.
	/// </summary>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeNew();

	/// <summary>
	/// Saves changes(if loaded value has)of given json value to its source file 
	/// </summary>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeSave();

	/// <summary>
	/// Saves given json value(changed/unchanged) to given file path 
	/// </summary>
	/// <param name="path">File path to save loaded json value</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeSaveAs(std::string path);

	/// <summary>
	/// Prints last search result of user as json array
	/// </summary>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool printSearchSet();

	/// <summary>
	/// Saves full search result to given file  
	/// </summary>
	/// <param name="filePath">Path to file where search set will be saved</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool saveFullSearchSet(std::string filePath);

	/// <summary>
	/// Saves element from search result to given file  
	/// </summary>
	/// <param name="idx">Index of the element</param>
	/// <param name="filePath">Path to the file where element will be saved</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool saveElementFromSearchSet(std::size_t idx,std::string filePath);
	
	/// <summary>
	/// Prints loaded value
	/// </summary>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executePrint();

	/// <summary>
	/// Gets all elements that has given key in loaded value or its sub-object.
	/// Then loads all found results in sarchSet property of the class 
	/// </summary>
	/// <param name="searchKey">Key to search values in loaded object</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeSearch(std::string searchKey);

	/// <summary>
	/// Searches for certain value (by given path in loaded object) and overrides it to new json value.
	/// New value is entered from user via dialog(startJsonValueConversation() method)
	/// </summary>
	/// <param name="path">Path to element in loaded json object</param>
	/// <param name="newValue">New value to override the old one</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeEdit(std::string path, std::string newValue);

	/// <summary>
	/// Adds new value that has given path(has to be not existing) in loaded object
	/// </summary>
	/// <param name="path">New values's json path</param>
	/// <param name="val">New value as string</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeCreate(std::string path, std::string val);

	/// <summary>
	/// removes value at given json path
	/// </summary>
	/// <param name="path">Json path where is located value that will be deleted</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeRemove(std::string path);

	/// <summary>
	/// Moves json value located on first path and adds/overrides it on second path
	/// </summary>
	/// <param name="path1">Json path of value that is going to be moved</param>
	/// <param name="path2">Location where value has to be after execution of the fuction</param>
	/// <returns>True if function executes correctly and false otherwise</returns>
	bool executeMove(std::string path1, std::string path2);

public:
	/// <summary>
	/// Constructor. Sets allocator property and associates jsonParser and manager propertyes to the allocator.
	/// No json object is loaded.
	/// </summary>
	/// <param name="allocator">Allocator that is going to be used in the program</param>
	CommandInterpreter(JsonValueAllocator& allocator);
	
	/// <summary>
	/// Executes users commands.
	/// </summary>
	/// <param name="command">User's command as string</param>
	void executeCommand(std::string command);

	//Clears allocator, manager and parser's data
	~CommandInterpreter();
};
