#pragma once
/* !
@file	text_serialization.h
@author	Tay Sen Chuan (t.senchuan@digipen.edu)
@date	28/9/2023

This file contains the declaration of the class JsonSerialization.

*//*__________________________________________________________________________*/
#include <json/json.h>
#include <Debug.h>

#include "serialization.h"

// Serialization class for json file (.json)
class JsonSerialization
{
public:

	Json::Value* jsonObject;
	Json::Value jsonArr;

	JsonSerialization()
	{
		jsonArr = Json::Value(Json::arrayValue);
	}

	virtual bool openFileRead(const std::string& file); // Open stream for reading
	virtual bool openFileWrite(const std::string& file); // Open stream for writing (overwrites existing)
	virtual bool closeFile(); // Close and delete jsonObject

	// Ensure that the pos and params (if exist) are valid when reading values from json file
	bool checkValidParam(int pos = -1, std::string param1 = "", std::string param2 = "", std::string param3 = "");

	// Read/Write from the file stream (pos is -1 when not needing to read json array)
	virtual void readInt(int& i, std::string param, int pos = -1);
	virtual void readInt(int& i, std::string param1, std::string param2, int pos = -1);
	virtual void readFloat(float& f, std::string param, int pos = -1);
	virtual void readFloat(float& f, std::string param1, std::string param2, int pos = -1);
	virtual void readFloat(float& f, std::string param1, std::string param2, std::string param3, int pos = -1);
	virtual void readString(std::string& str, std::string param, int pos = -1);
	virtual void readString(std::string& str, std::string param1, std::string param2, int pos = -1);
	virtual void readBool(bool& val, std::string param, int pos = -1);
	virtual void readBool(bool& val, std::string param1, std::string param2, int pos = -1);

	//virtual void WriteInt(Json::Value& data, std::string param, int value);
	//virtual void WriteInt(Json::Value& data, std::string param1, std::string param2, int value);
	//virtual void WriteFloat(Json::Value& data, std::string param, int value);
	//virtual void WriteFloat(Json::Value& data, std::string param1, std::string param2, int value);
	//virtual void WriteString(Json::Value& data, std::string param, int value);
	//virtual void WriteString(Json::Value& data, std::string param1, std::string param2, int value);
	//virtual void WriteBool(Json::Value& data, std::string param, int value);
	//virtual void WriteBool(Json::Value& data, std::string param1, std::string param2, int value);

	//template <typename T>
	//void writeData(std::string param, T value);

	//template <typename T>
	//void writeArrData(std::string param, T value);

	//void appendToArr();

	virtual bool isMember(std::string str);
	virtual bool isMember(std::string str, std::string param);
	auto read(std::string str) -> decltype((*jsonObject)[str]);


};


