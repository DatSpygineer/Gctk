#include "gctk_mesh.hpp"

#include <fstream>
#include "strlib/string.hpp"

namespace Gctk {
	struct MeshHeader {
		uint32_t vertex_count, texcoord_count, element_count;
		MeshVertexPrecision precision; uint8_t unused;
		uint16_t metadata_size;
	};

	consteval uint32_t MeshIdentifier() {
		return 'GMDL';
	}

	Mesh::~Mesh() {
		if (m_uVao) {
			glDeleteVertexArrays(1, &m_uVao);
			m_uVao = 0;
		}
		if (m_uVbo) {
			glDeleteBuffers(1, &m_uVbo);
			m_uVbo = 0;
		}
	}

	ErrorCode Mesh::Load(Mesh& mesh, const String& path, MeshDrawType draw_type, String& metadata) {
		std::ifstream ifs(path.std_str());
		std::vector<uint8_t> data;

		if (ifs.is_open()) {
			ifs.seekg(std::ios::end);
			size_t len = ifs.tellg();
			ifs.seekg(std::ios::beg);
			data.reserve(len);
			ifs.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(len));
			return Load(mesh, data, draw_type, metadata);
		}

		LogError(String::Format("Could not open file \"{}\"!", path), ErrorCode::CannotOpenFile);
		return ErrorCode::CannotOpenFile;
	}
	ErrorCode Mesh::Load(Mesh& mesh, const std::vector<uint8_t>& bytes, MeshDrawType draw_type, String& metadata) {
		MeshHeader header { };
		ByteReader reader(bytes);
		uint32_t identifier = 0;
		reader.read(identifier);

		if (identifier != MeshIdentifier()) {
			LogError("Failed to load mesh: Identifier is invalid or missing!", ErrorCode::MeshIdentifierError);
			return ErrorCode::MeshIdentifierError;
		}
		if (!reader.read(header)) {
			LogError("Failed to load mesh: Header not found!", ErrorCode::MeshHeaderMissing);
			return ErrorCode::MeshHeaderMissing;
		}

		std::vector<int> elements;
		elements.reserve(header.element_count);
		metadata.reserve(header.metadata_size);

		if (header.metadata_size > 0) {
			if (!reader.read(metadata, header.metadata_size)) {
				LogError("Failed to load mesh: could not read metadata!", ErrorCode::MeshLoadError);
				return ErrorCode::MeshLoadError;
			}
		}

		if (header.precision == MeshVertexPrecision::Float) {
			std::vector<float> verts;

			reader.read(verts, header.vertex_count);
			reader.read(verts, header.texcoord_count, false);
			reader.read(elements, header.element_count);

			return Create(mesh, verts, elements, draw_type);
		} else {
			std::vector<double> verts;

			reader.read(verts, header.vertex_count);
			reader.read(verts, header.texcoord_count, false);
			reader.read(elements, header.element_count);

			return Create(mesh, verts, elements, draw_type);
		}
	}

	ErrorCode Mesh::Create(Mesh& mesh, const std::vector<float>& verts, const std::vector<int>& elements, MeshDrawType draw_type) {
		GLuint vbo, vao, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);

		if (vbo == 0) {
			LogError("Failed to load mesh: Could not to generate vertex buffer object!", ErrorCode::MeshOpenGLError);
			return ErrorCode::MeshOpenGLError;
		}
		if (vao == 0) {
			LogError("Failed to load mesh: Could not to generate vertex array object!", ErrorCode::MeshOpenGLError);
			return ErrorCode::MeshOpenGLError;
		}
		if (ebo == 0 && !elements.empty()) {
			LogError("Failed to load mesh: Could not to generate element buffer object!", ErrorCode::MeshOpenGLError);
			return ErrorCode::MeshOpenGLError;
		}

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(verts.size()), verts.data(), GL_STATIC_DRAW);

		if (ebo != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(elements.size()), elements.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(3));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return ErrorCode::None;
	}

	ErrorCode Mesh::Create(Mesh& mesh, const std::vector<double>& verts, const std::vector<int>& elements, MeshDrawType draw_type) {
		GLuint vbo, vao, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);

		if (vbo == 0) {
			LogError("Failed to load mesh: Could not to generate vertex buffer object!", ErrorCode::MeshOpenGLError);
			return ErrorCode::MeshOpenGLError;
		}
		if (vao == 0) {
			LogError("Failed to load mesh: Could not to generate vertex array object!", ErrorCode::MeshOpenGLError);
			return ErrorCode::MeshOpenGLError;
		}
		if (ebo == 0 && !elements.empty()) {
			LogError("Failed to load mesh: Could not to generate element buffer object!", ErrorCode::MeshOpenGLError);
			return ErrorCode::MeshOpenGLError;
		}

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(verts.size()), verts.data(), GL_STATIC_DRAW);

		if (ebo != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(elements.size()), elements.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), nullptr);
		glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), reinterpret_cast<const void*>(3));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return ErrorCode::None;
	}

	void Mesh::draw() const {
		glBindVertexArray(m_uVao);
		if (m_uEbo != 0) {
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_uVertexCount), GL_UNSIGNED_INT, nullptr);
		} else {
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_uVertexCount));
		}
		glBindVertexArray(0);
	}
}