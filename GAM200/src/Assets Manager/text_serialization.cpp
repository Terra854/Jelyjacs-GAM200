/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>
#include "text_serialization.h"
#include <iostream>


// Opens a file for reading, return true if successfully opened
bool TextSerialization::openFileRead(const std::string& file)
{
	stream.open(file.c_str(), std::ios::in);
	if (!stream.is_open()) {
		std::cout << "file is not found : " << file.c_str() << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "file open" << file.c_str() << std::endl;
	}
	return stream.is_open();
}

// Open a file for writing, returns true if successfully opened
bool TextSerialization::openFileWrite(const std::string& file)
{
	stream.open(file.c_str(), std::ios::out | std::ios::trunc);
	return stream.is_open();
}

// Close a file, returns true if file is closed
bool TextSerialization::closeFile()
{
	stream.close();
	return !stream.is_open();
}

// Check if a file stream is still good (e.g. have not reached end of file)
bool TextSerialization::isGood()
{
	return stream.good();
}

// Use to read the next interger
void TextSerialization::readInt(int& i)
{
	stream >> i;
}

// Use to read the next float
void TextSerialization::readFloat(float& f)
{
	stream >> f;
}

// Use to read the next string
void TextSerialization::readString(std::string& str)
{
	stream >> str;
}

// Use to write the next interger
void TextSerialization::writeInt(int& i)
{
	stream << i;
}

// Use to write the next float
void TextSerialization::writeFloat(float& f)
{
	stream << f;
}

// Use to write the next string
void TextSerialization::writeString(std::string& str)
{
	stream << str;
}











