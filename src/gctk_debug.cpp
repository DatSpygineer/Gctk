#include "gctk_debug.hpp"

namespace Gctk {
	static std::ofstream LogFileStream;

	String EngineError::message() const noexcept {
		return String::Format("{}: (Error code {})", Exception::message(), m_eCode);
	}

	void BeginLogger() {

	}
	void EndLogger() {

	}

	void Log(const String& message) {

	}
	void LogWarning(const String& message) {

	}
	void LogError(const String& message, ErrorCode code) {

	}
}