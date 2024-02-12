#include "date.hpp"

enum class DateFormatterState {
	Default,
	Year,
	Months,
	Days,
	Hours,
	Hours24,
	Minutes,
	Seconds,
	AmPm
};

static const char* DateMonthNames[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"
};
static const char* DateMonthFullNames[] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

static const char* DateDayNames[] = {
	"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};
static const char* DateDayFullNames[] = {
	"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};

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

String Date::toString(const String& format) const {
	String result { };
	DateFormatterState state = DateFormatterState::Default;
	int chrCount = 0;
	int i = 0;

	for (char c : format) {
		switch (state) {
			default: {
				switch (c) {
					case 'y': case 'Y': {
						chrCount = 1;
						state = DateFormatterState::Year;
					} break;
					case 'M': {
						chrCount = 1;
						state = DateFormatterState::Months;
					} break;
					case 'd': case 'D': {
						chrCount = 1;
						state = DateFormatterState::Days;
					} break;
					case 'h': {
						chrCount = 1;
						state = DateFormatterState::Hours;
					} break;
					case 'H': {
						chrCount = 1;
						state = DateFormatterState::Hours24;
					} break;
					case 'm': {
						 chrCount = 1;
						 state = DateFormatterState::Minutes;
					} break;
					case 's': case 'S': {
						 chrCount = 1;
						 state = DateFormatterState::Seconds;
					} break;
					case 't': case 'T': {
						chrCount = 1;
						state = DateFormatterState::AmPm;
					} break;
					default: {
						if (std::isalnum(c)) {
							throw DateFormatException("Unknown format character '"_S + c + '\'');
						}
						result += c;
					} break;
				}
			} break;
			case DateFormatterState::Year: {
				if (c == 'y' || c == 'Y') {
					chrCount++;
				} else if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				}

				if ((c != 'y' && c != 'Y') || i == format.length() - 1) {
					String _year = String::From(year());
					if (chrCount < 3) {
						result += _year.substr(_year.length() - 3);
					} else {
						result += _year;
					}

					if (c != 'y' && c != 'Y') {
						result += c;
					}
					state = DateFormatterState::Default;
					chrCount = 0;
				}
			} break;
			case DateFormatterState::Months: {
				if (c == 'M') {
					chrCount++;
				} else if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				}

				if (c != 'M' || i == format.length() - 1) {
					String _month;
					if (chrCount <= 2) {
						_month = String::From(month());
						if (chrCount == 2) {
							while (_month.length() < 2) {
								_month.insert(0, '0');
							}
						}
					} else if (chrCount == 3) {
						_month = DateMonthNames[m_time.tm_mon];
					} else if (chrCount == 4) {
						_month = DateMonthFullNames[m_time.tm_mon];
					} else {
						_month = String::From(month());
					}
					result += _month;

					if (c != 'M') {
						result += c;
					}
					state = DateFormatterState::Default;
					chrCount = 0;
				}
			} break;
			case DateFormatterState::Days: {
				if (c == 'd' || c == 'D') {
					chrCount++;
				} else if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				}

				if ((c != 'd' && c != 'D') || i == format.length() - 1) {
					String _day;
					if (chrCount <= 2) {
						_day = String::From(day());
						if (chrCount == 2) {
							if (_day.length() < 2) {
								_day.insert(0, '0');
							}
						}
					} else if (chrCount == 3) {
						_day = DateDayNames[m_time.tm_mon];
					} else if (chrCount == 4) {
						_day = DateDayFullNames[m_time.tm_mon];
					} else {
						_day = String::From(day());
					}
					result += _day;

					if (c != 'd' && c != 'D') {
						result += c;
					}
					state = DateFormatterState::Default;
					chrCount = 0;
				}
			} break;
			case DateFormatterState::Hours: {
				if (c == 'h') {
					chrCount++;
				} else if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				}

				if (c != 'h' || i == format.length() - 1) {
					int _i_hours = hour();
					if (_i_hours > 12) {
						_i_hours -= 12;
					}

					String _hours = String::From(_i_hours);
					if (chrCount > 1) {
						if (_hours.length() < 2) {
							_hours.insert(0, '0');
						}
					}
					result += _hours;

					if (c != 'h') {
						result += c;
					}
					state = DateFormatterState::Default;
					chrCount = 0;
				}
			} break;
			case DateFormatterState::Hours24: {
				if (c == 'H') {
					chrCount++;
				} else if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				}

				if (c != 'H' || i == format.length() - 1) {
					String _hours = String::From(hour());
					if (chrCount > 1) {
						if (_hours.length() < 2) {
							_hours.insert(0, '0');
						}
					}
					result += _hours;

					if (c != 'H') {
						result += c;
					}
					state = DateFormatterState::Default;
					chrCount = 0;
				}
			} break;
			case DateFormatterState::Minutes: {
				if (c == 'm') {
					chrCount++;
				} else if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				}

				if (c != 'm' || i == format.length() - 1) {
					String _minutes = String::From(minute());
					if (chrCount > 1) {
						if (_minutes.length() < 2) {
							_minutes.insert(0, '0');
						}
					}
					result += _minutes;

					if (c != 'm') {
						result += c;
					}
					state = DateFormatterState::Default;
					chrCount = 0;
				}
			} break;
			case DateFormatterState::Seconds: {
				if (c == 's' || c == 'S') {
					chrCount++;
				} else if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				}

				if ((c != 's' && c != 'S') || i == format.length() - 1) {
					String _seconds = String::From(second());
					if (chrCount > 1) {
						if (_seconds.length() < 2) {
							_seconds.insert(0, '0');
						}
					}
					result += _seconds;

					if (c != 's' && c != 'S') {
						result += c;
					}
					state = DateFormatterState::Default;
					chrCount = 0;
				}
			} break;
			case DateFormatterState::AmPm: {
				result += hour() >= 12 && hour() < 24 ? "pm" : "am";

				if (std::isalnum(c)) {
					throw DateFormatException("Unexpected format character '"_S + c + '\'');
				} else {
					result += c;
				}

				state = DateFormatterState::Default;
				chrCount = 0;
			} break;
		}
		i++;
	}

	return result;
}
