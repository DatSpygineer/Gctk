#pragma once

#include <string>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "fmt/format.h"

#include "gctk_common.hpp"
#include "strlib/string.hpp"
#include "math/gctk_math.hpp"
#include "gctk_debug.hpp"

namespace Gctk {
	enum class VersionCycle : uint8_t {
		Release,
		PreAlpha,
		Alpha,
		Beta,
		PreRelease,
		ReleaseCandidate
	};

	struct Version {
		uint8_t major, minor, patch;
		VersionCycle cycle;

		Version(): major(0), minor(0), patch(0), cycle(VersionCycle::Release) { }
		Version(uint8_t major, uint8_t minor, uint8_t patch = 0, VersionCycle cycle = VersionCycle::Release):
			major(major), minor(minor), patch(patch), cycle(cycle) { }
		explicit Version(uint32_t version):
			cycle(static_cast<VersionCycle>(version & 0xFF)),
			patch((version >> 8) & 0xFF), minor((version >> 16) & 0xFF), major((version >> 24) & 0xFF) { }

		static Result<Version, ErrorCode> FromString(const String& str);

		[[nodiscard]] String to_string() const;

		static const Version EngineVersion;
	};

	class Window {
		SDL_Window* m_pWindow;
		SDL_Event m_sEvent;
		void* m_pContext;
		String m_sAuthor, m_sName;
		double m_fTimeLast;
	public:
		Window(const String& author, const String& name, size_t width, size_t height);
		~Window();

		virtual void onInit() { }
		virtual void onUpdate(double delta) { }
		virtual void onRender(double delta) { }
		virtual void onQuit() { }
		virtual void onResized() { }
		virtual void onMoved() { }

		bool update();

		[[nodiscard]] Vec2I position() const;
		void position(const Vec2I& pos);
		[[nodiscard]] Vec2I size() const;
		void size(const Vec2I& pos);
		[[nodiscard]] String title() const;
		void title(const String& title);

		[[nodiscard]] String author() const { return m_sAuthor; }
		[[nodiscard]] String name() const { return m_sName; }
	private:
		[[nodiscard]] static String debug_title(const String& title);
	};
}

template<>
struct fmt::formatter<Gctk::VersionCycle> : fmt::formatter<std::string_view> {
	auto format(Gctk::VersionCycle cycle, fmt::format_context& ctx) const {
		std::string_view name = "Invalid";
		switch (cycle) {
			case Gctk::VersionCycle::Release: name = "Release"; break;
			case Gctk::VersionCycle::PreAlpha: name = "PreAlpha"; break;
			case Gctk::VersionCycle::Alpha: name = "Alpha"; break;
			case Gctk::VersionCycle::Beta: name = "Beta"; break;
			case Gctk::VersionCycle::PreRelease: name = "PreRelease"; break;
			case Gctk::VersionCycle::ReleaseCandidate: name = "ReleaseCandidate"; break;
		}
		return fmt::formatter<std::string_view>::format(name, ctx);
	}
};
template<>
struct fmt::formatter<Gctk::Version> : fmt::formatter<std::string_view> {
	auto format(Gctk::Version cycle, fmt::format_context& ctx) const {
		return fmt::formatter<std::string_view>::format(cycle.to_string().std_str(), ctx);
	}
};