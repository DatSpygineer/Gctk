#include <fstream>

#include "gctk_gtex.hpp"
#include "gctk_common.hpp"

namespace Gctk {
	struct ColorRGB {
		uint8_t r, g, b;
	};
	struct ColorARGB {
		uint8_t a, r, g, b;
	};

	GTex::GTex(const std::vector<uint8_t>& data): m_imageData(), m_header() {
		ByteReader reader { data };

		uint32_t id;
		reader.read(id);
		reader.position(0);

		if (id != GTexIdentifier()) {
			throw std::runtime_error("Could not find GTex header!");
		} else {
			reader.read(m_header);
			if (m_header.version != GTexVersion()) {
				// Add version migration if needed
			}

			// Rearrange ARGB to RGBA
			if (m_header.format == GTexPixelFormat::ARGB) {
				for (size_t i = 0; i < (data.size() / 4) * 4; i += 4) {
					ColorARGB argb = { data.at(i), data.at(i + 1), data.at(i + 2), data.at(i + 3) };
					m_imageData.push_back(argb.a);
					m_imageData.push_back(argb.b);
					m_imageData.push_back(argb.g);
					m_imageData.push_back(argb.r);
				}
			// Processed indexed format
			} else if (m_header.format == GTexPixelFormat::Indexed8 || m_header.format == GTexPixelFormat::Indexed8WithAlpha ||
						m_header.format == GTexPixelFormat::Indexed16 || m_header.format == GTexPixelFormat::Indexed16WithAlpha) {
				if (m_header.palette_size == 0) {
					throw std::runtime_error("Unexpected palette size. Must be more then 0!");
				}
				std::vector<ColorRGB> palette;

				for (uint16_t i = 0; i < m_header.palette_size * 3; i += 3) {
					ColorRGB rgb { };
					if (!reader.read(rgb.r)) { throw std::runtime_error("Buffer overflow while reading!"); }
					if (!reader.read(rgb.g)) { throw std::runtime_error("Buffer overflow while reading!"); }
					if (!reader.read(rgb.b)) { throw std::runtime_error("Buffer overflow while reading!"); }
					palette.push_back(rgb);
				}

				size_t index_count = m_header.width;
				if (m_header.target > static_cast<uint8_t>(GTexTarget::Texture1D)) {
					index_count *= m_header.height;
				}
				if (static_cast<GTexTarget>(m_header.target) == GTexTarget::Texture3D) {
					index_count *= m_header.depth;
				}

				const bool has_alpha = m_header.format == GTexPixelFormat::Indexed8WithAlpha || m_header.format == GTexPixelFormat::Indexed16WithAlpha;

				m_imageData.reserve(index_count * (has_alpha ? 4 : 3));

				size_t index_size = 1;
				if (static_cast<uint8_t>(m_header.format) == static_cast<uint8_t>(GTexPixelFormat::Indexed16) ||
					static_cast<uint8_t>(m_header.format) == static_cast<uint8_t>(GTexPixelFormat::Indexed8WithAlpha)) {
					index_size = 2;
				} else if (static_cast<uint8_t>(m_header.format) == static_cast<uint8_t>(GTexPixelFormat::Indexed16WithAlpha)) {
					index_size = 3;
				}

				for (size_t i = 0; i < index_count * index_size; i += index_size) {
					uint16_t index;
					uint8_t alpha = 0xFF;

					if (index_size == 1) {
						index = reader.read_byte();
					} else {
						reader.read(index);
					}

					if (has_alpha) {
						alpha = reader.read_byte();
					}

					if (index >= palette.size()) {
						throw std::runtime_error("Pixel index is out of range!");
					}

					ColorRGB color = palette.at(index);
					m_imageData.push_back(alpha);
					m_imageData.push_back(color.b);
					m_imageData.push_back(color.g);
					m_imageData.push_back(color.r);
				}
			// Raw format, copy the data.
			} else {
				m_imageData = data;
			}
		}
	}

	GTex::GTex(const std::vector<uint8_t>& data, const GTexHeader& header): m_imageData(data), m_header(header) {
		if (m_header.format == GTexPixelFormat::ARGB) {
			m_imageData.clear();
			m_imageData.reserve(data.size());
			for (size_t i = 0; i < (data.size() / 4) * 4; i += 4) {
				ColorARGB argb = { data.at(i), data.at(i + 1), data.at(i + 2), data.at(i + 3) };
				m_imageData.push_back(argb.a);
				m_imageData.push_back(argb.b);
				m_imageData.push_back(argb.g);
				m_imageData.push_back(argb.r);
			}
		}
	}

	Result<GTex, ErrorCode> GTex::LoadFromBytes(const std::vector<uint8_t>& data) {
		GTex gtex;
		try {
			gtex = GTex(data);
		} catch (const std::exception& _) {
			return { ErrorCode::TextureLoadError };
		}
		return { gtex };
	}

	Result<GTex, ErrorCode> GTex::LoadFromBytes(const std::vector<uint8_t>& data, const GTexHeader& header) noexcept {
		return { GTex(data, header) };
	}

	Result<GTex, ErrorCode> GTex::LoadFromFile(const std::string& path) {
		GTex gtex;
		try {
			std::vector<uint8_t> data;

			std::ifstream ifs(path);
			ifs.seekg(std::ios::end);
			const auto len = ifs.tellg();

			data.reserve(len);
			ifs.seekg(std::ios::beg);

			ifs.read(reinterpret_cast<char*>(data.data()), len);

			if (data.size() < 4) {
				// TODO: Log "Identifier not found" error!
				return { ErrorCode::TextureIdentifierError };
			}

			uint32_t identifier = 0;
			ByteReader reader { data };

			GTexHeader header = { 0 };
			if (!reader.read(identifier) || identifier != GTexIdentifier()) {
				// TODO: Log "Invalid identifier" error
				return { ErrorCode::TextureIdentifierError };
			}
			reader.position(reader.position() - 4);
			reader.read(header);

			gtex = GTex({ reader.current(), data.end() }, header);
		} catch (const std::exception& _) {
			return { ErrorCode::TextureLoadError };
		}

		return { gtex };
	}
	Result<GTex, ErrorCode> GTex::LoadFromFile(const std::string& path, const GTexHeader& header) {
		Option<GTex> gtex;
		try {
			std::vector<uint8_t> data;

			std::ifstream ifs(path);
			ifs.seekg(std::ios::end);
			const auto len = ifs.tellg();

			data.reserve(len);
			ifs.seekg(std::ios::beg);

			ifs.read(reinterpret_cast<char*>(data.data()), len);

			if (data.size() < 4) {
				// TODO: Log "Identifier not found" error!
				return { ErrorCode::TextureIdentifierError };
			}

			uint32_t identifier = 0;
			ByteReader reader { data };

			if (!reader.read(identifier) || identifier != GTexIdentifier()) {
				gtex = GTex({ reader.current(), data.end() }, header);
			} else {
				// TODO: Log warning "Found GTex header, only flags will be overwritten!"
				GTexHeader f_header = { 0 };
				reader.position(reader.position() - 4);
				reader.read(f_header);
				f_header.flags = header.flags;

				gtex = GTex({ reader.current(), data.end() }, f_header);
			}
		} catch (const std::exception& _) {
			return { ErrorCode::TextureLoadError };
		}

		return { gtex, std::nullopt };
	}
}