#pragma once

#include <ctime>

#include "string.hpp"

class DateFormatException : public Exception {
public:
	explicit DateFormatException(const String& message): Exception(message) { }
};

class Date {
	time_t m_iTime;
	struct tm m_time;
public:
	Date();
	explicit Date(const struct tm& time);
	explicit Date(const time_t& time);

	[[nodiscard]] inline constexpr int year() const { return m_time.tm_year + 1900; }
	[[nodiscard]] inline constexpr int month() const { return m_time.tm_mon + 1; }
	[[nodiscard]] inline constexpr int day() const { return m_time.tm_mday; }
	[[nodiscard]] inline constexpr int hour() const { return m_time.tm_hour + 1; }
	[[nodiscard]] inline constexpr int minute() const { return m_time.tm_min + 1; }
	[[nodiscard]] inline constexpr int second() const { return m_time.tm_sec + 1; }
	[[nodiscard]] inline constexpr bool isDaylightSaving() const { return m_time.tm_isdst != 0; }

	[[nodiscard]] inline constexpr uint64_t totalSeconds() const { return static_cast<uint64_t>(m_iTime); }

	[[nodiscard]] inline constexpr auto operator<=>(const Date& date) const { return m_iTime <=> date.m_iTime; }

	static Date Now();
	static Date NowUTC();

	[[nodiscard]] String toString(const String& format) const;
};