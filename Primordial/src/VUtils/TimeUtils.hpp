#pragma once
#include <string>
#include <ctime>

namespace TimeUtils {
	//Complete date
	static const std::string GetDate()
	{
		time_t t0 = time(0);
		char str[26];
		ctime_s(str, sizeof(str), &t0);
		std::string res = std::string(str);
		res.pop_back();
		return (res);
	}

	//Complete date directly appended to a std::string for performance purposes
	static void GetDateToStr(std::string& str)
	{
		time_t t0 = time(0);
		char cstr[26];
		ctime_s(cstr, sizeof(cstr), &t0);
		str.append(cstr);
		str.pop_back();
	}

	//Year
	static const int GetYear()
	{
		time_t tNow = time(0);

		tm tmNow;
		localtime_s(&tmNow, &tNow);

		return tmNow.tm_year + 1900;
	}

	//Month
	static const int GetMonth()
	{
		time_t tNow = time(0);

		tm tmNow;
		localtime_s(&tmNow, &tNow);

		return tmNow.tm_mon + 1;
	}

	//Hour
	static const int GetHour()
	{
		time_t tNow = time(0);

		tm tmNow;
		localtime_s(&tmNow, &tNow);

		return tmNow.tm_hour;
	}

	//Day of the month
	static const int GetMonthDay()
	{
		time_t tNow = time(0);

		tm tmNow;
		localtime_s(&tmNow, &tNow);

		return tmNow.tm_mday;
	}

	//Day of the year
	static const int GetYearDay()
	{
		time_t tNow = time(0);

		tm tmNow;
		localtime_s(&tmNow, &tNow);

		return tmNow.tm_yday;
	}

	//Current minutes of the hour
	static const int GetMinutes()
	{
		time_t tNow = time(0);

		tm tmNow;
		localtime_s(&tmNow, &tNow);

		return tmNow.tm_min;
	}

	//Current seconds of the minute
	static const int GetSeconds()
	{
		time_t tNow = time(0);

		tm tmNow;
		localtime_s(&tmNow, &tNow);

		return tmNow.tm_sec;
	}
}
