#include "gctk.hpp"

#include <iostream>

namespace Gctk {
	Result<Version, ErrorCode> Version::FromString(const String& str) {
		VersionCycle lifeCycle = VersionCycle::Release;

		if (str.endsWith("PA") || str.endsWith(" PA") || str.endsWith("Pre-Alpha") || str.endsWith(" Pre-Alpha")) {
			lifeCycle = VersionCycle::PreAlpha;
		} else if (str.endsWith("A") || str.endsWith(" A") || str.endsWith("Alpha") || str.endsWith(" Alpha")) {
			lifeCycle = VersionCycle::Alpha;
		} else if (str.endsWith("B") || str.endsWith(" B") || str.endsWith("Beta") || str.endsWith(" Beta")) {
			lifeCycle = VersionCycle::Beta;
		} else if (str.endsWith("RC") || str.endsWith(" RC") || str.endsWith("Release-Candidate") ||
				   str.endsWith(" Release-Candidate")) {
			lifeCycle = VersionCycle::ReleaseCandidate;
		} else if (str.endsWith("PR") || str.endsWith(" PR") || str.endsWith("Pre-Release") ||
				   str.endsWith(" Pre-Release")) {
			lifeCycle = VersionCycle::PreRelease;
		}

		std::vector<String> tokens;
		if (str.startsWith("v")) {
			tokens = str.substr(1).trim().split('.');
		} else {
			tokens = str.trim().split('.');
		}

		if (tokens.size() == 2) {
			uint8_t major, minor;
			if (!tokens.at(0).parse(major) || !tokens.at(1).parse(minor)) {
				return { ErrorCode::ParseError };
			}
			return { { major, minor } };
		} else if (tokens.size() >= 3) {
			uint8_t major, minor, patch;
			if (!tokens.at(0).parse(major) || !tokens.at(1).parse(minor) || !tokens.at(2).parse(patch)) {
				return { ErrorCode::ParseError };
			}
			return { { major, minor, patch } };
		}
		return { ErrorCode::ParseError };
	}

	String Version::to_string() const {
		String result = patch > 0 ? String::Format("{}.{}.{}", major, minor, patch) : String::Format("{}.{}", major, minor);
		if (cycle != VersionCycle::Release) {
			result += String::Format(" {}", cycle);
		}
		return result;
	}

	const Version Version::EngineVersion = { 1, 0, 0, VersionCycle::PreAlpha };

	Window::Window(const String& author, const String& name, size_t width, size_t height): m_sEvent({ 0 }) {
		if (SDL_WasInit(0) == 0 && SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			throw std::runtime_error(fmt::format("Failed to initialize SDL2: \"{}\"", SDL_GetError()));
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		m_pWindow = SDL_CreateWindow(name.cStr(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
		if (m_pWindow != nullptr) {
			throw std::runtime_error(fmt::format("Failed to create window: \"{}\"", SDL_GetError()));
		}

		m_pContext = SDL_GL_CreateContext(m_pWindow);
		if (m_pContext != nullptr) {
			throw std::runtime_error(fmt::format("Failed to create OpenGL context: \"{}\"", SDL_GetError()));
		}

		SDL_GL_MakeCurrent(m_pWindow, m_pContext);

		if (GLenum glewError = glewInit(); glewError != GLEW_OK) {
			const char* errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewError));
			throw std::runtime_error(fmt::format("Failed to initialize OpenGL: \"{}\" (code: {})", errorMsg, glewError));
		}

		m_fTimeLast = static_cast<double>(SDL_GetTicks64()) / 1000.0;
		onInit();
	}
	Window::~Window() {
		if (m_pContext) {
			SDL_GL_DeleteContext(m_pContext);
		}
		if (m_pWindow) {
			SDL_DestroyWindow(m_pWindow);
		}

		if (SDL_WasInit(0) != 0) {
			SDL_Quit();
		}
	}

	bool Window::update() {
		while (SDL_PollEvent(&m_sEvent) != 0) {
			switch (m_sEvent.type) {
				case SDL_QUIT: {
					onQuit();
					return false;
				}
				case SDL_WINDOWEVENT: {
					switch (m_sEvent.window.type) {
						case SDL_WINDOWEVENT_MOVED: {
							onMoved();
						} break;
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						case SDL_WINDOWEVENT_RESIZED: {
							onResized();
						} break;
					}
				} break;
			}
		}

		const double time = static_cast<double>(SDL_GetTicks64()) / 1000.0;
		const double delta = std::abs(time - m_fTimeLast);

		onUpdate(delta);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		onRender(delta);
		SDL_GL_SwapWindow(m_pWindow);

		m_fTimeLast = time;
		return true;
	}

	Vec2I Window::position() const {
		int x, y;
		SDL_GetWindowPosition(m_pWindow, &x, &y);
		return { x, y };
	}
	void Window::position(const Vec2I& pos) {
		SDL_SetWindowPosition(m_pWindow, pos.x, pos.y);
	}

	Vec2I Window::size() const {
		int w, h;
		SDL_GetWindowSize(m_pWindow, &w, &h);
		return {w, h };
	}
	void Window::size(const Vec2I& pos) {
		SDL_SetWindowSize(m_pWindow, pos.x, pos.y);
	}

	String Window::title() const {
#ifndef NDEBUG
		String title = SDL_GetWindowTitle(m_pWindow);
		if (size_t idx = title.find('|'); idx != String::NoFind) {
			return title.substr(0, idx);
		}
		return title;
#else
		return SDL_GetWindowTitle(m_pWindow);
#endif
	}
	void Window::title(const String& title) {
#ifndef NDEBUG
		SDL_SetWindowTitle(m_pWindow, debug_title(title).cStr());
#else
		SDL_SetWindowTitle(m_pWindow, title.c_str());
#endif
	}

	String Window::debug_title(const String& title) {
		return String::Format("{} | GCTK DEBUG v{} | OpenGL v{}", title, Version::EngineVersion, reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}
}
