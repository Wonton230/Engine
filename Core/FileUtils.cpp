#include "FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

int FileReadToBuffer(std::vector<uint8_t>& out_buffer, const std::string& filename)
{
    FILE* filePointer;
    errno_t err;
    err = fopen_s(&filePointer, filename.c_str(), "rb");
    if (err != 0)
    {
        ERROR_AND_DIE("File could not be opened or found. Please check the file name.");
    }
    err = fseek(filePointer, 0, SEEK_END);
    if (err != 0)
    {
        ERROR_AND_DIE("Could not seek to end of file, but file is open");
    }
    long fileSize = ftell(filePointer);
	if (fileSize == -1)
	{
		ERROR_AND_DIE("ftell call failed when reading to buffer.");
	}

    out_buffer.resize(fileSize);
    
	err = fseek(filePointer, 0, SEEK_SET);
	if (err != 0)
	{
		ERROR_AND_DIE("Could not seek to start of file, but file size is saved.");
	}
    long bytesRead = static_cast<long>(fread(out_buffer.data(), sizeof(uint8_t), static_cast<size_t>(fileSize), filePointer));
    if (bytesRead != fileSize)
    {
        ERROR_AND_DIE("Read bytes does not match file size. Incomplete read");
    }

    return bytesRead;
}

int FileReadToString(std::string& outString, const std::string& filename)
{
    std::vector<uint8_t> buffer;
    int bytesRead = FileReadToBuffer(buffer, filename);
    buffer.resize(static_cast<size_t>(buffer.size()) + 1);
    buffer[buffer.size()-1] = '\0';

    outString.assign(reinterpret_cast<char*>(buffer.data()), buffer.size() - 1);
	return bytesRead;
}
