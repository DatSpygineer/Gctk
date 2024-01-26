#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fmt/format.h>

class String {
	std::string m_sString;
public:
	String(): m_sString() { }
	String(const char* cstr): m_sString(cstr) { }			// NOLINT: Implicit conversion for backwards compatibility
	String(const char* cstr, size_t len): m_sString(cstr, len) { }
	String(std::string&& str): m_sString(std::move(str)) { }	// NOLINT: Implicit conversion for backwards compatibility
	String(const std::string& str): m_sString(str) { }			// NOLINT: Implicit conversion for backwards compatibility
	String(String&& str) noexcept : m_sString(std::move(str.m_sString)) { str.m_sString = nullptr; }
	String(const String& str) = default;
	String(char c, size_t n);

	using traits_type    = std::string::traits_type;
	using allocator_type = std::string::allocator_type;
	using value_type      = std::string::value_type;
	using size_type       = std::string::size_type;
	using difference_type = std::string::difference_type;
	using pointer         = std::string::pointer;
	using const_pointer   = std::string::const_pointer;
	using reference       = std::string::reference&;
	using const_reference = std::string::const_reference&;
	using iterator       = std::string::iterator;
	using const_iterator = std::string::const_iterator;
	using reverse_iterator       = std::string::reverse_iterator;
	using const_reverse_iterator = std::string::const_reverse_iterator;

	String& operator= (String&& other) noexcept;
	String& operator= (const String& other) = default;

	[[nodiscard]] inline constexpr size_t length() const noexcept { return m_sString.length(); }
	[[nodiscard]] inline constexpr size_t size() const noexcept { return m_sString.size(); }
	[[nodiscard]] inline constexpr size_t capacity() const noexcept { return m_sString.capacity(); }
	[[nodiscard]] inline constexpr bool is_empty() const noexcept { return m_sString.empty(); }

	inline constexpr char* data() { return m_sString.data(); }
	[[nodiscard]] inline constexpr const char* data() const { return m_sString.data(); }
	[[nodiscard]] inline constexpr const char* c_str() const { return m_sString.c_str(); }

	inline std::string& std_str() { return m_sString; }
	[[nodiscard]] inline constexpr const std::string& std_str() const { return m_sString; }

	inline void reserve(size_t n) { m_sString.reserve(n); }

	[[nodiscard]] inline bool starts_with(char c) const { return m_sString.starts_with(c); }
	[[nodiscard]] inline bool starts_with(const String& str) const { return m_sString.starts_with(str.m_sString); }
	[[nodiscard]] inline bool ends_with(char c) const { return m_sString.ends_with(c); }
	[[nodiscard]] inline bool ends_with(const String& str) const { return m_sString.ends_with(str.m_sString); }

	[[nodiscard]] inline size_t find(char c, size_t offset = 0) const { return m_sString.find(c, offset); }
	[[nodiscard]] inline size_t find(const String& str, size_t offset = 0) const { return m_sString.find(str.m_sString, offset); }
	[[nodiscard]] inline size_t find_last(char c, size_t offset = 0) const { return m_sString.rfind(c, offset); }
	[[nodiscard]] inline size_t find_last(const String& str, size_t offset = 0) const { return m_sString.rfind(str.m_sString, offset); }
	[[nodiscard]] inline size_t find_first_of(const String& str, size_t offset = 0) const { return m_sString.find_first_of(str.m_sString, offset); }
	[[nodiscard]] inline size_t find_first_not_of(const String& str, size_t offset = 0) const { return m_sString.find_first_not_of(str.m_sString, offset); }
	[[nodiscard]] inline size_t find_last_of(const String& str, size_t offset = 0) const { return m_sString.find_last_of(str.m_sString, offset); }
	[[nodiscard]] inline size_t find_last_not_of(const String& str, size_t offset = 0) const { return m_sString.find_last_not_of(str.m_sString, offset); }

	[[nodiscard]] inline String substr(size_t start) const { return m_sString.substr(start); }
	[[nodiscard]] inline String substr(size_t start, size_t n) const { return m_sString.substr(start, n); }

	[[nodiscard]] String trim_start(const String& chars = " \t\n\r") const;
	[[nodiscard]] String trim_end(const String& chars = " \t\n\r") const;
	[[nodiscard]] String trim(const String& chars = " \t\n\r") const;

	[[nodiscard]] String to_lowercase() const;
	[[nodiscard]] String to_uppercase() const;

	inline String& append(char c) { m_sString.push_back(c); return *this; }
	inline String& append(const String& str) { m_sString += str.m_sString; return *this; }

	iterator insert(size_t at, char c);
	iterator insert(size_t at, const String& str);
	iterator insert(const iterator& at, char c);
	iterator insert(const iterator& at, const String& str);

	iterator erase(const iterator& at);
	iterator erase(const iterator& beg, const iterator& end);
	iterator erase(size_t at);
	iterator erase(size_t at, size_t n);

	String& replace(size_t at, size_t n, const String& replacement);
	String& replace(const iterator& beg, const iterator& end, const String& replacement);
	String& replace(const String& to_replace, const String& replacement);
	String& replace_all(const String& to_replace, const String& replacement);

	String& remove_prefix(const String& prefix);
	String& remove_suffix(const String& suffix);
	String& remove_prefix(char prefix);
	String& remove_suffix(char suffix);

	[[nodiscard]] String without_prefix(const String& prefix) const;
	[[nodiscard]] String without_suffix(const String& suffix) const;
	[[nodiscard]] String without_prefix(char prefix) const;
	[[nodiscard]] String without_suffix(char suffix) const;

	[[nodiscard]] inline String offset(size_t n) const { return m_sString.substr(n); }

	[[nodiscard]] std::vector<String> split(char delim) const;
	[[nodiscard]] std::vector<String> split(const String& delim) const;

	[[nodiscard]] int compare(const String& other) const;
	[[nodiscard]] int compare(const String& other, size_t n) const;
	[[nodiscard]] int compare(const String& other, size_t offset, size_t n) const;
	[[nodiscard]] int compare(const String& other, size_t this_offset, size_t offset, size_t n) const;
	[[nodiscard]] int compare_no_case(const String& other) const;
	[[nodiscard]] int compare_no_case(const String& other, size_t n) const;
	[[nodiscard]] int compare_no_case(const String& other, size_t offset, size_t n) const;
	[[nodiscard]] int compare_no_case(const String& other, size_t this_offset, size_t offset, size_t n) const;

	[[nodiscard]] bool parse(signed char& value, int base = 10) const;
	[[nodiscard]] bool parse(short& value, int base = 10) const;
	[[nodiscard]] bool parse(int& value, int base = 10) const;
	[[nodiscard]] bool parse(long& value, int base = 10) const;
	[[nodiscard]] bool parse(long long& value, int base = 10) const;
	[[nodiscard]] bool parse(unsigned char& value, int base = 10) const;
	[[nodiscard]] bool parse(unsigned short& value, int base = 10) const;
	[[nodiscard]] bool parse(unsigned int& value, int base = 10) const;
	[[nodiscard]] bool parse(unsigned long& value, int base = 10) const;
	[[nodiscard]] bool parse(unsigned long long& value, int base = 10) const;
	[[nodiscard]] bool parse(float& value) const;
	[[nodiscard]] bool parse(double& value) const;
	[[nodiscard]] bool parse(long double& value) const;

	template<typename ...T>
	inline static String Format(const String& format, T&&... args) {
		return { fmt::vformat(format.m_sString, fmt::make_format_args(args...)) };
	}
	static String FromUTF16(const wchar_t* cstr);
	static String FromChar(char value);
	static String FromChar(wchar_t value);

	static String From(uint8_t value, int base = 10);
	static String From(uint16_t value, int base = 10);
	static String From(uint32_t value, int base = 10);
	static String From(uint64_t value, int base = 10);
	static String From(int8_t value, int base = 10);
	static String From(int16_t value, int base = 10);
	static String From(int32_t value, int base = 10);
	static String From(int64_t value, int base = 10);
	static String From(float value);
	static String From(double value);
	static String From(long double value);
	static String From(bool value);

	[[nodiscard]] inline String operator+ (char c) const { return { m_sString + c }; }
	[[nodiscard]] inline String operator+ (const String& other) const { return { m_sString + other.m_sString }; }
	inline String operator+= (char c) { m_sString += c; return *this; }
	inline String operator+= (const String& other) { m_sString += other.m_sString; return *this; }

	[[nodiscard]] inline constexpr bool operator== (const String& other) const { return m_sString == other.m_sString; }
	[[nodiscard]] inline constexpr bool operator!= (const String& other) const { return m_sString != other.m_sString; }

	[[nodiscard]] inline constexpr char operator[] (size_t idx) const { return m_sString[idx]; }
	inline constexpr char& operator[] (size_t idx) { return m_sString[idx]; }

	std::string& operator* () { return m_sString; }
	const std::string& operator* () const { return m_sString; }
	std::string& operator-> () { return m_sString; }
	const std::string& operator-> () const { return m_sString; }

	inline auto begin() { return m_sString.begin(); }
	[[nodiscard]] inline auto begin() const { return m_sString.begin(); }
	inline auto rbegin() { return m_sString.rbegin(); }
	[[nodiscard]] inline auto rbegin() const { return m_sString.rbegin(); }
	inline auto end() { return m_sString.end(); }
	[[nodiscard]] inline auto end() const { return m_sString.end(); }
	inline auto rend() { return m_sString.rend(); }
	[[nodiscard]] inline auto rend() const { return m_sString.rend(); }

	static const size_t NoFind;

	friend std::ostream& operator<< (std::ostream& os, const String& string);
	friend std::istream& operator>> (std::istream& is, String& string);
};

inline String operator""_S(const char* cstr, size_t len);
template<>
struct fmt::formatter<String> : fmt::formatter<std::string_view> {
	auto format(String str, fmt::format_context& ctx) const {
		return fmt::formatter<std::string_view>::format(str.std_str(), ctx);
	}
};

class Exception : public std::exception {
	String m_sMessage;
public:
	explicit Exception(String&& message) noexcept: m_sMessage(std::move(message)) { }
	explicit Exception(const String& message) noexcept: m_sMessage(message) { }
	template<typename ...T>
	explicit Exception(const String& format, T... args) noexcept: m_sMessage({ fmt::vformat(format.std_str(), fmt::make_format_args(args...)) }) { }

	[[nodiscard]] virtual String message() const noexcept { return m_sMessage; }
	[[nodiscard]] const char* what() const noexcept override { return message().c_str(); }
};