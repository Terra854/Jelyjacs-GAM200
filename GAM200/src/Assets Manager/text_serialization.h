#pragma once
/* !
@file	text_serialization.h
@author	Tay Sen Chuan (t.senchuan@digipen.edu)
@date	28/9/2023

This file contains the declaration of the class TextSerialization.

*//*__________________________________________________________________________*/
#include <Debug.h>

#include "serialization.h"

// Serialization class for text file (.txt)
class TextSerialization : public Serialization
{
public:
	std::fstream stream;
	virtual bool openFileRead(const std::string& file); // Open stream for reading
	virtual bool openFileWrite(const std::string& file); // Open stream for writing (overwrites existing)
	virtual bool closeFile(); // Close file
	virtual bool isGood(); // Check if the file is good e.g check eof

	// Read/Write from the file stream
	virtual void readInt(int& i);
	virtual void readFloat(float& f);
	virtual void readString(std::string& str);
	virtual void writeInt(int& i);
	virtual void writeFloat(float& f);
	virtual void writeString(std::string& str);

};













