#include "io.hpp"

#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

#ifndef _WIN32
	#define STRLIB_PATH_SEP '/'
#else
	#define STRLIB_PATH_SEP '\\'
#endif

inline constexpr bool EndsWithPathSeparator(const String& str) {
	return str.startsWith('/')
#ifdef _WIN32
	|| str.startsWith('\\')
#endif
	;
}

Path& Path::append(const String& str) {
	if (!EndsWithPathSeparator(str) && !str.isEmpty()) {
		m_sInternalString += STRLIB_PATH_SEP;
	}
	m_sInternalString += str;
	return *this;
}
Path& Path::append(const Path& path) {
	if (!EndsWithPathSeparator(path.m_sInternalString) && !path.m_sInternalString.isEmpty()) {
		m_sInternalString += STRLIB_PATH_SEP;
	}
	m_sInternalString += path.m_sInternalString;
	return *this;
}
Path& Path::append(String&& str) {
	if (!EndsWithPathSeparator(str) && !str.isEmpty()) {
		m_sInternalString += STRLIB_PATH_SEP;
	}
	m_sInternalString += str;
	return *this;
}
Path& Path::append(Path&& path) {
	if (!EndsWithPathSeparator(path.m_sInternalString) && !path.m_sInternalString.isEmpty()) {
		m_sInternalString += STRLIB_PATH_SEP;
	}
	m_sInternalString += path.m_sInternalString;
	return *this;
}

bool Path::exists() const {
	return fs::exists(m_sInternalString.stdStr());
}

String Path::name(bool containExtension) const {
	return fs::path(m_sInternalString.stdStr()).filename().string();
}
String Path::extension() const {
	return fs::path(m_sInternalString.stdStr()).extension().string();
}

Path Path::parent() const {
	return {String { fs::path(m_sInternalString.stdStr()).parent_path().string() } };
}

Path Path::toAbsolute() const {
	return { String { fs::absolute(m_sInternalString.stdStr()).string() } };
}
constexpr bool Path::isEmpty() const {
	return m_sInternalString.isEmpty();
}
bool Path::isAbsolute() const {
	return fs::path(m_sInternalString.stdStr()).is_absolute();
}
bool Path::isRelative() const {
	return !fs::path(m_sInternalString.stdStr()).is_absolute();
}

bool Path::isFile() const {
	return fs::is_regular_file(fs::path(m_sInternalString.stdStr()));
}
bool Path::isDirectory() const {
	return fs::is_directory(fs::path(m_sInternalString.stdStr()));
}
bool Path::isSymLink() const {
	return fs::is_symlink(fs::path(m_sInternalString.stdStr()));
}
bool Path::isSocket() const {
	return fs::is_socket(fs::path(m_sInternalString.stdStr()));
}
bool Path::isFIFO() const {
	return fs::is_fifo(fs::path(m_sInternalString.stdStr()));
}
bool Path::isBlockFile() const {
	return fs::is_block_file(fs::path(m_sInternalString.stdStr()));
}
bool Path::isCharacterFile() const {
	return fs::is_character_file(fs::path(m_sInternalString.stdStr()));
}

inline std::regex CreateRegexFromWildcard(const String& wildcard) {
	std::string regexPattern = wildcard.stdStr();
	regexPattern = std::regex_replace(regexPattern, std::regex("\\\\"), "\\\\");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\^"), "\\^");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\."), "\\.");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\$"), "\\$");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\|"), "\\|");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\("), "\\(");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\)"), "\\)");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\{"), "\\{");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\{"), "\\}");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\["), "\\[");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\]"), "\\]");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\+"), "\\+");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\/"), "\\/");

	regexPattern = std::regex_replace(regexPattern, std::regex("\\?"), ".");
	regexPattern = std::regex_replace(regexPattern, std::regex("\\*"), ".*");

	return std::regex(regexPattern, std::regex_constants::ECMAScript
#ifdef _WIN32
		| std::regex_constants::icase
#endif
	);
}

bool Path::matches(const String& pattern) const {
	return std::regex_match(m_sInternalString.stdStr(), CreateRegexFromWildcard(pattern));
}

Path Path::Join(const std::initializer_list<Path>& paths) {
	Path result;
	for (const auto& path : paths) {
		result.append(path);
	}
	return result;
}

Path Path::operator/ (Path&& other) const {
	Path result = *this;
	result.append(std::move(other));
	return result;
}
Path Path::operator/ (const Path& other) const {
	Path result = *this;
	result.append(other);
	return result;
}
Path Path::operator/ (String&& other) const {
	Path result = *this;
	result.append(std::move(other));
	return result;
}
Path Path::operator/ (const String& other) const {
	Path result = *this;
	result.append(other);
	return result;
}

Path& Path::operator/=(const Path& other) {
	return append(other);
}
Path& Path::operator/=(Path&& other) {
	return append(std::move(other));
}
Path& Path::operator/=(const String& other) {
	return append(other);
}
Path& Path::operator/=(String&& other) {
	return append(std::move(other));
}

Path Path::operator+ (Path&& other) const {
	return { m_sInternalString + std::move(other.m_sInternalString) };
}
Path Path::operator+ (const Path& other) const {
	return { m_sInternalString + other.m_sInternalString };
}
Path Path::operator+ (String&& other) const {
	return { m_sInternalString + std::move(other) };
}
Path Path::operator+ (const String& other) const {
	return { m_sInternalString + other };
}

Path& Path::operator+= (const Path& other) {
	m_sInternalString += other.m_sInternalString;
	return *this;
}
Path& Path::operator+= (Path&& other) {
	m_sInternalString += std::move(other.m_sInternalString);
	return *this;
}
Path& Path::operator+= (const String& other) {
	m_sInternalString += other;
	return *this;
}
Path& Path::operator+= (String&& other) {
	m_sInternalString += std::move(other);
	return *this;
}

bool Path::hasExtension() const {
	return fs::path(m_sInternalString.stdStr()).has_extension();
}

std::optional<File> Path::openFile(FileMode mode) const {
	File f { m_sInternalString, mode };
	if (f.isOpen()) {
		return std::make_optional(f);
	}
	return std::nullopt;
}
std::optional<Directory> Path::createDirectory(bool recursive) const {
	Directory dir { m_sInternalString };
	if (dir.create(recursive)) {
		return std::make_optional(dir);
	}
	return std::nullopt;
}

bool Path::remove(bool recursive) const {
	return recursive ? fs::remove_all(m_sInternalString.stdStr()) : fs::remove(m_sInternalString.stdStr());
}

bool Path::move(const Path& dest) const {
	try {
		fs::rename(m_sInternalString.stdStr(), dest.m_sInternalString.stdStr());
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}
bool Path::copy(const Path& dest) const {
	try {
		fs::copy(m_sInternalString.stdStr(), dest.m_sInternalString.stdStr());
	} catch (const std::exception& _) {
		return false;
	}
	return true;
}

Path Path::CurrentPath() {
	return { String { fs::current_path().string() } };
}

File::File(const Path& path, FileMode mode) {
	const char* mode_str;
	switch (mode) {
		case FileMode::Read: mode_str = "r"; break;
		case FileMode::ReadOrCreate: mode_str = "r+"; break;
		case FileMode::Write: mode_str = "w"; break;
		case FileMode::Append: mode_str = "a"; break;
		case FileMode::ReadWrite: mode_str = "w+"; break;
		case FileMode::ReadAppend: mode_str = "a+"; break;
	}
	m_pFile = fopen(path.asString().cStr(), mode_str);
}
File::~File() {
	if (m_pFile != nullptr) {
		fclose(m_pFile);
	}
}

char File::read() const {
	return static_cast<char>(fgetc(m_pFile));
}
String File::readToEnd() const {
	String str;
	str.reserve(size());
	fread(str.data(), str.capacity(), 1, m_pFile);
	return str;
}

size_t File::readBytes(std::vector<uint8_t>& data, size_t n) const {
	if (n <= 0) {
		n = size();
	}
	if (data.capacity() < n) {
		data.reserve(n);
	}

	return fread(data.data(), n, 1, m_pFile);
}

template<typename T>
bool File::read(T& result) const {
	return fread(&result, sizeof(T), 1, m_pFile) > 0;
}
template<typename T>
bool File::read(std::vector<T>& result, size_t count) const {
	if (count <= 0) {
		count = size() / sizeof(T);
	}
	if (result.capacity() < count) {
		result.reserve(count);
	}

	return fread(result.data(), sizeof(T), count, m_pFile) == count;
}


void File::write(char c) const {
	fputc(c, m_pFile);
}
void File::write(const String& str) const {
	fputs(str.cStr(), m_pFile);
}
void File::writeLine(const String& str) const {
	fprintf(m_pFile, "%s\n", str.cStr());
}
void File::writeBytes(const std::vector<uint8_t>& data) const {
	fwrite(data.data(), sizeof(uint8_t), data.size(), m_pFile);
}

size_t File::size() const {
	size_t pos = position();
	if (!seek(FileSeekOrigin::End, 0)) return 0;
	size_t size = position();
	if (!seek(FileSeekOrigin::Begin, static_cast<int>(pos))) return 0;
	return size;
}

template<typename T>
bool File::write(const T& result) {
	return fwrite(&result, sizeof(T), 1, m_pFile) > 0;
}
template<typename T>
bool File::write(const std::vector<T>& result) {
	return fwrite(result.data(), sizeof(T), result.size()) == result.size();
}

std::vector<Path> Directory::files(const String& pattern) const {
	std::vector<Path> result;

	if (!exists()) {
		return result;
	}

	auto dirIt = fs::directory_iterator(m_path.asString().stdStr());
	result.reserve(std::distance(dirIt, fs::directory_iterator { }));

	for (const auto& path : dirIt) {
		Path p = { path.path().string() };
		if (p.matches(pattern)) {
			result.emplace_back(p);
		}
	}
	return result;
}

bool Directory::exists() const {
	return m_path.exists();
}
bool Directory::create(bool recursive) const {
	return recursive ?	fs::create_directories(m_path.asString().stdStr()) :
						fs::create_directory(m_path.asString().stdStr());
}
bool Directory::remove(bool recursive) const {
	return recursive ? fs::remove_all(m_path.asString().stdStr()) : fs::remove(m_path.asString().stdStr());
}
