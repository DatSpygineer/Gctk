#pragma once

#include <fstream>
#include <strlib/string.hpp>

namespace Gctk {
	enum class ErrorCode {
		None,
		RuntimeError,
		OutOfRangeError,
		NullPointerError,
		SDLInitError,
		OpenGLInitError,
		ParseError,
		FormatError,

		TextureLoadError,
		TextureHeaderMissing,
		TextureIdentifierError,
		TextureOpenGLError,

		MeshLoadError,
		MeshHeaderMissing,
		MeshIdentifierError,
		MeshOpenGLError,

		ShaderCompileError,
		ShaderLinkerError,
		ShaderUniformNotFound,

		OpenGLRuntimeError,

		CannotOpenFile
	};

	class EngineError : public Exception {
		ErrorCode m_eCode;
	public:
		EngineError(const String& message, ErrorCode error): Exception(message), m_eCode(error) { }

		[[nodiscard]] String message() const noexcept override;
		[[nodiscard]] inline constexpr ErrorCode code() const { return m_eCode; }
	};

	void BeginLogger();
	void EndLogger();

	void Log(const String& message);
	void LogWarning(const String& message);
	void LogError(const String& message, ErrorCode code);
}
template<>
struct fmt::formatter<Gctk::ErrorCode> : fmt::formatter<std::string_view> {
	auto format(const Gctk::ErrorCode& code, fmt::format_context & ctx) const {
		std::string_view name;

		switch (code) {
			case Gctk::ErrorCode::None:						name = "None"; break;
			case Gctk::ErrorCode::RuntimeError:				name = "RuntimeError"; break;
			case Gctk::ErrorCode::OutOfRangeError:			name = "OutOfRangeError"; break;
			case Gctk::ErrorCode::NullPointerError:			name = "NullPointerError"; break;
			case Gctk::ErrorCode::SDLInitError:				name = "SDLInitError"; break;
			case Gctk::ErrorCode::OpenGLInitError:			name = "OpenGLInitError"; break;
			case Gctk::ErrorCode::ParseError:				name = "ParseError"; break;
			case Gctk::ErrorCode::FormatError:				name = "FormatError"; break;
			case Gctk::ErrorCode::TextureLoadError:			name = "TextureLoadError"; break;
			case Gctk::ErrorCode::TextureHeaderMissing:		name = "TextureHeaderMissing"; break;
			case Gctk::ErrorCode::TextureIdentifierError:	name = "TextureIdentifierError"; break;
			case Gctk::ErrorCode::TextureOpenGLError:		name = "TextureOpenGLError"; break;
			case Gctk::ErrorCode::OpenGLRuntimeError:		name = "OpenGLRuntimeError"; break;
		}

		return fmt::formatter<std::string_view>::format(name, ctx);
	}
};