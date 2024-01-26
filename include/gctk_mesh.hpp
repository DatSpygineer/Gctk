#pragma once

#include <iostream>
#include <vector>

#include "gctk_common.hpp"
#include "gctk_texture.hpp"

namespace Gctk {
	enum class MeshVertexPrecision : uint8_t {
		Float,
		Double
	};
	enum class MeshDrawType : GLenum {
		StaticDraw	= GL_STATIC_DRAW,	// Data is set once and used many times.
		DynamicDraw = GL_DYNAMIC_DRAW,	// Data is changing and used many times.
		StreamDraw	= GL_STREAM_DRAW	// Data is set once and used a few times.
	};

	class Mesh {
		GLuint m_uVbo, m_uVao, m_uEbo;
		MeshVertexPrecision m_ePrecision;
		uint32_t m_uVertexCount;
	public:
		Mesh():  m_uVao(0), m_uVbo(0), m_uEbo(0), m_uVertexCount(0), m_ePrecision(MeshVertexPrecision::Float) { }
		Mesh(GLuint vbo, GLuint vao, GLuint ebo, uint32_t vertex_count, MeshVertexPrecision precision):
			m_uVbo(vbo), m_uVao(vao), m_uEbo(ebo), m_uVertexCount(vertex_count), m_ePrecision(precision) { }
		Mesh(Mesh&& mesh) noexcept: m_uVao(mesh.m_uVao), m_uVbo(mesh.m_uVbo), m_uEbo(mesh.m_uEbo),
			m_ePrecision(mesh.m_ePrecision), m_uVertexCount(mesh.m_uVertexCount)
			{ mesh.m_uVbo = 0; mesh.m_uVao = 0; mesh.m_uEbo = 0; mesh.m_uVertexCount = 0; }
		Mesh(const Mesh& mesh) = default;
		~Mesh();

		static ErrorCode Load(Mesh& mesh, const String& path, MeshDrawType draw_type, String& metadata);
		static ErrorCode Load(Mesh& mesh, const std::vector<uint8_t>& bytes, MeshDrawType draw_type, String& metadata);
		static ErrorCode Create(Mesh& mesh, const std::vector<float>& verts, const std::vector<int>& elements, MeshDrawType draw_type);
		static ErrorCode Create(Mesh& mesh, const std::vector<double>& verts, const std::vector<int>& elements, MeshDrawType draw_type);

		[[nodiscard]] inline constexpr MeshVertexPrecision precision() const { return m_ePrecision; }
		[[nodiscard]] inline constexpr GLuint vbo() const { return m_uVbo; }
		[[nodiscard]] inline constexpr GLuint vao() const { return m_uVao; }
		[[nodiscard]] inline constexpr uint32_t vertex_count() const { return m_uVertexCount; }

		inline constexpr bool operator== (const Mesh& mesh) const { return m_uVbo == mesh.m_uVbo && m_uVao == mesh.m_uVbo; }
		inline constexpr bool operator!= (const Mesh& mesh) const = default;

		void draw() const;
	};
}