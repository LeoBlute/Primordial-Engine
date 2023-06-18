#pragma once
#include <vector>
#include <algorithm>

//... Args = arguments
//T = Class associated
//Use example:
//class example_class{
//    example_class() {
//        ObjEvent<example_class, int> example_event(&example_class::example_function);
//        example_event.AddObj(this);
//        example_event.Invoke(5); }
//    void example_function(int a); };
template<typename T, typename... Args>
class ObjEvent
{
public:
    //Function with the arguments
    using Function = void (T::*)(Args...);

    ObjEvent() = delete;
    ~ObjEvent() = default;
    ObjEvent(const Function assignFunc, const unsigned int reserveAmount = 0)
    {
        mAssignedFunc = assignFunc;
        mObjs.reserve(reserveAmount);
    }

    inline void Invoke(Args... params)
    {
        for (T* obj : mObjs)
        {
            (*obj.*mAssignedFunc)(params...);
        }
    }

    inline void AddObj(T* obj)
    {
        auto it = std::find(mObjs.begin(), mObjs.end(), obj);
        if (it == mObjs.end())
        {
           mObjs.push_back(obj);
        }
    }

    inline void RemoveObj(T* obj)
    {
        auto it = std::find(mObjs.begin(), mObjs.end(), obj);
        if (it != mObjs.end())
        {
            mObjs.erase(it);
        }
    }

    inline bool HasObj(T* obj)
    {
        auto it = std::find(mObjs.begin(), mObjs.end(), obj);
        if (it != mObjs.end())
        {
            return 1;
        }
        return 0;
    }

private:
    std::vector<T*> mObjs;
    Function mAssignedFunc;
};

//... Args = arguments
//Use example:
//void example_function(int a);
//Event<int> example_event;
//example_event.AddFunc(example_function);
//example_event.Invoke(5);
template<typename... Args>
class Event
{
public:
   //Function with the arguments
   using Function = void (*)(Args...);

    Event() = default;
    ~Event() = default;

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