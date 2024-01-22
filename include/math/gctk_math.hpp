#pragma once

#include <cmath>
#include <array>
#include <chrono>
#include <random>

#include "fmt/format.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/mat4x4.hpp"

namespace Gctk {
#if defined(GCTK_USE_DOUBLE_FLOAT) && GCTK_USE_DOUBLE_FLOAT != 0
	using Float = double;
#else
	using Float = float;
#endif

	constexpr Float Min(Float value, Float min);
	constexpr Float Max(Float value, Float max);
	constexpr Float Clamp(Float value, Float min, Float max);
	constexpr Float Lerp(Float min, Float max, Float amount);
	constexpr Float Remap(Float value, Float inMin, Float inMax, Float outMin, Float outMax);
	inline constexpr Float DegToRad(Float angle) {
		return angle * static_cast<Float>(0.017453292519943295);
	}
	inline constexpr Float RadToDeg(Float angle) {
		return angle * static_cast<Float>(57.29577951308232);
	}
	inline constexpr Float Pi() { return glm::pi<Float>(); }

	using GlmVec2 = glm::vec<2, Float, glm::defaultp>;
	using GlmVec3 = glm::vec<3, Float, glm::defaultp>;
	using GlmVec4 = glm::vec<4, Float, glm::defaultp>;
	using GlmQuat = glm::qua<Float, glm::defaultp>;
	using GlmMat4 = glm::mat<4, 4, Float, glm::defaultp>;

	union Vec2 {
		struct { Float x, y; };
		struct { Float width, height; };

		Vec2(): x(0.0), y(0.0) { }
		Vec2(const Float x, const Float y): x(x), y(y) { }
		Vec2(Vec2&& vec) = default;
		Vec2(const Vec2& vec) = default;
		Vec2(const GlmVec2& vec): x(vec.x), y(vec.y) { } // NOLINT: Compatibility with glm
		explicit Vec2(const std::array<Float, 2>& array): x(array[0]), y(array[1]) { }

		Vec2& operator= (Vec2&& vec) = default;
		Vec2& operator= (const Vec2& vec) = default;

		constexpr bool operator== (const Vec2& other) const { return x == other.x && y == other.y; }
		constexpr bool operator!= (const Vec2& other) const = default;

		operator GlmVec2() const; // NOLINT: Compatibility with glm
		explicit operator std::array<Float, 2>() const;

		Vec2 operator+ (const Vec2& other) const;
		Vec2 operator- (const Vec2& other) const;
		Vec2 operator* (const Vec2& other) const;
		Vec2 operator/ (const Vec2& other) const;
		Vec2 operator* (Float scale) const;
		Vec2 operator/ (Float scale) const;
		Vec2 operator- () const;

		Vec2& operator+= (const Vec2& other);
		Vec2& operator-= (const Vec2& other);
		Vec2& operator*= (const Vec2& other);
		Vec2& operator/= (const Vec2& other);
		Vec2& operator*= (Float scale);
		Vec2& operator/= (Float scale);

		[[nodiscard]] constexpr Float lengthSqrt() const;
		[[nodiscard]] constexpr Float length() const;
		[[nodiscard]] constexpr Float dot(const Vec2& other) const;
		[[nodiscard]] Float distance(const Vec2& other) const;
		[[nodiscard]] Vec2 normalized() const;
		[[nodiscard]] Vec2 swapComponents() const;
		[[nodiscard]] Vec2 min(const Vec2& min) const;
		[[nodiscard]] Vec2 max(const Vec2& max) const;
		[[nodiscard]] Vec2 clamp(const Vec2& min, const Vec2& max) const;

		static Vec2 Lerp(const Vec2& min, const Vec2& max, Float amount);

		static const Vec2 Zero;
		static const Vec2 UnitX;
		static const Vec2 UnitY;
		static const Vec2 One;
	};

	Vec2 operator* (Float scale, const Vec2& vec);
	Vec2 operator/ (Float scale, const Vec2& vec);

	struct Vec3 {
		Float x, y, z;

		Vec3(): x(0.0), y(0.0), z(0.0) { }
		Vec3(Float x, Float y, Float z): x(x), y(y), z(z) { }
		Vec3(const Vec2& xy, Float z): x(xy.x), y(xy.y), z(z) { }
		Vec3(Vec3&& vec) = default;
		Vec3(const Vec3& vec) = default;
		Vec3(const GlmVec3& vec): x(vec.x), y(vec.y), z(vec.z) { } // NOLINT: Compatibility with glm
		explicit Vec3(const std::array<Float, 3>& array): x(array[0]), y(array[1]), z(array[2]) { }

		Vec3& operator= (Vec3&& vec) = default;
		Vec3& operator= (const Vec3& vec) = default;

		constexpr bool operator== (const Vec3& other) const { return x == other.x && y == other.y && z == other.z; }
		constexpr bool operator!= (const Vec3& other) const = default;

		operator GlmVec3() const; // NOLINT: Compatibility with glm
		explicit operator std::array<Float, 3>() const;

		Vec3 operator+ (const Vec3& other) const;
		Vec3 operator- (const Vec3& other) const;
		Vec3 operator* (const Vec3& other) const;
		Vec3 operator/ (const Vec3& other) const;
		Vec3 operator* (Float scale) const;
		Vec3 operator/ (Float scale) const;
		Vec3 operator- () const;

		Vec3& operator+= (const Vec3& other);
		Vec3& operator-= (const Vec3& other);
		Vec3& operator*= (const Vec3& other);
		Vec3& operator/= (const Vec3& other);
		Vec3& operator*= (Float scale);
		Vec3& operator/= (Float scale);

		[[nodiscard]] constexpr Float lengthSqrt() const;
		[[nodiscard]] constexpr Float length() const;
		[[nodiscard]] constexpr Float dot(const Vec3& other) const;
		[[nodiscard]] Vec3 cross(const Vec3& other) const;
		[[nodiscard]] Float distance(const Vec3& other) const;
		[[nodiscard]] Vec3 normalized() const;
		[[nodiscard]] inline Vec2 xy() const { return { x, y }; }
		[[nodiscard]] inline Vec2 xz() const { return { x, z }; }
		[[nodiscard]] inline Vec2 yz() const { return { y, z }; }
		[[nodiscard]] Vec3 min(const Vec3& min) const;
		[[nodiscard]] Vec3 max(const Vec3& max) const;
		[[nodiscard]] Vec3 clamp(const Vec3& min, const Vec3& max) const;

		static Vec3 Lerp(const Vec3& min, const Vec3& max, Float amount);

		static const Vec3 Zero;
		static const Vec3 UnitX;
		static const Vec3 UnitY;
		static const Vec3 UnitZ;
		static const Vec3 One;
	};

	struct Vec4 {
		Float x, y, z, w;

		Vec4(): x(0), y(0), z(0), w(0) { }
		Vec4(Float x, Float y, Float z, Float w): x(x), y(y), z(z), w(w) { }
		Vec4(const Vec3& xyz, Float w): x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
		Vec4(const Vec2& xy, const Vec2& zw): x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
		Vec4(const Vec2& xy, Float z, Float w): x(xy.x), y(xy.y), z(z), w(w) { }
		Vec4(Vec4&& vec) = default;
		Vec4(const Vec4& vec) = default;
		Vec4(const GlmVec4& vec): x(vec.x), y(vec.y), z(vec.z), w(vec.w) { } // NOLINT: Compatibility with glm
		explicit Vec4(const std::array<Float, 4>& array): x(array[0]), y(array[1]), z(array[2]), w(array[3]) { }

		Vec4& operator= (Vec4&& vec) = default;
		Vec4& operator= (const Vec4& vec) = default;

		constexpr bool operator== (const Vec4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		constexpr bool operator!= (const Vec4& other) const = default;

		operator GlmVec4() const; // NOLINT: Compatibility with glm
		explicit operator std::array<Float, 4>() const;

		Vec4 operator+ (const Vec4& other) const;
		Vec4 operator- (const Vec4& other) const;
		Vec4 operator* (const Vec4& other) const;
		Vec4 operator/ (const Vec4& other) const;
		Vec4 operator* (Float scale) const;
		Vec4 operator/ (Float scale) const;
		Vec4 operator- () const;

		Vec4& operator+= (const Vec4& other);
		Vec4& operator-= (const Vec4& other);
		Vec4& operator*= (const Vec4& other);
		Vec4& operator/= (const Vec4& other);
		Vec4& operator*= (Float scale);
		Vec4& operator/= (Float scale);

		[[nodiscard]] constexpr Float lengthSqrt() const;
		[[nodiscard]] constexpr Float length() const;
		[[nodiscard]] constexpr Float dot(const Vec4& other) const;
		[[nodiscard]] Float distance(const Vec4& other) const;
		[[nodiscard]] Vec4 normalized() const;
		[[nodiscard]] inline Vec2 xy() const { return { x, y }; }
		[[nodiscard]] inline Vec2 xz() const { return { x, z }; }
		[[nodiscard]] inline Vec2 yz() const { return { y, z }; }
		[[nodiscard]] inline Vec3 xyz() const { return { x, y, z }; }
		[[nodiscard]] inline Vec3 yzw() const { return { y, z, w }; }
		[[nodiscard]] inline Vec3 xzw() const { return { x, z, w }; }
		[[nodiscard]] inline Vec3 xyw() const { return { x, y, w }; }
		[[nodiscard]] Vec4 min(const Vec4& min) const;
		[[nodiscard]] Vec4 max(const Vec4& max) const;
		[[nodiscard]] Vec4 clamp(const Vec4& min, const Vec4& max) const;

		static Vec4 Lerp(const Vec4& min, const Vec4& max, Float amount);

		static const Vec4 Zero;
		static const Vec4 UnitX;
		static const Vec4 UnitY;
		static const Vec4 UnitZ;
		static const Vec4 UnitW;
		static const Vec4 One;
	};

	enum class AngleKind {
		Degrees,
		Radians
	};

	class Angle {
		Float m_fAngle;
		AngleKind m_eKind;
	public:
		Angle(const Float angle, const AngleKind kind = AngleKind::Degrees): m_fAngle(angle), m_eKind(kind) { } // NOLINT: Implicit conversion intended.
		Angle(): Angle(0.0) { }
		Angle(Angle&& angle) = default;
		Angle(const Angle& angle) = default;

		[[nodiscard]] inline Float asRadians() const { return m_eKind == AngleKind::Radians ? m_fAngle : DegToRad(m_fAngle); }
		[[nodiscard]] inline Float asDegrees() const { return m_eKind == AngleKind::Degrees ? m_fAngle : RadToDeg(m_fAngle); }
	};

	struct AxisAngle {
		Vec3 axis;
		Angle angle;

		AxisAngle(): axis(Vec3::Zero), angle(0.0) { }
		AxisAngle(const Vec3& axis, const Angle& angle): axis(axis), angle(angle) { }
		explicit AxisAngle(const Vec4& axisAngle): axis(axisAngle.xyz()), angle(axisAngle.w) { }

		explicit operator Vec4() const { return { axis, angle.asDegrees() }; }
	};

	struct Quat : public Vec4 {
		Quat(): Vec4(0.0, 0.0, 0.0, 1.0) { }
		Quat(Float x, Float y, Float z, Float w): Vec4(x, y, z, w) { }
		Quat(Quat&& quat) = default;
		Quat(const Quat& quat) = default;
		Quat(const GlmQuat& quat): Vec4(quat.x, quat.y, quat.z, quat.w) { } // NOLINT: Compatibility with glm
		explicit Quat(const std::array<Float, 4>& array): Vec4(array) { }

		operator GlmQuat() const; // NOLINT: Compatibility with glm

		[[nodiscard]] AxisAngle toAxisAngle() const;
		[[nodiscard]] Vec3 toEuler() const;

		static Quat FromAxisAngle(const AxisAngle& axisAngle);
		static Quat FromEuler(const Vec3& euler);
		static Quat LookAt(const Vec3& dir, const Vec3& up);

		static const Quat Identity;
	};

	struct Mat4 {
		std::array<Vec4, 4> columns;

		Mat4(): columns({ Vec4::UnitX, Vec4::UnitY, Vec4::UnitZ, Vec4::UnitW }) { }
		explicit Mat4(Float scalar): columns({ Vec4::UnitX * scalar, Vec4::UnitY * scalar, Vec4::UnitZ * scalar, Vec4::UnitW * scalar }) { }
		Mat4(Mat4&& mat) = default;
		Mat4(const Mat4& mat) = default;
		Mat4(const GlmMat4& mat): columns({ mat[0], mat[1], mat[2], mat[3] }) { } // NOLINT: Compatibility with glm
		explicit Mat4(const std::array<Vec4, 4>& array): columns(array) { }
		explicit Mat4(const std::array<Float, 16>& array);

		operator GlmMat4() const; // NOLINT: Compatibility with glm
		explicit operator std::array<Vec4, 4>() const;
		explicit operator std::array<Float, 16>() const;

		constexpr bool operator== (const Mat4& mat) const;
		constexpr bool operator!= (const Mat4& mat) const = default;

		Mat4 operator* (const Mat4& mat) const;
		Mat4& operator*= (const Mat4& mat);

		const Float* operator* () const;

		[[nodiscard]] std::array<Vec4, 4> rows() const;
		[[nodiscard]] Mat4 transpose() const;
		[[nodiscard]] Mat4 translate(const Vec3& position) const;
		[[nodiscard]] Mat4 scale(const Vec3& scale) const;
		[[nodiscard]] Mat4 rotate(const AxisAngle& aa) const;
		[[nodiscard]] Mat4 rotate(const Quat& quat) const;
		[[nodiscard]] const Float* asRawPtr() const;

		static Mat4 CreateTranslation(const Vec3& position);
		static Mat4 CreateScale(const Vec3& scale);
		static Mat4 CreateRotation(const AxisAngle& aa);
		static Mat4 CreateRotation(const Quat& quat);
		static Mat4 CreateOrtho(Float left, Float right, Float bottom, Float top, Float near, Float far);
		static Mat4 CreatePerspective(const Angle& fov, Float aspect, Float near, Float far);

		static const Mat4 Identity;
	};

	struct Color {
		Float r, g, b, a;
	public:
		Color(): r(0.0), g(0.0), b(0.0), a(1.0) { }
		explicit Color(Float i, Float a = 1.0): r(i), g(i), b(i), a(a) { }
		Color(Float r, Float g, Float b, Float a = 1.0): r(r), g(g), b(b), a(a) { }
		Color(Color&& color) = default;
		Color(const Color& color) = default;
		explicit Color(uint32_t rgba): Color(
				Float((rgba >> 24) & 0xFF) / Float(255.0),
				Float((rgba >> 16) & 0xFF) / Float(255.0),
				Float((rgba >> 8) & 0xFF) / Float(255.0),
				Float(rgba & 0xFF) / Float(255.0)
		) { }
		explicit Color(const Vec4& vec): r(vec.x), g(vec.y), b(vec.z), a(vec.w) { }
		explicit Color(const Vec3& vec, Float a = 1.0): r(vec.x), g(vec.y), b(vec.z), a(a) { }
		explicit Color(const std::array<Float, 3>& array, Float a = 1.0): r(array[0]), g(array[1]), b(array[2]), a(a) { }
		explicit Color(const std::array<Float, 4>& array): r(array[0]), g(array[1]), b(array[2]), a(array[3]) { }

		inline explicit operator Vec3() const { return { r, g, b }; }
		inline explicit operator Vec4() const { return { r, g, b, a }; }
		inline explicit operator std::array<Float, 3>() const { return { r, g, b }; }
		inline explicit operator std::array<Float, 4>() const { return { r, g, b, a }; }

		inline constexpr bool operator== (const Color& color) const { return r == color.r && g == color.g && b == color.b && a == color.a; }
		constexpr bool operator!= (const Color& color) const = default;

		static Color FromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

		[[nodiscard]] constexpr uint32_t toRgba() const;
		constexpr void toBytes(uint8_t& r, uint8_t& g, uint8_t& b) const;
		constexpr void toBytes(uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) const;

#include "generated/gctk_colours.hpp"
	};
	using Colour = Color;

	class Random {
		std::mt19937 m_rng;
	public:
		explicit Random(uint32_t seed): m_rng(std::random_device()()) { }
		Random(): Random(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) { }

		inline bool NextBool(Float trueChance = 0.5) {
			std::bernoulli_distribution dist(trueChance);
			return dist(m_rng);
		}
		template<typename T>
		inline T NextInt(T min, T max) {
			static_assert(std::is_integral<T>(), "Expected an integer type!");
			std::uniform_int_distribution<T> dist(min, max);
			return dist(m_rng);
		}
		template<typename T>
		inline T NextFloat(T min, T max) {
			static_assert(std::is_floating_point<T>(), "Expected a floating-point type!");
			std::uniform_real_distribution<T> dist(min, max);
			return dist(m_rng);
		}
		inline Vec2 NextVec2(const Vec2& min, const Vec2& max) {
			return { NextFloat(min.x, max.x), NextFloat(min.y, max.y) };
		}
		inline Vec3 NextVec3(const Vec3& min, const Vec3& max) {
			return { NextFloat(min.x, max.x), NextFloat(min.y, max.y), NextFloat(min.z, max.z) };
		}
		inline Vec4 NextVec4(const Vec4& min, const Vec4& max) {
			return { NextFloat(min.x, max.x), NextFloat(min.y, max.y), NextFloat(min.z, max.z), NextFloat(min.w, max.w) };
		}
		inline Vec2 InCircle(Float radius) {
			const Float r = NextFloat(static_cast<Float>(0.0), radius);
			const Float angle = NextFloat(static_cast<Float>(0.0), Pi());
			return {
				r * std::cos(angle),
				r * std::sin(angle)
			};
		}
	};

	union Vec2I {
		struct { long x, y; };
		struct { long width, height; };

		Vec2I(): x(0), y(0) { }
		Vec2I(long x, long y): x(x), y(y) { }
		explicit Vec2I(const std::array<long, 2>& array): x(array[0]), y(array[1]) { }

		[[nodiscard]] constexpr long lengthSqr() const { return x * x + y * y; }
		[[nodiscard]] Float length() const { return std::sqrt(static_cast<Float>(x * x + y * y)); }
		[[nodiscard]] constexpr long dot(const Vec2I& other) const { return x * other.x + y * other.y; }
		[[nodiscard]] Float distance(const Vec2I& other) const { return (other - *this).length(); }

		Vec2I operator+ (const Vec2I& other) const;
		Vec2I operator- (const Vec2I& other) const;
		Vec2I operator* (const Vec2I& other) const;
		Vec2I operator/ (const Vec2I& other) const;
		Vec2I operator* (long scale) const;
		Vec2I operator/ (long scale) const;
		Vec2I operator- () const;

		Vec2I& operator+= (const Vec2I& other);
		Vec2I& operator-= (const Vec2I& other);
		Vec2I& operator*= (const Vec2I& other);
		Vec2I& operator/= (const Vec2I& other);
		Vec2I& operator*= (long scale);
		Vec2I& operator/= (long scale);

		constexpr bool operator== (const Vec2I& other) const { return x == other.x && y == other.y; }
		constexpr bool operator!= (const Vec2I& other) const = default;
		explicit operator Vec2() const { return { static_cast<Float>(x), static_cast<Float>(y) }; }
	};
	struct Rect {
		Vec2 location, size;

		Rect(): location(), size() { }
		Rect(Float x, Float y, Float width, Float height): location(x, y), size(width, height) { }
		Rect(const Vec2& location, const Vec2& size): location(location), size(size) { }
		explicit Rect(const std::array<Float, 4>& array): location(array[0], array[1]), size(array[2], array[3]) { }
		explicit Rect(const std::array<Vec2, 2>& array): location(array[0]), size(array[1]) { }

		bool is_point_inside(const Vec2& point) const;
		bool is_rect_collide(const Rect& rect) const;
	};
}

template<>
struct fmt::formatter<Gctk::Vec2I> : fmt::nested_formatter<long> {
	auto format(const Gctk::Vec2I& vec, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "{}, {}", nested(vec.x), nested(vec.y));
		});
	}
};
template<>
struct fmt::formatter<Gctk::Vec2> : fmt::nested_formatter<Gctk::Float> {
	auto format(const Gctk::Vec2& vec, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "{}, {}", nested(vec.x), nested(vec.y));
		});
	}
};
template<>
struct fmt::formatter<Gctk::Vec3> : fmt::nested_formatter<Gctk::Float> {
	auto format(const Gctk::Vec3& vec, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "{}, {}, {}", nested(vec.x), nested(vec.y), nested(vec.z));
		});
	}
};
template<>
struct fmt::formatter<Gctk::Vec4> : fmt::nested_formatter<Gctk::Float> {
	auto format(const Gctk::Vec4& vec, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "{}, {}, {}, {}", nested(vec.x), nested(vec.y), nested(vec.z), nested(vec.w));
		});
	}
};
template<>
struct fmt::formatter<Gctk::Quat> : fmt::nested_formatter<Gctk::Float> {
	auto format(const Gctk::Quat& quat, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "{}, {}, {}, {}", nested(quat.x), nested(quat.y), nested(quat.z), nested(quat.w));
		});
	}
};
template<>
struct fmt::formatter<Gctk::Color> : fmt::nested_formatter<Gctk::Float> {
	auto format(const Gctk::Color& color, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "{}, {}, {}, {}", nested(color.r), nested(color.g), nested(color.b), nested(color.a));
		});
	}
};
template<>
struct fmt::formatter<Gctk::Mat4> : fmt::nested_formatter<Gctk::Float> {
	auto format(const Gctk::Mat4& mat, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "[ {} ],\n[ {} ],\n[ {} ],\n[ {} ]",
				mat.columns[0],
				mat.columns[1],
				mat.columns[2],
				mat.columns[3]
			);
		});
	}
};
template<>
struct fmt::formatter<Gctk::Rect> : fmt::nested_formatter<Gctk::Vec2> {
	auto format(const Gctk::Rect& rect, fmt::format_context & ctx) const {
		return write_padded(ctx, [=](auto out) {
			return format_to(out, "[ {} ], [ {} ]", nested(rect.location), nested(rect.size));
		});
	}
};