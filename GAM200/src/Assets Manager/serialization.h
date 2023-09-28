#pragma once
/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>

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

	// Read/Write from the file stream
	virtual void readInt(int& i) = 0;
	virtual void readFloat(float& f) = 0;
	virtual void readString(std::string& str) = 0;
	virtual void writeInt(int& i) = 0;
	virtual void writeFloat(float& f) = 0;
	virtual void writeString(std::string& str) = 0;
};

// ALlows the component to run it's own serialization on creation
template<typename type>
inline void streamGet(Serialization& stream, type& typeInstance)
{
	typeInstance.Serialize(stream);
}

// Stream reading for common base types
inline void streamGet(Serialization& stream, int& i)
{
	stream.readInt(i);
}

inline void streamGet(Serialization& stream, float& f)
{
	stream.readFloat(f);
}

inline void streamGet(Serialization& stream, std::string& str)
{
	stream.readString(str);
}

// Same as above but for writing to stream
template<typename type>
inline void streamWrite(Serialization& stream, type& typeInstance)
{
	typeInstance.Deserialize(stream);
}

// Stream writing for common base types
inline void streamWrite(Serialization& stream, int& i)
{
	stream.writeInt(i);
}

inline void streamWrite(Serialization& stream, float& f)
{
	stream.writeFloat(f);
}

inline void streamWrite(Serialization& stream, std::string& str)
{
	stream.writeString(str);
}










