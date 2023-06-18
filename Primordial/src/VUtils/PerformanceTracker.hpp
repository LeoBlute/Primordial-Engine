#pragma once
#include <iostream>
#include <chrono>

//*Determines if should track the amount of memory allocated
//*Enabled on EnableMemoryTracking()
//*Disabled on DisableMemoryTracking()
inline static bool gMemtrack = 0;
inline static unsigned int gAllocated = 0; //The amount of memory allocated
inline static unsigned int gAllocatedCount = 0; //The amount of times a memory allocation happened
inline static unsigned int gDeleted = 0; //The amount of memory deleted
inline static unsigned int gDeletedCount = 0; //The amount of times a memory deletion happened

//*Gets the current time at start
//*Gets a new current time on ResetTimeTracking()
inline static std::chrono::steady_clock::time_point gStartTimeTracker = std::chrono::steady_clock::now();

inline void EnableMemoryTracking() { gMemtrack = 1; };

inline void DisableMemoryTracking() { gMemtrack = 0; };

inline void ResetMemoryTacking()
{
    gAllocated = 0;
    gAllocatedCount = 0;
    gDeleted = 0;
    gDeletedCount = 0;
}

inline void PrintMemory()
{
    std::cout << "Allocated : " << gAllocated << std::endl;
    std::cout << "Count     : " << gAllocatedCount << std::endl;
    std::cout << "Deleted   : " << gDeleted << std::endl;
    std::cout << "Count     : " << gDeletedCount << std::endl;
}

template<typename T>
inline void PrintMemorySizeOf(const char* name, T item)
{
    std::cout << "Memory size of " << name << ":" << sizeof(item) << std::endl;
}

inline void ResetTimeTracking()
{
    gStartTimeTracker = std::chrono::steady_clock::now();
}

inline void PrintTimeTracking()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(now - gStartTimeTracker).count();
    std::cout << "Time:" << duration << "ms" << std::endl;
}

inline void* operator new(size_t size)
{
    if (gMemtrack)
    {
        gAllocated += static_cast<unsigned int>(size);
        gAllocatedCount++;
    }

    return malloc(size);
}

inline void operator delete(void* memory, size_t size)
{
    if (gMemtrack)
    {
        gDeleted += static_cast<unsigned int>(size);
        gDeletedCount++;
    }

    free(memory);
}