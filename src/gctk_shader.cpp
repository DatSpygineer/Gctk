#include "gctk_shader.hpp"

#include "gctk_debug.hpp"

#if defined(GCTK_USE_DOUBLE_FLOAT) && GCTK_USE_DOUBLE_FLOAT != 0
	#define glUniform1F glUniform1d
	#define glUniform1F glUniform2d
	#define glUniform3F glUniform3d
	#define glUniform4F glUniform4d
	#define glUniformMatrix4Fv glUniformMatrix4dv
	#define glGetUniformFv glGetUniformdv
#else
	#define glUniform1F glUniform1f
	#define glUniform2F glUniform2f
	#define glUniform3F glUniform3f
	#define glUniform4F glUniform4f
	#define glUniformMatrix4Fv glUniformMatrix4fv
	#define glGetUniformFv glGetUniformfv
#endif

namespace Gctk {
	Shader::~Shader() {
		if (m_uId != 0) {
			glDeleteProgram(m_uId);
			m_uId = 0;
		}
	}

	Result<Shader, ErrorCode> Shader::Load(const String& vertex, const String& fragment) {
		char info_log[128] = { 0 };
		GLint status = 0;
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		const char* src = vertex.c_str();
		glShaderSource(vert, 1, &src, nullptr);
		glCompileShader(vert);
		glGetShaderiv(vert, GL_COMPILE_STATUS, &status);

		if (status == 0) {
			glDeleteShader(vert);
			glGetShaderInfoLog(vert, 128, nullptr, info_log);
			LogError(String::Format("Failed to compile vertex shader: {}", info_log), ErrorCode::ShaderCompileError);
			return { ErrorCode::ShaderCompileError };
		}

		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
		src = fragment.c_str();
		glShaderSource(frag, 1, &src, nullptr);
		glCompileShader(frag);
		glGetShaderiv(frag, GL_COMPILE_STATUS, &status);

		if (status == 0) {
			glDeleteShader(vert);
			glDeleteShader(frag);
			glGetShaderInfoLog(frag, 128, nullptr, info_log);
			LogError(String::Format("Failed to compile fragment shader: {}", info_log), ErrorCode::ShaderCompileError);
			return { ErrorCode::ShaderCompileError };
		}

		GLuint id = glCreateProgram();
		glAttachShader(id, vert);
		glAttachShader(id, frag);
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &status);

		glDetachShader(id, vert);
		glDetachShader(id, frag);
		glDeleteShader(vert);
		glDeleteShader(frag);

		if (status == 0) {
			glGetProgramInfoLog(id, 128, nullptr, info_log);
			LogError(String::Format("Failed to link shader: {}", info_log), ErrorCode::ShaderLinkerError);
			return { ErrorCode::ShaderLinkerError };
		}

		return { Shader { id } };
	}

	bool Shader::setUniform(const String& name, GLint value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform1i(location, value);

		return true;
	}
	bool Shader::setUniform(const String& name, GLuint value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform1ui(location, value);

		return true;
	}
	bool Shader::setUniform(const String& name, Float value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform1F(location, value);

		return true;
	}
	bool Shader::setUniform(const String& name, const Vec2I& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform2i(location, value.x, value.y);

		return true;
	}
	bool Shader::setUniform(const String& name, const Vec2& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform2F(location, value.x, value.y);

		return true;
	}
	bool Shader::setUniform(const String& name, const Vec3& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform3F(location, value.x, value.y, value.z);

		return true;
	}
	bool Shader::setUniform(const String& name, const Vec4& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform4F(location, value.x, value.y, value.z, value.w);

		return true;
	}
	bool Shader::setUniform(const String& name, const Quat& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform4F(location, value.x, value.y, value.z, value.w);

		return true;
	}
	bool Shader::setUniform(const String& name, const Color& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform4F(location, value.r, value.g, value.b, value.a);

		return true;
	}
	bool Shader::setUniform(const String& name, const Mat4& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniformMatrix4Fv(location, 16, GL_FALSE, value.asRawPtr());

		return true;
	}
	bool Shader::setUniform(const String& name, const Texture& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glUniform1ui(location, value.id());

		return true;
	}
	bool Shader::getUniform(const String& name, GLint& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glGetUniformiv(m_uId, location, &value);

		return true;
	}
	bool Shader::getUniform(const String& name, GLuint& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glGetUniformuiv(m_uId, location, &value);

		return true;
	}
	bool Shader::getUniform(const String& name, Float& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		glGetUniformFv(m_uId, location, &value);

		return true;
	}
	bool Shader::getUniform(const String& name, Vec2I& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		GLint array[2] = { 0 };
		glGetUniformiv(m_uId, location, array);
		value = { array[0], array[1] };

		return true;
	}
	bool Shader::getUniform(const String& name, Vec2& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		Float items[2] = { 0.0 };
		glGetUniformFv(m_uId, location, items);
		value = Vec2 { items[0], items[1] };

		return true;
	}
	bool Shader::getUniform(const String& name, Vec3& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		Float items[3] = { 0.0 };
		glGetUniformFv(m_uId, location, items);
		value = Vec3 { items[0], items[1], items[2] };

		return true;
	}
	bool Shader::getUniform(const String& name, Vec4& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		Float items[4] = { 0.0 };
		glGetUniformFv(m_uId, location, items);
		value = Vec4 { items[0], items[1], items[2], items[3] };

		return true;
	}
	bool Shader::getUniform(const String& name, Quat& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		Float items[4] = { 0.0 };
		glGetUniformFv(m_uId, location, items);
		value = Quat { items[0], items[1], items[2], items[3] };

		return true;
	}
	bool Shader::getUniform(const String& name, Color& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		Float items[4] = { 0.0 };
		glGetUniformFv(m_uId, location, items);
		value = Color { items[0], items[1], items[2], items[3] };

		return true;
	}
	bool Shader::getUniform(const String& name, Mat4& value) const {
		GLint location = glGetUniformLocation(m_uId, name.c_str());
		if (location < 0) {
			LogError(String::Format("Could not locate shader uniform {}!", name), ErrorCode::ShaderUniformNotFound);
			return false;
		}

		std::array<Float, 16> data { };
		glGetUniformFv(m_uId, location, data.data());
		value = Mat4 { data };

		return true;
	}

	void Shader::apply() const {
		glUseProgram(m_uId);
	}

	const Shader Shader::Null { 0 };
}