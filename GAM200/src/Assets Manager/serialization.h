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
	virtual void readFloat(int& f) = 0;
	virtual void readString(std::string& str) = 0;
	virtual void writeInt(int& i) = 0;
	virtual void writeFloat(int& f) = 0;
	virtual void writeString(std::string& str) = 0;
};













