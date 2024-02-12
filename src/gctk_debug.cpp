#include "gctk_debug.hpp"
#include "strlib/date.hpp"

#include "gctk.hpp"

namespace Gctk {
	static std::ofstream LogFileStream;

	static Path GetLogFileName() {
		auto game = Game::Instance();
		auto root = game == nullptr ? Path::CurrentPath() : game->rootPath();
		return root / "logs" / String::Format("log_{}.txt", Date::Now().toString("yyyy_MM_dd_HH_mm_ss"));
	}

	String EngineError::message() const noexcept {
		return String::Format("{}: (Error code {})", Exception::message(), m_eCode);
	}

	void BeginLogger() {
		LogFileStream = std::ofstream(GetLogFileName().asString().stdStr());
	}
	void EndLogger() {
		if (LogFileStream.is_open()) {
			LogFileStream.close();
		}
	}

	static void LogCreateString(const char* label, const String& message, const DebugInfo& info, String& target) {
		size_t len = snprintf(nullptr, 0, "%s [ %s ] %s:%i => %s: %s",
				Date::Now().toString("YYYY/MM/dd HH:mm:ss").cStr(),
				label,
				info.callerfile, info.linenumber, info.callerfunction,
				message.cStr()
		);
		target.reserve(len);
		snprintf(target.data(), len + 1, "%s [ %s ] %s:%i => %s: %s",
			Date::Now().toString("YYYY/MM/dd HH:mm:ss").cStr(),
			label,
			info.callerfile, info.linenumber, info.callerfunction,
			message.cStr()
		);
	}

	void Log(const String& message, const DebugInfo& info) {
		String msg;
		LogCreateString("INFO", message, info, msg);
		printf("%s\n", msg.cStr());
#ifndef NDEBUG
		if (LogFileStream.is_open()) LogFileStream << msg << "\n";
#endif
	}

	void LogWarning(const String& message, const DebugInfo& info) {
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO prevInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &prevInfo);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
#else
		printf("\x1B[33m");
#endif
		String msg;
		LogCreateString("WARN", message, info, msg);
		printf("%s\n", msg.cStr());
#ifndef NDEBUG
		if (LogFileStream.is_open()) LogFileStream << msg << "\n";
#endif
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), prevInfo.wAttributes);
#else
		printf("\x1B[0m");
#endif
	}

	void LogError(const String& message, ErrorCode code, const DebugInfo& info) {
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO prevInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &prevInfo);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
#else
		printf("\x1B[31m");
#endif
		String msg;
		LogCreateString("ERR", message, info, msg);
		printf("%s\n", msg.cStr());
#ifndef NDEBUG
		if (LogFileStream.is_open()) LogFileStream << msg << "\n";
#endif
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), prevInfo.wAttributes);
#else
		printf("\x1B[0m");
#endif
	}
}