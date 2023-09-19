#pragma once

#include <string>
#include <fstream>

// Base serialization class
class Serialization
{
public:
	virtual bool openFileRead(const std::string& file) = 0; // Open stream for reading
	virtual bool openFileWrite(const std::string& file) = 0; // Open stream for writing (overwrites existing)
	virtual bool closeFile() = 0; // Close file
	virtual bool isGood() = 0; // Check if the file is good e.g check eof
	virtual void readInt(int& i) = 0;
	virtual void readFloat(float& f) = 0;
	virtual void readString(std::string& str) = 0;
	virtual void writeInt(int& i) = 0;
	virtual void writeFloat(float& f) = 0;
	virtual void writeString(std::string& str) = 0;
};

//Base case of serialization is that the object serializes itself.
// Reference from Sample:
template<typename type>
inline void StreamRead(Serialization& stream, type& typeInstance)
{
	typeInstance.Serialize(stream);
}

//Base non recursive stream operators of fundamental types.
inline void StreamRead(Serialization& stream, int& i)
{
	stream.readInt(i);
}

inline void StreamRead(Serialization& stream, float& f)
{
	stream.readFloat(f);
}

inline void StreamRead(Serialization& stream, std::string& str)
{
	stream.readString(str);
}

// Same as above but for writing to stream
template<typename type>
inline void StreamWrite(Serialization& stream, type& typeInstance)
{
	typeInstance.Serialize(stream);
}

//Base non recursive stream operators of fundamental types.
inline void StreamWrite(Serialization& stream, int& i)
{
	stream.writeInt(i);
}

inline void StreamWrite(Serialization& stream, float& f)
{
	stream.writeFloat(f);
}

inline void StreamWrite(Serialization& stream, std::string& str)
{
	stream.writeString(str);
}










