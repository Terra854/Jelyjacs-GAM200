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

	virtual bool openFileRead(const std::string& file); // Open stream for reading
	//virtual bool openFileWrite(const std::string& file); // Open stream for writing (overwrites existing)
	virtual bool closeFile(); // Close and delete jsonObject

	// Read/Write from the file stream
	virtual void readInt(int& i, std::string param1, std::string param2, int pos = -1);
	virtual void readFloat(float& f, std::string param1, std::string param2, int pos = -1);
	virtual void readFloat(float& f, std::string param1, std::string param2, std::string param3, int pos = -1);
	virtual void readString(std::string& str, std::string param, int pos = -1);
	virtual void readString(std::string& str, std::string param1, std::string param2, int pos = -1);
	virtual void readBool(bool& val, std::string param, int pos = -1);
	virtual void readBool(bool& val, std::string param1, std::string param2, int pos = -1);

	virtual bool isMember(std::string str);
	virtual bool isMember(std::string str, std::string param);
	auto read(std::string str) -> decltype((*jsonObject)[str]);
	//virtual void writeInt(int& i);
	//virtual void writeFloat(float& f);
	//virtual void writeString(std::string& str);

};













