#include "date.hpp"



namespace Gctk {
	Date::Date(const tm& time): m_time(time), m_iTime(mktime(&m_time)) { }
	Date::Date(const time_t& time): m_time(), m_iTime(time) {
		if (struct tm* tm = localtime(&m_iTime); tm != nullptr) {
			m_time = *tm;
		}
		throw Exception("Unable to get date!");
	}
	Date::Date(): Date(std::time(nullptr)) { }

	Date Date::Now() {
		return { };
	}
	Date Date::NowUTC() {
		time_t t = std::time(nullptr);
		if (struct tm* tm = std::gmtime(&t); tm != nullptr) {
			return Date(*tm);
		}
		return Now();
	}
}
