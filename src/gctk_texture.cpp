#include "gctk_texture.hpp"

namespace Gctk {
	Texture::Texture(const GTex& texture_data) {
		GLuint target = GL_TEXTURE_2D;
		switch (static_cast<GTexTarget>(texture_data.header().target)) {
			case GTexTarget::Texture1D: {
				target = texture_data.header().array ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D;
			} break;
			case GTexTarget::Texture2D: {
				target = texture_data.header().array ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
			} break;
			case GTexTarget::Texture3D: {
				target = GL_TEXTURE_3D;
			} break;
			case GTexTarget::Cubemap: {
				target = texture_data.header().array ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_CUBE_MAP;
			} break;
		}

		GLuint id = 0;
		glGenTextures(1, &id);
		if (id == 0) {
			throw EngineError("Failed to generate texture!", ErrorCode::TextureOpenGLError);
		}

		glBindTexture(target, id);

		glTexParameteri(target, GL_TEXTURE_WRAP_R, texture_data.header().clamp_r ? GL_CLAMP : GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, texture_data.header().clamp_s ? GL_CLAMP : GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, texture_data.header().clamp_t ? GL_CLAMP : GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, texture_data.header().point_filter ? GL_NEAREST : GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
						texture_data.header().mipmaps ?
						(texture_data.header().point_filter ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR) :
						(texture_data.header().point_filter ? GL_NEAREST : GL_LINEAR)
		);

		switch (target) {
			case GL_TEXTURE_1D: {
				glTexImage1D(
						target, 0,
						GTexFormatToGLInternalFormat(texture_data.header().format),
						texture_data.header().width, 0,
						GTexFormatToGLFormat(texture_data.header().format),
						GL_UNSIGNED_BYTE,
						texture_data.image_data().data()
				);
			} break;
			case GL_TEXTURE_2D: {
				glTexImage2D(
						target, 0,
						GTexFormatToGLInternalFormat(texture_data.header().format),
						texture_data.header().width, texture_data.header().height, 0,
						GTexFormatToGLFormat(texture_data.header().format),
						GL_UNSIGNED_BYTE,
						texture_data.image_data().data()
				);
			} break;
			case GL_TEXTURE_3D: {
				glTexImage3D(
						target, 0,
						GTexFormatToGLInternalFormat(texture_data.header().format),
						texture_data.header().width, texture_data.header().height, texture_data.header().depth, 0,
						GTexFormatToGLFormat(texture_data.header().format),
						GL_UNSIGNED_BYTE,
						texture_data.image_data().data()
				);
			} break;
			case GL_TEXTURE_1D_ARRAY: {
				glTexStorage2D(target, 0,
						GTexFormatToGLInternalFormat(texture_data.header().format),
						texture_data.header().width,
						texture_data.header().depth
				);
				glTexSubImage2D(target, 0, 0, 0,
						texture_data.header().width,
						texture_data.header().depth,
						GTexFormatToGLFormat(texture_data.header().format),
						GL_UNSIGNED_BYTE,
						texture_data.image_data().data()
				);
			} break;
			case GL_TEXTURE_2D_ARRAY: {
				glTexStorage3D(target, 0,
						GTexFormatToGLInternalFormat(texture_data.header().format),
						texture_data.header().width,
						texture_data.header().height,
						texture_data.header().depth
				);
				glTexSubImage3D(target, 0, 0, 0, 0,
						texture_data.header().width,
						texture_data.header().height,
						texture_data.header().depth,
						GTexFormatToGLFormat(texture_data.header().format),
						GL_UNSIGNED_BYTE,
						texture_data.image_data().data()
				);
			} break;
			case GL_TEXTURE_CUBE_MAP: {
				const GLsizei width = texture_data.header().width / 6;
				glTexStorage2D(target, 0,
						GTexFormatToGLInternalFormat(texture_data.header().format),
						texture_data.header().width,
						texture_data.header().height
				);
				for (GLsizei i = 0; i < 6; i++) {
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, width * i, 0,
							width, texture_data.header().height,
							GTexFormatToGLFormat(texture_data.header().format),
							GL_UNSIGNED_BYTE,
							texture_data.image_data().data()
					);
				}
			} break;
			case GL_TEXTURE_CUBE_MAP_ARRAY: {
				const GLsizei width = texture_data.header().width / 6;
				const GLsizei height = texture_data.header().height / texture_data.header().depth;
				glTexStorage2D(target, 0,
						GTexFormatToGLInternalFormat(texture_data.header().format),
						texture_data.header().width,
						texture_data.header().height
				);

				for (GLsizei j = 0; j < texture_data.header().depth; j++) {
					for (GLsizei i = 0; i < 6; i++) {
						glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, width * i, height * j,
								width, height,
								GTexFormatToGLFormat(texture_data.header().format),
								GL_UNSIGNED_BYTE,
								texture_data.image_data().data()
						);
					}
				}
			} break;
			default: throw EngineError("Failed to load image, invalid texture target!", ErrorCode::TextureOpenGLError);
		}

		if (texture_data.header().mipmaps) {
			glGenerateMipmap(target);
		}

		glBindTexture(target, 0);
	}
	Texture::~Texture() {
		if (m_uId != 0) {
			glDeleteTextures(1, &m_uId);
		}
	}

	Result<Texture, ErrorCode> Texture::Load(const std::vector<uint8_t>& data) {
		auto gtex_res = GTex::LoadFromBytes(data);
		if (!gtex_res.has_value()) {
			return { gtex_res.error().value() };
		}
		return Load(gtex_res.unwrap());
	}
	Result<Texture, ErrorCode> Texture::Load(const std::vector<uint8_t>& data, const GTexHeader& header) {
		auto gtex_res = GTex::LoadFromBytes(data, header);
		if (!gtex_res.has_value()) {
			return { gtex_res.error().value() };
		}
		return Load(gtex_res.unwrap());
	}
	Result<Texture, ErrorCode> Texture::Load(const std::string& path) {
		auto gtex_res = GTex::LoadFromFile(path);
		if (!gtex_res.has_value()) {
			return { gtex_res.error().value() };
		}
		return Load(gtex_res.unwrap());
	}
	Result<Texture, ErrorCode> Texture::Load(const std::string& path, const GTexHeader& header) {
		auto gtex_res = GTex::LoadFromFile(path, header);
		if (!gtex_res.has_value()) {
			return { gtex_res.error().value() };
		}
		return Load(gtex_res.unwrap());
	}
	Result<Texture, ErrorCode> Texture::Load(const GTex& texture_data) {
		try {
			return { Texture(texture_data) };
		} catch (const EngineError& error) {
			return { error.code() };
		}
	}

	void Texture::bind() const {
		glBindTexture(m_uTarget, m_uId);
	}

	const Texture Texture::Null = { };
}