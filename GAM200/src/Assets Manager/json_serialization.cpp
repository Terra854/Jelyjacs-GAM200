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


// Close a file, returns true if file is closed
bool JsonSerialization::closeFile()
{
	delete jsonObject;
	return true;
}

// Use to read the next interger
void JsonSerialization::readInt(int& i, std::string param1, std::string param2)
{
	i = (*jsonObject)[param1][param2].asInt();
}

// Use to read the next float
void JsonSerialization::readFloat(float& f, std::string param1, std::string param2)
{
	f = (*jsonObject)[param1][param2].asFloat();
}

// Use to read the next float
void JsonSerialization::readFloat(float& f, std::string param1, std::string param2, std::string param3)
{
	f = (*jsonObject)[param1][param2][param3].asFloat();
}

// Use to read the next string
void JsonSerialization::readString(std::string& str, std::string param)
{
	str = (*jsonObject)[param].asString();
}

void JsonSerialization::readString(std::string& str, std::string param1, std::string param2)
{
	str = (*jsonObject)[param1][param2].asString();
}

auto JsonSerialization::read(std::string str) -> decltype((*jsonObject)[str])
{
	return (*jsonObject)[str];
}

// Use to write the next interger


// Use to write the next float


// Use to write the next string










