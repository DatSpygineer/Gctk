#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "gctk_common.hpp"
#include "gctk_debug.hpp"

namespace Gctk {
	enum class GTexPixelFormat : uint8_t {
		Luminescence,
		LuminescenceWithAlpha,
		RGB,
		RGBA,
		ARGB,
		BGR,
		ABGR,
		BGRA,

		Indexed8,
		Indexed8WithAlpha,
		Indexed16,
		Indexed16WithAlpha,

		BC1,
		BC1A,
		BC2,
		BC3,
		BC4U,
		BC4S,
		BC5U,
		BC5S,
		BC6S,
		BC6U,
		BC7,

		DXT1 = BC1,
		DXT1WithAlpha = BC1A,
		DXT3 = BC2,
		DXT5 = BC3,
	};

	inline constexpr GLuint GTexFormatToGLInternalFormat(GTexPixelFormat format) {
		switch (format) {
			case GTexPixelFormat::Luminescence: return GL_LUMINANCE;
			case GTexPixelFormat::LuminescenceWithAlpha: return GL_LUMINANCE_ALPHA;
			case GTexPixelFormat::Indexed8: case GTexPixelFormat::Indexed16: case GTexPixelFormat::RGB: return GL_RGB;
			default: case GTexPixelFormat::Indexed8WithAlpha: case GTexPixelFormat::Indexed16WithAlpha: case GTexPixelFormat::RGBA: case GTexPixelFormat::ARGB: return GL_RGBA;
			case GTexPixelFormat::BGR: return GL_BGR;
			case GTexPixelFormat::BGRA: return GL_BGRA;
			case GTexPixelFormat::ABGR: return GL_ABGR_EXT;
			case GTexPixelFormat::BC1: return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			case GTexPixelFormat::BC1A: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			case GTexPixelFormat::BC2: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			case GTexPixelFormat::BC3: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			case GTexPixelFormat::BC4U: return GL_COMPRESSED_RED_RGTC1;
			case GTexPixelFormat::BC4S: return GL_COMPRESSED_SIGNED_RED_RGTC1;
			case GTexPixelFormat::BC5U: return GL_COMPRESSED_RG_RGTC2;
			case GTexPixelFormat::BC5S: return GL_COMPRESSED_SIGNED_RG_RGTC2;
			case GTexPixelFormat::BC6U: return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
			case GTexPixelFormat::BC6S: return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
			case GTexPixelFormat::BC7: return GL_COMPRESSED_RGBA_BPTC_UNORM;
		}
	}
	inline constexpr GLuint GTexFormatToGLFormat(GTexPixelFormat format) {
		switch (format) {
			case GTexPixelFormat::Luminescence: return GL_LUMINANCE;
			case GTexPixelFormat::LuminescenceWithAlpha: return GL_LUMINANCE_ALPHA;
			case GTexPixelFormat::BC4U: case GTexPixelFormat::BC4S: return GL_RED;
			case GTexPixelFormat::BC5U: case GTexPixelFormat::BC5S: return GL_RG;
			case GTexPixelFormat::BC1: case GTexPixelFormat::BC6S:
			case GTexPixelFormat::Indexed8: case GTexPixelFormat::Indexed16: case GTexPixelFormat::BC6U: case GTexPixelFormat::RGB: return GL_RGB;
			case GTexPixelFormat::BGR: return GL_BGR;
			case GTexPixelFormat::BGRA: return GL_BGRA;
			case GTexPixelFormat::ABGR: return GL_ABGR_EXT;
			default:
			case GTexPixelFormat::RGBA: case GTexPixelFormat::ARGB:
			case GTexPixelFormat::BC1A: case GTexPixelFormat::BC2: case GTexPixelFormat::BC3: case GTexPixelFormat::BC7:
			case GTexPixelFormat::Indexed8WithAlpha: case GTexPixelFormat::Indexed16WithAlpha: return GL_RGBA;
		}
	}

	enum class GTexTarget {
		Texture1D,
		Texture2D,
		Texture3D,
		Cubemap
	};

	inline constexpr uint32_t GTexIdentifier() {
		return 0x47544558U;
	}
	inline constexpr uint8_t GTexVersion() {
		return 0U;
	}

	struct GTexHeader {
		uint32_t identifier;
		uint8_t version;
		union {
			uint8_t flags;
			struct {
				uint8_t point_filter: 1;
				uint8_t clamp_s: 1;
				uint8_t clamp_r: 1;
				uint8_t clamp_t: 1;
				uint8_t target: 2;
				uint8_t array: 1;
				uint8_t mipmaps: 1;
			};
			GTexPixelFormat format;
		};
		uint16_t width, height, depth, sample_count, palette_size;
	};

	class GTex {
		GTexHeader m_header;
		std::vector<uint8_t> m_imageData;

	public:
		GTex(): m_header(), m_imageData() { }
		explicit GTex(const std::vector<uint8_t>& data);
		explicit GTex(const std::vector<uint8_t>& data, const GTexHeader& header);

		static Result<GTex, ErrorCode> LoadFromBytes(const std::vector<uint8_t>& data);
		static Result<GTex, ErrorCode> LoadFromBytes( const std::vector<uint8_t>& data, const GTexHeader& header) noexcept;
		static Result<GTex, ErrorCode> LoadFromFile(const std::string& path);
		static Result<GTex, ErrorCode> LoadFromFile(const std::string& path, const GTexHeader& header);

		[[nodiscard]] constexpr GTexHeader header() const { return m_header; }
		[[nodiscard]] constexpr std::vector<uint8_t> image_data() const { return m_imageData; }
	};
}