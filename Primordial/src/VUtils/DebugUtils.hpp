#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include <type_traits>
#include <filesystem>
#include <fstream>

namespace DebugUtils {
    template <typename T>
    static void Log(T msg, const char* additional, bool toFile)
    {
        //final message that will appear
        std::string fmsg;

        //construct final message beginning text
        fmsg.append("-");
        {
        time_t t0 = time(0);
		char cstr[26];
		ctime_s(cstr, sizeof(cstr), &t0);
		fmsg.append(cstr);
		fmsg.pop_back();
        }
        fmsg.append(additional); //type

        //checks if the variable is loggable
        if constexpr (std::is_arithmetic_v<T>) //if variable is arithmetic
        {
            fmsg.append(std::to_string(msg));
        }
        else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, const char*> || std::is_same_v<T, char*> ||
            std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, const char> ||
            std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>) //if variable is not arithmetic but is still loggable
        {
            fmsg.append(msg);
        }
        else if constexpr (std::is_same_v<T, std::string*>) //if variable is a std::string pointer
        {
            fmsg.append(msg->c_str());
        }
        else //if variable can not be printed
        {
            fmsg.append("Non loggable variable");
        }

        //if toFile is true it will write the log to a new file
        if (toFile)
        {
            std::fstream f; //file

            //Path + file name
            std::string fileName = "Logs\\Log-"; 
        {
            time_t t0 = time(0);
		    char cstr[26];
		    ctime_s(cstr, sizeof(cstr), &t0);
		    fileName.append(cstr);
		    fileName.pop_back();
        }
            for (char& c : fileName)
            {
                if (c == ' ' || c == ':')
                {
                    c = '-';
                }
            }
            fileName.append(".txt");

            //if directory Logs doesn't exists,it creates
            if (!std::filesystem::is_directory("Logs"))
            {
                std::filesystem::create_directory("Logs");
            }

            //Write to file
            f.open(fileName.c_str(), std::ios::app | std::ios::binary); //open
            f << fmsg; //send log to file
            f.close(); //close
        }

        //Log final message to console
        std::cout << fmsg << std::endl;

    }
}

#ifdef DEBUG
    //Log to console only
    #define DEBUG_INFO(MSG) DebugUtils::Log(MSG, "-Info:", false);
    #define DEBUG_WARN(MSG) DebugUtils::Log(MSG, "-Warning:", false);
    #define DEBUG_CRIT(MSG) DebugUtils::Log(MSG, "-Critical:", false);
    #define DEBUG_ERROR(MSG) DebugUtils::Log(MSG, "-Error:", false);\
    __debugbreak();
    #define DEBUG_TODO(MSG) DebugUtils::Log(MSG, "-TODO:", false);
    
    //Log to console and write to file
    #define DEBUG_FILE_INFO(MSG) DebugUtils::Log(MSG, "-Info:", true);
    #define DEBUG_FILE_WARN(MSG) DebugUtils::Log(MSG, "-Warning:", true);
    #define DEBUG_FILE_CRIT(MSG) DebugUtils::Log(MSG, "-Critical:", true);
    #define DEBUG_FILE_ERROR(MSG) DebugUtils::Log(MSG, "-Error:", true);\
    __debugbreak();
    #define DEBUG_FILE_TODO(MSG) DebugUtils::Log(MSG, "-TODO:", true);

#else
//Log to console only
    #define DEBUG_INFO(MSG)
    #define DEBUG_WARN(MSG)
    #define DEBUG_CRIT(MSG)
    #define DEBUG_ERROR(MSG)
    #define DEBUG_TODO(MSG)

//Log to console and write to file
    #define DEBUG_FILE_INFO(MSG) 
    #define DEBUG_FILE_WARN(MSG) 
    #define DEBUG_FILE_CRIT(MSG)
    #define DEBUG_FILE_ERROR(MSG)
    #define DEBUG_FILE_TODO(MSG)
#endif // DEBUG