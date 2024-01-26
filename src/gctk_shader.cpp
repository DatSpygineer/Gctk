#include "gctk_shader.hpp"

#include "gctk_debug.hpp"

namespace Gctk {

	Shader::~Shader() {
		if (m_uId != 0) {
			glDeleteProgram(m_uId);
			m_uId = 0;
		}
	}

	bool Shader::Load(Shader& shader, const String& vertex, const String& fragment) {
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
			return false;
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
			return false;
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
			return false;
		}

		shader = Shader { id };
		return true;
	}
}