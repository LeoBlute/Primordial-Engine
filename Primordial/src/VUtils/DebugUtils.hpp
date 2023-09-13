#pragma once
#include "TimeUtils.hpp"
#include "ANSICode.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ostream>


namespace DebugUtils {
    #pragma region Memory and Time
    //*Determines if should track the amount of memory allocated
    //*Enabled on EnableMemoryTracking()
    //*Disabled on DisableMemoryTracking()
    inline static bool gMemtrack = false;
    //The amount of memory allocated
    inline static size_t gAllocated = 0;
    //The amount of times a memory allocation happened
    inline static size_t gAllocatedCount = 0;
    //The amount of memory deleted
    inline static size_t gDeleted = 0;
    //The amount of times a memory deletion happened
    inline static size_t gDeletedCount = 0;

    //*Gets the current time at start
    //*Gets a new current time on ResetTimeTracking()
    inline static std::chrono::steady_clock::time_point gStartTimeTracker = std::chrono::steady_clock::now();

    #define IGNORE_TIME_TRACKING_BEGIN\
    std::chrono::steady_clock::time_point timepoint_begin =\
            std::chrono::steady_clock::now();


    #define IGNORE_TIME_TRACKING_END\
    std::chrono::steady_clock::time_point timepoint_end =\
            std::chrono::steady_clock::now();\
        gStartTimeTracker = DebugUtils::gStartTimeTracker - (timepoint_begin-timepoint_end);

    inline void EnableMemoryTracking() { gMemtrack = true; };

    inline void DisableMemoryTracking() { gMemtrack = false; };

    inline void ResetMemoryTacking()
    {
        gAllocated = 0;
        gAllocatedCount = 0;
        gDeleted = 0;
        gDeletedCount = 0;
    }

    inline void PrintMemory()
    {
        IGNORE_TIME_TRACKING_BEGIN;
        std::cout << "Allocated : " << gAllocated << std::endl;
        std::cout << "Count     : " << gAllocatedCount << std::endl;
        std::cout << "Deleted   : " << gDeleted << std::endl;
        std::cout << "Count     : " << gDeletedCount << std::endl;
        IGNORE_TIME_TRACKING_END;
    }

    template<typename T>
    inline void PrintMemorySizeOf(const char* name, T item)
    {
        IGNORE_TIME_TRACKING_BEGIN;
        std::cout << "Memory size of " << name << ":" << sizeof(item) << std::endl;
        IGNORE_TIME_TRACKING_END;
    }

    inline void ResetTimeTracking()
    {
        gStartTimeTracker = std::chrono::steady_clock::now();
    }

    inline void PrintTimeTracked()
    {
        IGNORE_TIME_TRACKING_BEGIN;
        double duration =
            std::chrono::duration_cast<std::chrono::microseconds>
            (timepoint_begin - gStartTimeTracker).count();
        std::cout << "Time:" << duration << "ms" << std::endl;
        IGNORE_TIME_TRACKING_END;
    }

    #pragma endregion

    #pragma region Logging
    template<typename T>
    constexpr static inline void AddToStream
    (std::ostream& stream, const T& arg)
    {
        stream << arg;
    }

    template<typename T, typename ...Args>
    constexpr static inline void AddToStream
    (std::ostream& stream, const T& arg, const Args&... args)
    {
        stream << arg;
        AddToStream(stream, args...);
    }

    template <typename... Args>
    static void Log(bool toFile, ANSICode ansiCode, Args... msg)
    {
        IGNORE_TIME_TRACKING_BEGIN;
        //final message that will appear
        std::string fmsg;
        //construct final message beginning text
        fmsg.append("-");
        TimeUtils::GetDateToStr(fmsg); //date
        std::ostringstream stream;
        AddToStream(stream, msg...);
        fmsg.append(stream.str());

        //if toFile is true it will write the log to a new file
        if (toFile)
        {
            std::fstream f; //file

            //Path + file name
            std::string fileName = "Logs\\Log-"; 
            TimeUtils::GetDateToStr(fileName);
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
        std::cout << ANSICodeToCStr(ansiCode) << fmsg << ANSICodeToCStr(ANSICode::Reset) << std::endl;
        IGNORE_TIME_TRACKING_END;
    }
    #pragma endregion
}

inline void* operator new(size_t size)
{
    if (DebugUtils::gMemtrack)
    {
        DebugUtils::gAllocated += static_cast<unsigned int>(size);
        DebugUtils::gAllocatedCount++;
    }

    return malloc(size);
}

inline void operator delete(void* memory, size_t size)
{
    if (DebugUtils::gMemtrack)
    {
        DebugUtils::gDeleted += static_cast<unsigned int>(size);
        DebugUtils::gDeletedCount++;
    }

    free(memory);
}

#ifdef DEBUG
    #define DEBUG_ACTIVATE_MEM_TRACK() DebugUtils::EnableMemoryTracking()
    #define DEBUG_DISABLE_MEM_TRACK() DebugUtils::DisableMemoryTracking()
    #define DEBUG_RESET_MEM_TRACK() DebugUtils::ResetMemoryTacking();
    #define DEBUG_PRINT_MEM() DebugUtils::PrintMemory();
    #define DEBUG_PRINT_MEMSIZEOF(NAME, ITEM) DebugUtils::PrintMemorySizeOf(NAME, ITEM);

    #define DEBUG_RESET_TIME_TRACK() DebugUtils::ResetTimeTracking();
    #define DEBUG_PRINT_TIME_TRACK() DebugUtils::PrintTimeTracked();

    #define DEBUG_INFO(...) DebugUtils::Log(false, ANSICode::Reset, "-Info:", __VA_ARGS__);
    #define DEBUG_WARN(...) DebugUtils::Log(false, ANSICode::BrightYellow,"-Warning:", __VA_ARGS__);
    #define DEBUG_ERROR(...) DebugUtils::Log(false, ANSICode::BrightRed, "-Error:", __VA_ARGS__);\
    __debugbreak();
    #define DEBUG_TODO(...) DebugUtils::Log(false, ANSICode::Reset, "-TODO:", __VA_ARGS__);
    
    #define DEBUG_FILE_INFO(...) DebugUtils::Log(true, ANSICode::Reset, "-Info:", __VA_ARGS__);
    #define DEBUG_FILE_WARN(...) DebugUtils::Log(true, ANSICode::BrightYellow, "-Warning:", __VA_ARGS__);
    #define DEBUG_FILE_ERROR(...) DebugUtils::Log(true, ANSICode::BrightRed, "-Erro:", __VA_ARGS__);\
    __debugbreak();
    #define DEBUG_FILE_TODO(...) DebugUtils::Log(true, ANSICode::Reset, "-TODO:", __VA_ARGS__);

#else
    #define DEBUG_ACTIVATE_MEM_TRACK()
    #define DEBUG_DISABLE_MEM_TRACK()
    #define DEBUG_RESET_MEM_TRACK()
    #define DEBUG_PRINT_MEM(
    #define DEBUG_PRINT_MEMSIZEOF(NAME, ITEM)

    #define DEBUG_RESET_TIME_TRACK()
    #define DEBUG_PRINT_TIME_TRACK()

    #define DEBUG_INFO(...)
    #define DEBUG_WARN(...)
    #define DEBUG_CRIT(...)
    #define DEBUG_ERROR(...)
    #define DEBUG_TODO(...)

    #define DEBUG_FILE_INFO(...) 
    #define DEBUG_FILE_WARN(...) 
    #define DEBUG_FILE_CRIT(...)
    #define DEBUG_FILE_ERROR(...)
    #define DEBUG_FILE_TODO(...)
#endif // DEBUG