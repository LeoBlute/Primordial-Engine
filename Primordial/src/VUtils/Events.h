#pragma once
#include <vector>

//.. Args = arguments
template<typename... Args>
class Event
{
public:
    //Function with the arguments
    using Function = void (*)(Args...);

    inline Event& operator+=(const Function func)
    {
        AddFunc(func);
        return *this;
    }

    inline Event& operator-=(const Function func)
    {
        RemoveFunc(func);
        return *this;
    }

    inline void operator()(Args... params)
    {
        Invoke(params...);
    }

    inline void AddFunc(const Function func)
    {
        auto it = std::find(mFunctions.begin(), mFunctions.end(), func);
        if (it == mFunctions.end())
        {
            mFunctions.push_back(func);
        }
    }

    inline void RemoveFunc(const Function func)
    {
        auto it = std::find(mFunctions.begin(), mFunctions.end(), func);
        if (it != mFunctions.end())
        {
            mFunctions.erase(it);
        }
    }

    inline void Invoke(Args... params)
    {
        for (auto function : mFunctions)
        {
            function(params...);
        }
    }

    inline bool HasFunction(const Function func)
    {
        auto it = std::find(mFunctions.begin(), mFunctions.end(), func);
        if (it != mFunctions.end())
        {
            return 1;
        }
        return 0;
    }

    inline const std::vector<Function>& GetFunctions() { return mFunctions; };
private:
    //Holder of all functions in event
    std::vector<Function> mFunctions;
};