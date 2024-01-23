#pragma once

#include <cstdint>
#include <GL/glew.h>

#include "gtex/gctk_gtex.hpp"
#include "math/gctk_math.hpp"
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
		Texture(): Texture(0, { 0, 0 }) { }
		~Texture();

		static Result<Texture, ErrorCode> Load(const std::vector<uint8_t>& data);
		static Result<Texture, ErrorCode> Load(const std::vector<uint8_t>& data, const GTexHeader& header);
		static Result<Texture, ErrorCode> Load(const std::string& path);
		static Result<Texture, ErrorCode> Load(const std::string& path, const GTexHeader& header);
		static Result<Texture, ErrorCode> Load(const GTex& texture_data);

		[[nodiscard]] constexpr GLuint id() const { return m_uId; }

		void bind() const;

		[[nodiscard]] inline Vec2I size() const { return m_vecSize; }
		[[nodiscard]] inline uint16_t width() const { return m_vecSize.width; }
		[[nodiscard]] inline uint16_t height() const { return m_vecSize.height; }
		[[nodiscard]] inline uint16_t depth() const { return m_uDepth; }
		[[nodiscard]] inline GLuint target() const { return m_uTarget; }

		static const Texture Null;
	};
}