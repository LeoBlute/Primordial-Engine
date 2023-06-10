#pragma once
#include "DebugUtils.h"

namespace FileUtils {
    //Load content of a file to a char* and returns it
    static char* CPReadFile(const char* path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            DEBUG_CRIT("Unable to open file");
        }

        //Get the size of the file
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        //Allocate a char* to hold the file data
        std::size_t size = static_cast<std::size_t>(fileSize) + 1;
        char* buffer = new char[size];

        //Read the file into the buffer
        file.read(buffer, fileSize);
        buffer[fileSize] = '\0';

        //Close file
        file.close();

        return buffer;
    }
    
    //Load content of a file to a std::string and returns it
    static std::string StrReadFile(const char* path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            DEBUG_CRIT("Unable to open file");
        }

        //get the size of the file
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        //makes a std::string to hold the file data
        std::string data;
        data.resize(static_cast<std::size_t>(fileSize));

        //Read the file into the string
        file.read(&data[0], fileSize);

        //Close file
        file.close();

        return data;
    }

    //*Self explanatory
    //*the path must also include the file name and extension
    //*return true if file could be created
    static bool CreateFile(const char* path)
    {
        //File
        std::ofstream file;
        //File opened/created
        file.open(path, std::ios::out | std::ios::app);
        if (!file.is_open())
        {
            return 0;
        }
        //File closed
        file.close();
        return 1;
    }

    //*Write the data from a const char* or char* to a file
    //*the path must also include the file name and extension
    //*return true if file can be opened/exists
    static bool CPWriteToFile(const char* path, const char* data)
    {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            return 0;
        }
        file.write(data, strlen(data));
        file.close();
        return 1;
    }

    //*Write the data from a std::string to a file
    //*the path must also include the file name and extension
    //*return true if file can be opened/exists
    static bool StrWriteToFile(const char* path, const std::string& data)
    {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            return 0;
        }
        file.write(data.c_str(), strlen(data.c_str()));
        file.close();
        return 1;
    }
}