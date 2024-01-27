#pragma once

#include <GL/glew.h>

#include "gctk_common.hpp"
#include "gctk_texture.hpp"
#include "math/gctk_math.hpp"
#include "strlib/string.hpp"

namespace Gctk {
	class Shader {
		GLuint m_uId;
	public:
		explicit Shader(GLuint id): m_uId(id) { }
		Shader(Shader&& shader) noexcept: m_uId(shader.m_uId) { shader.m_uId = 0; }
		Shader(const Shader& shader) = default;
		~Shader();

		inline Shader& operator= (Shader&& shader) noexcept {
			m_uId = shader.m_uId;
			shader.m_uId = 0;
			return *this;
		}
		Shader& operator= (const Shader& shader) = default;

		[[nodiscard]] inline constexpr bool operator== (const Shader& shader) const { return m_uId == shader.m_uId; }
		[[nodiscard]] inline constexpr bool operator!= (const Shader& shader) const { return m_uId != shader.m_uId; }

		static Result<Shader, ErrorCode> Load(const String& vertex, const String& fragment);

		[[nodiscard]] bool setUniform(const String& name, GLint value) const;
		[[nodiscard]] bool setUniform(const String& name, GLuint value) const;
		[[nodiscard]] bool setUniform(const String& name, Float value) const;
		[[nodiscard]] bool setUniform(const String& name, const Vec2I& value) const;
		[[nodiscard]] bool setUniform(const String& name, const Vec2& value) const;
		[[nodiscard]] bool setUniform(const String& name, const Vec3& value) const;
		[[nodiscard]] bool setUniform(const String& name, const Vec4& value) const;
		[[nodiscard]] bool setUniform(const String& name, const Quat& value) const;
		[[nodiscard]] bool setUniform(const String& name, const Color& value) const;
		[[nodiscard]] bool setUniform(const String& name, const Mat4& value) const;
		[[nodiscard]] bool setUniform(const String& name, const Texture& value) const;

		bool getUniform(const String& name, GLint& value) const;
		bool getUniform(const String& name, GLuint& value) const;
		bool getUniform(const String& name, Float& value) const;
		bool getUniform(const String& name, Vec2I& value) const;
		bool getUniform(const String& name, Vec2& value) const;
		bool getUniform(const String& name, Vec3& value) const;
		bool getUniform(const String& name, Vec4& value) const;
		bool getUniform(const String& name, Quat& value) const;
		bool getUniform(const String& name, Color& value) const;
		bool getUniform(const String& name, Mat4& value) const;

		void apply() const;

		static const Shader Null;
	};
}