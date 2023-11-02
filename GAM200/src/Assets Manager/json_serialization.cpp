/* !
@file	text_serialization.cpp
@author	Tay Sen Chuan (t.senchuan@digipen.edu)
@date	28/9/2023

This file contains the definition of class TextSerialization. TextSerialization inherits 
from the Serialization class, but is designed to be used with text files.

*//*__________________________________________________________________________*/
#include <json/json.h>

#include <Debug.h>
#include "json_serialization.h"
#include <iostream>


// Opens a file for reading, return true if successfully opened
bool JsonSerialization::openFileRead(const std::string& file)
{
	std::ifstream stream;
	
	stream.open(file.c_str(), std::ios::in);
	if (!stream.is_open()) 
	{
		std::cout << "file is not found : " << file.c_str() << std::endl;
		exit(EXIT_FAILURE);
	}
	else 
	{
		std::cout << "file open..." << file.c_str() << std::endl;
		jsonObject = new Json::Value;
		Json::Reader reader;

		if (!reader.parse(stream, *jsonObject))
		{
			std::cout << "Failed to parse JSON" << std::endl;
			stream.close();
			return false;
		}


		return true;
	}
	return false;
}

// Open a file for writing, returns true if successfully opened
bool JsonSerialization::openFileWrite(const std::string& file)
{
	std::ofstream outputFile(file);
	if (outputFile.is_open()) {
		Json::StreamWriterBuilder writer;
		writer["indentation"] = "  ";

		outputFile << Json::writeString(writer, jsonObject);
		outputFile.close();
		std::cout << file << " has been successfully saved." << std::endl;
		return true;
	}
	else
		std::cerr << "Failed to open file for writing." << std::endl;
	return false;
}

// Close a file, returns true if file is closed
bool JsonSerialization::closeFile()
{
	delete jsonObject;
	return true;
}

bool JsonSerialization::checkValidParam(int pos, std::string param1, std::string param2, std::string param3)
{
	// Check Param1
	if (param1.empty())
		return false;
	else if (!jsonObject->isMember(param1))
		return false; // Invalid param1

	// Check param2
	if (param2.empty())
	{
		// Ensure pos is valid
		if (pos != -1 && (!(*jsonObject)[param1].isArray() || pos >= static_cast<int>((*jsonObject)[param1].size())))
			return false; // Invalid pos
	}
	else if (!(*jsonObject)[param1].isMember(param2))
		return false; // Invalid param2
		
	// Check param3
	if (param3.empty())
	{
		// Ensure pos is valid
		if (pos != -1 && (!(*jsonObject)[param1][param2].isArray() || pos >= static_cast<int>((*jsonObject)[param1][param2].size())))
			return false; // Invalid pos
	}
	else if (!(*jsonObject)[param1][param2].isMember(param3))
		return false; // Invalid param3

	if (!param3.empty())
	{
		// Ensure pos is valid
		if (pos != -1 && (!(*jsonObject)[param1][param2][param3].isArray() || pos >= static_cast<int>((*jsonObject)[param1][param2][param3].size())))
			return false; // Invalid pos
	}

	return true; // All valid
}

void JsonSerialization::readInt(int& i, std::string param, int pos)
{
	if (!checkValidParam(pos, param)) return;

	if (pos == -1)
		i = (*jsonObject)[param].asInt();
	else
		i = (*jsonObject)[param][pos].asInt();
}

// Use to read the next interger
void JsonSerialization::readInt(int& i, std::string param1, std::string param2, int pos)
{
	if (!checkValidParam(pos, param1, param2)) return;

	if (pos == -1)
		i = (*jsonObject)[param1][param2].asInt();
	else
		i = (*jsonObject)[param1][param2][pos].asInt();
}

void JsonSerialization::readFloat(float& f, std::string param, int pos)
{
	if (!checkValidParam(pos, param)) return;

	if (pos == -1)
		f = (*jsonObject)[param].asFloat();
	else
		f = (*jsonObject)[param][pos].asFloat();
}

// Use to read the next float
void JsonSerialization::readFloat(float& f, std::string param1, std::string param2, int pos)
{
	if (!checkValidParam(pos, param1, param2)) return;

	if (pos == -1)
		f = (*jsonObject)[param1][param2].asFloat();
	else
		f = (*jsonObject)[param1][param2][pos].asFloat();
}

// Use to read the next float
void JsonSerialization::readFloat(float& f, std::string param1, std::string param2, std::string param3, int pos)
{
	if (!checkValidParam(pos, param1, param2, param3)) return;

	if (pos == -1)
		f = (*jsonObject)[param1][param2][param3].asFloat();
	else
		f = (*jsonObject)[param1][param2][param3][pos].asFloat();
}

// Use to read the next string
void JsonSerialization::readString(std::string& str, std::string param, int pos)
{
	if (!checkValidParam(pos, param)) return;

	if (pos == -1)
		str = (*jsonObject)[param].asString();
	else
		str = (*jsonObject)[param][pos].asString();
}

// Use to read the next string
void JsonSerialization::readString(std::string& str, std::string param1, std::string param2, int pos)
{
	if (!checkValidParam(pos, param1, param2)) return;

	if (pos == -1)
		str = (*jsonObject)[param1][param2].asString();
	else
		str = (*jsonObject)[param1][param2][pos].asString();
}

void JsonSerialization::readBool(bool& val, std::string param, int pos)
{
	if (!checkValidParam(pos, param)) return;

	if (pos == -1)
		val = (*jsonObject)[param].asBool();
	else
		val = (*jsonObject)[param][pos].asBool();
}

void JsonSerialization::readBool(bool& val, std::string param1, std::string param2, int pos)
{
	if (!checkValidParam(pos, param1, param2)) return;

	if (pos == -1)
		val = (*jsonObject)[param1][param2].asBool();
	else
		val = (*jsonObject)[param1][param2][pos].asBool();
}

bool JsonSerialization::isMember(std::string str)
{
	if ((*jsonObject).isMember(str))
		return true;
	else
		return false;
}

bool JsonSerialization::isMember(std::string str, std::string param)
{
	if ((*jsonObject)[param].isMember(str))
		return true;
	else
		return false;
}

// For for-loop range usage
auto JsonSerialization::read(std::string str) -> decltype((*jsonObject)[str])
{
	return (*jsonObject)[str];
}

//template <typename T>
//void JsonSerialization::writeData(std::string param, T value)
//{
//	(*jsonObject)[param] = value;
//}
//
//template <typename T>
//void JsonSerialization::writeArrData(std::string param, T value)
//{
//	jsonArr[param] = value;
//}
//
//void JsonSerialization::appendToArr()
//{
//	jsonObject->append(jsonArr);
//	jsonArr.clear();
//}










