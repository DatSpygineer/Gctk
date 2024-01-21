#include "string.hpp"

#include <cstdlib>
#include <cstring>

#ifndef __GNUC__
	#define strcasecmp _stricmp
	#define strncasecmp _strnicmp
#endif

String::String(char c, size_t n): m_sString(n, '\0') {
	std::memset(m_sString.data(), c, n * sizeof(char));
}

String& String::operator=(String&& other) noexcept {
	m_sString = std::move(other.m_sString);
	other.m_sString = nullptr;

	return *this;
}

String String::trim_start(const String& chars) const {
	size_t idx = find_first_not_of(chars);
	if (idx == NoFind) {
		return "";
	}
	return substr(idx);
}
String String::trim_end(const String& chars) const {
	size_t idx = find_last_not_of(chars);
	if (idx == NoFind) {
		return "";
	}
	return substr(0, idx + 1);
}
String String::trim(const String& chars) const {
	return trim_start(chars).trim_end(chars);
}

String String::to_lowercase() const {
	String str = *this;
	for (char& c : str) {
		if (c >= 'A' && c <= 'Z') {
			c += ('a' - 'A');
		}
	}
	return str;
}
String String::to_uppercase() const {
	String str = *this;
	for (char& c : str) {
		if (c >= 'a' && c <= 'z') {
			c -= ('a' - 'A');
		}
	}
	return str;
}

String::iterator String::insert(size_t at, char c) {
	return insert(begin() + static_cast<std::string::difference_type>(at), c);
}
String::iterator String::insert(size_t at, const String& str) {
	m_sString.insert(at, str.m_sString);
	return begin() + static_cast<std::string::difference_type>(at);
}
String::iterator String::insert(const String::iterator& at, char c) {
	return m_sString.insert(at, c);
}
String::iterator String::insert(const String::iterator& at, const String& str) {
	return insert(std::distance(begin(), at), str);
}

String::iterator String::erase(const String::iterator& at) {
	return m_sString.erase(at);
}
String::iterator String::erase(const String::iterator& beg, const String::iterator& end) {
	return m_sString.erase(beg, end);
}
String::iterator String::erase(size_t at) {
	return m_sString.erase(begin() + static_cast<std::string::difference_type>(at));
}
String::iterator String::erase(size_t at, size_t n) {
	return m_sString.erase(begin() + static_cast<std::string::difference_type>(at), begin() + static_cast<std::string::difference_type>(at + n));
}

String& String::replace(size_t at, size_t n, const String& replacement) {
	m_sString.replace(at, n, replacement.m_sString);
	return *this;
}
String& String::replace(const String::iterator& beg, const String::iterator& end, const String& replacement) {
	m_sString.replace(beg, end, replacement.m_sString);
	return *this;
}
String& String::replace(const String& to_replace, const String& replacement) {
	size_t idx = find(to_replace);
	if (idx != NoFind) {
		replace(idx, to_replace.length(), replacement);
	}
	return *this;
}
String& String::replace_all(const String& to_replace, const String& replacement) {
	size_t idx;
	while ((idx = find(to_replace)) != NoFind) {
		replace(idx, to_replace.length(), replacement);
	}
	return *this;
}

String& String::remove_prefix(const String& prefix) {
	if (starts_with(prefix)) {
		erase(0, prefix.length());
	}

	return *this;
}
String& String::remove_suffix(const String& suffix) {
	if (ends_with(suffix)) {
		erase(length() - suffix.length() - 1, suffix.length());
	}

	return *this;
}
String& String::remove_prefix(char prefix) {
	if (starts_with(prefix)) {
		erase(0);
	}
	return *this;
}
String& String::remove_suffix(char suffix) {
	if (ends_with(suffix)) {
		erase(length() - 1);
	}
	return *this;
}

String String::without_prefix(const String& prefix) const {
	if (starts_with(prefix)) {
		return substr(prefix.length());
	}
	return *this;
}
String String::without_suffix(const String& suffix) const {
	if (ends_with(suffix)) {
		return substr(0, length() - suffix.length() - 1);
	}
	return *this;
}
String String::without_prefix(char prefix) const {
	if (starts_with(prefix)) {
		return substr(1);
	}
	return *this;
}
String String::without_suffix(char suffix) const {
	if (ends_with(suffix)) {
		return substr(0, length() - 1);
	}
	return *this;
}

int String::compare(const String& other) const {
	return std::strcmp(m_sString.data(), other.m_sString.data());
}
int String::compare(const String& other, size_t n) const {
	return std::strncmp(m_sString.data(), other.m_sString.data(), n);
}
int String::compare(const String& other, size_t offset, size_t n) const {
	return std::strncmp(m_sString.data(), other.m_sString.data() + offset, n);
}
int String::compare(const String& other, size_t this_offset, size_t offset, size_t n) const {
	return std::strncmp(m_sString.data() + this_offset, other.m_sString.data() + offset, n);
}

int String::compare_no_case(const String& other) const {
	return strcasecmp(m_sString.data(), other.m_sString.data());
}
int String::compare_no_case(const String& other, size_t n) const {
	return strncasecmp(m_sString.data(), other.m_sString.data(), n);
}
int String::compare_no_case(const String& other, size_t offset, size_t n) const {
	return strncasecmp(m_sString.data(), other.m_sString.data() + offset, n);
}
int String::compare_no_case(const String& other, size_t this_offset, size_t offset, size_t n) const {
	return strncasecmp(m_sString.data() + this_offset, other.m_sString.data() + offset, n);
}

bool String::parse(signed char& value, int base) const {
	try {
		value = static_cast<signed char>(std::stoi(m_sString, nullptr, base));
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(short& value, int base) const {
	try {
		value = static_cast<short>(std::stoi(m_sString, nullptr, base));
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(int& value, int base) const {
	try {
		value = std::stoi(m_sString, nullptr, base);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(long& value, int base) const {
	try {
		value = std::stol(m_sString, nullptr, base);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(long long& value, int base) const {
	try {
		value = std::stoll(m_sString, nullptr, base);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(unsigned char& value, int base) const {
	try {
		value = static_cast<unsigned char>(std::stoul(m_sString, nullptr, base));
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(unsigned short& value, int base) const {
	try {
		value = static_cast<unsigned short>(std::stoul(m_sString, nullptr, base));
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(unsigned int& value, int base) const {
	try {
		value = static_cast<unsigned int>(std::stoul(m_sString, nullptr, base));
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(unsigned long& value, int base) const {
	try {
		value = std::stoul(m_sString, nullptr, base);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(unsigned long long& value, int base) const {
	try {
		value = std::stoull(m_sString, nullptr, base);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(float& value) const {
	try {
		value = std::stof(m_sString);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(double& value) const {
	try {
		value = std::stod(m_sString);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool String::parse(long double& value) const {
	try {
		value = std::stold(m_sString);
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}

template<typename ...T>
String String::Format(const String& format, T ...args) {
	return fmt::format(format.m_sString, args...);
}
String String::FromUTF16(const wchar_t* cstr) {
	const size_t n = wcslen(cstr);
	String str;
	str.reserve(n);
	wcstombs(str.m_sString.data(), cstr, n);
	return str;
}

const size_t String::NoFind = std::string::npos;

std::vector<String> String::split(char delim) const {
	String str = *this;
	std::vector<String> tokens;

	size_t idx;
	while ((idx = str.find(delim)) != NoFind) {
		tokens.push_back(str.substr(0, idx));
		str.erase(0, idx);
	}

	if (!str.is_empty()) {
		tokens.push_back(std::move(str));
	}
	return tokens;
}
std::vector<String> String::split(const String& delim) const {
	String str = *this;
	std::vector<String> tokens;

	size_t idx;
	while ((idx = str.find(delim)) != NoFind) {
		tokens.push_back(str.substr(0, idx));
		str.erase(0, idx + delim.length());
	}

	if (!str.is_empty()) {
		tokens.push_back(std::move(str));
	}
	return tokens;
}

std::ostream& operator<<(std::ostream& os, const String& string) {
	os << string.m_sString;
	return os;
}
std::istream& operator>>(std::istream& is, String& string) {
	is >> string.m_sString;
	return is;
}
