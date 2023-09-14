#include "text_serialization.h"

bool TextSerialization::openFileRead(const std::string& file)
{
	stream.open(file.c_str(), std::ios::in);
	return stream.is_open();
}

bool TextSerialization::openFileWrite(const std::string& file)
{
	stream.open(file.c_str(), std::ios::out | std::ios::trunc);
	return stream.is_open();
}

bool TextSerialization::closeFile()
{
	stream.close();
	return !stream.is_open();
}

bool TextSerialization::isGood()
{
	return stream.good();
}

void TextSerialization::readInt(int& i)
{
	stream >> i;
}

void TextSerialization::readFloat(int& f)
{
	stream >> f;
}

void TextSerialization::readString(std::string& str)
{
	stream >> str;
}

void TextSerialization::writeInt(int& i)
{
	stream << i;
}

void TextSerialization::writeFloat(int& f)
{
	stream << f;
}

void TextSerialization::writeString(std::string& str)
{
	stream << str;
}











