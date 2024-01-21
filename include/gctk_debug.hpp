#pragma once

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
		OpenGLRuntimeError,
	};
}