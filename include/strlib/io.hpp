#pragma once

#include <optional>
#include "string.hpp"

template<typename ...T>
inline void Print(const String& message, T&&... args) {
	printf("%s", String::Format(message, fmt::make_format_args(args...)).cStr());
}
template<typename ...T>
inline void Println(const String& message, T&&... args) {
	puts(String::Format(message, fmt::make_format_args(args...)).cStr());
}

enum class FileMode : uint8_t {
	Read,
	ReadOrCreate,
	Write,
	Append,
	ReadWrite,
	ReadAppend
};

enum class FileSeekOrigin : uint8_t {
	Begin = SEEK_SET,
	Current = SEEK_CUR,
	End = SEEK_END
};

class File;
class Directory;

class Path {
	String m_sInternalString;
public:
	Path(): m_sInternalString() { }
	Path(const String& str): m_sInternalString(str) { } // NOLINT: Implicit conversion intended
	Path(Path&& path) noexcept: m_sInternalString(std::move(path.m_sInternalString)) { path.m_sInternalString = ""; }
	Path(const Path& path) = default;

	Path& operator= (const Path& path) = default;
	inline Path& operator= (Path&& path) noexcept {
		m_sInternalString = std::move(path.m_sInternalString);
		path.m_sInternalString = "";
		return *this;
	}

	Path& append(const String& str);
	Path& append(const Path& path);
	Path& append(String&& str);
	Path& append(Path&& path);

	[[nodiscard]] bool exists() const;
	[[nodiscard]] String name(bool containExtension = false) const;
	[[nodiscard]] String extension() const;
	[[nodiscard]] Path parent() const;
	[[nodiscard]] Path toAbsolute() const;
	[[nodiscard]] constexpr bool isEmpty() const;
	[[nodiscard]] bool hasExtension() const;

	[[nodiscard]] bool isAbsolute() const;
	[[nodiscard]] bool isRelative() const;

	[[nodiscard]] bool isFile() const;
	[[nodiscard]] bool isDirectory() const;
	[[nodiscard]] bool isSymLink() const;
	[[nodiscard]] bool isSocket() const;
	[[nodiscard]] bool isFIFO() const;
	[[nodiscard]] bool isBlockFile() const;
	[[nodiscard]] bool isCharacterFile() const;

	[[nodiscard]] bool matches(const String& pattern) const;

	[[nodiscard]] std::optional<File> openFile(FileMode mode) const;
	[[nodiscard]] std::optional<Directory> createDirectory(bool recursive) const;
	[[nodiscard]] bool remove(bool recursive) const;
	[[nodiscard]] bool move(const Path& dest) const;
	[[nodiscard]] inline constexpr bool rename(const Path& dest) const { return move(dest); }
	[[nodiscard]] bool copy(const Path& dest) const;

	[[nodiscard]] inline String& asString() { return m_sInternalString; }
	[[nodiscard]] inline const String& asString() const { return m_sInternalString; }

	static Path Join(const std::initializer_list<Path>& paths);

	Path operator/ (Path&& other) const;
	Path operator/ (const Path& other) const;
	Path operator/ (String&& other) const;
	Path operator/ (const String& other) const;
	Path& operator/= (Path&& other);
	Path& operator/= (const Path& other);
	Path& operator/= (String&& other);
	Path& operator/= (const String& other);
	Path operator+ (Path&& other) const;
	Path operator+ (const Path& other) const;
	Path operator+ (String&& other) const;
	Path operator+ (const String& other) const;
	Path& operator+= (Path&& other);
	Path& operator+= (const Path& other);
	Path& operator+= (String&& other);
	Path& operator+= (const String& other);

	inline constexpr bool operator== (const Path& path) const { return m_sInternalString == path.m_sInternalString; }
	inline constexpr bool operator!= (const Path& path) const { return m_sInternalString == path.m_sInternalString; }

	[[nodiscard]] inline explicit operator const String&() const { return m_sInternalString; }

	static Path CurrentPath();
};

class File {
	FILE* m_pFile;
public:
	File(const Path& path, FileMode mode);
	~File();

	[[nodiscard]] char read() const;
	[[nodiscard]] String readToEnd() const;
	[[nodiscard]] size_t readBytes(std::vector<uint8_t>& data, size_t n) const;

	template<typename T>
	[[nodiscard]] bool read(T& result) const;
	template<typename T>
	[[nodiscard]] bool read(std::vector<T>& result, size_t count) const;

	void write(char c) const;
	void write(const String& str) const;
	void writeLine(const String& str) const;
	void writeBytes(const std::vector<uint8_t>& data) const;

	template<typename T>
	[[nodiscard]] bool write(const T& result);
	template<typename T>
	[[nodiscard]] bool write(const std::vector<T>& result);

	[[nodiscard]] size_t size() const;

	[[nodiscard]] inline constexpr bool seek(FileSeekOrigin origin, int offset) const {
		return fseek(m_pFile, static_cast<long>(origin), offset) >= 0;
	}
	[[nodiscard]] inline constexpr size_t tell() const { return ftell(m_pFile); }
	[[nodiscard]] inline constexpr size_t position() const { return ftell(m_pFile); }

	inline constexpr void flush() const { fflush(m_pFile); }
	[[nodiscard]] inline constexpr bool eof() const { return feof(m_pFile) != 0; }

	[[nodiscard]] inline constexpr bool isOpen() const { return m_pFile != nullptr; }
};

class Directory {
	Path m_path;
public:
	explicit Directory(Path&& path) noexcept: m_path(std::move(path)) { }
	explicit Directory(const Path& path): m_path(path) { }
	Directory(Directory&& directory) noexcept = default;
	Directory(const Directory& directory) = default;

	[[nodiscard]] bool exists() const;
	[[nodiscard]] bool create(bool recursive) const;
	[[nodiscard]] bool remove(bool recursive) const;

	[[nodiscard]] std::vector<Path> files(const String& pattern = "*.*") const;
};