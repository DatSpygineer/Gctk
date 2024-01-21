#pragma once

#include <cstdint>
#include <GL/glew.h>

#include "gctk_gtex.hpp"
#include "gctk_math.hpp"
#include "gctk_debug.hpp"
#include "gctk_common.hpp"

namespace Gctk {
	class Texture {
		GLuint m_uId;
		Vec2I m_vecSize;
		uint16_t m_uDepth;
		GLuint m_uTarget;

	public:
		Texture(GLuint id, const Vec2I& size, uint16_t depth = 0, GLuint target = GL_TEXTURE_2D):
				m_uId(id), m_vecSize(size), m_uDepth(depth), m_uTarget(target) { }
		Texture(const GTex& texture_data); // NOLINT: Implicit conversion is intended.
		~Texture();

		static Result<Texture, ErrorCode> Load(const std::vector<uint8_t>& data);
		static Result<Texture, ErrorCode> Load(const std::vector<uint8_t>& data, const GTexHeader& header);
		static Result<Texture, ErrorCode> Load(const std::string& path);
		static Result<Texture, ErrorCode> Load(const std::string& path, const GTexHeader& header);
		static Result<Texture, ErrorCode> Load(const GTex& texture_data);

		[[nodiscard]] constexpr GLuint id() const { return m_uId; }

		void bind() const;

		Vec2I size() const;
		uint16_t width() const;
		uint16_t height() const;
		uint16_t depth() const;
		GLuint target() const;
	};
}