#include "gctk_texture.hpp"

namespace Gctk {
	Texture::Texture(const GTex& texture_data) {

	}
	Texture::~Texture() {

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
			return { ErrorCode::TextureOpenGLError };
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
				// TODO:
			} break;
			case GL_TEXTURE_2D_ARRAY: {
				// TODO:
			} break;
			case GL_TEXTURE_CUBE_MAP: {
				// TODO:
			} break;
			case GL_TEXTURE_CUBE_MAP_ARRAY: {
				// TODO:
			} break;
		}

		if (texture_data.header().mipmaps) {
			glGenerateMipmap(target);
		}

		glBindTexture(target, 0);

		try {
			return { Texture(id, {texture_data.header().width, texture_data.header().height }, texture_data.header().depth, target) };
		} catch (const std::exception& _) {
			return { ErrorCode::TextureOpenGLError };
		}
	}
}