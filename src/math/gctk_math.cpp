#include "gctk_math.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstdlib>
#include <cstring>

namespace Gctk {
	constexpr Float Min(Float value, Float min) {
		return value < min ? min : value;
	}
	constexpr Float Max(Float value, Float max) {
		return value > max ? max : value;
	}
	constexpr Float Clamp(Float value, Float min, Float max) {
		return value > max ? max : (value < min ? min : value);
	}
	constexpr Float Lerp(Float min, Float max, Float amount) {
		return amount * (max - min) + min;
	}
	constexpr Float Remap(Float value, Float inMin, Float inMax, Float outMin, Float outMax) {
		return Lerp(outMin, outMax, (value - inMin) / (inMax - inMin));
	}

	Vec2::operator GlmVec2() const {
		return { x, y };
	}
	Vec2::operator std::array<Float, 2>() const {
		return { x, y };
	}

	Vec2 Vec2::operator+ (const Vec2& other) const {
		return { x + other.x, y + other.y };
	}
	Vec2 Vec2::operator- (const Vec2& other) const {
		return { x - other.x, y - other.y };
	}
	Vec2 Vec2::operator* (const Vec2& other) const {
		return { x * other.x, y * other.y };
	}
	Vec2 Vec2::operator/ (const Vec2& other) const {
		return { x / other.x, y / other.y };
	}
	Vec2 Vec2::operator* (Float scale) const {
		return {x * scale, y * scale };
	}
	Vec2 Vec2::operator/ (Float scale) const {
		return {x / scale, y / scale };
	}
	Vec2 Vec2::operator-() const {
		return { -x, -y };
	}

	Vec2& Vec2::operator+= (const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2& Vec2::operator-= (const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vec2& Vec2::operator*= (const Vec2& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}
	Vec2& Vec2::operator/= (const Vec2& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}
	Vec2& Vec2::operator*= (Float scale) {
		x *= scale;
		y *= scale;
		return *this;
	}
	Vec2& Vec2::operator/= (Float scale) {
		x /= scale;
		y /= scale;
		return *this;
	}

	constexpr Float Vec2::lengthSqrt() const {
		return x * x + y * y;
	}
	constexpr Float Vec2::length() const {
		return std::sqrt(lengthSqrt());
	}

	constexpr Float Vec2::dot(const Vec2& other) const {
		return x * other.x + y * other.y;
	}

	Float Vec2::distance(const Vec2& other) const {
		return (other - *this).length();
	}
	Vec2 Vec2::normalized() const {
		return *this / length();
	}
	Vec2 Vec2::swapComponents() const {
		return { y, x };
	}
	Vec2 Vec2::min(const Vec2& min) const {
		return { Min(x, min.x), Min(y, min.y) };
	}
	Vec2 Vec2::max(const Vec2& max) const {
		return { Max(x, max.x), Max(y, max.y) };
	}
	Vec2 Vec2::clamp(const Vec2& min, const Vec2& max) const {
		return { Clamp(x, min.x, max.x), Clamp(y, min.y, max.y) };
	}
	Vec2 Vec2::Lerp(const Vec2& min, const Vec2& max, Float amount) {
		return { Gctk::Lerp(min.x, max.x, amount), Gctk::Lerp(min.y, max.y, amount) };
	}

	Vec2 operator* (Float scale, const Vec2& vec) {
		return vec * scale;
	}
	Vec2 operator/ (Float scale, const Vec2& vec) {
		return { scale / vec.x, scale / vec.y };
	}

	const Vec2 Vec2::Zero = { 0, 0 };
	const Vec2 Vec2::UnitX = { 1, 0 };
	const Vec2 Vec2::UnitY = { 0, 1 };
	const Vec2 Vec2::One = { 1, 1 };

	Vec3::operator GlmVec3() const {
		return { x, y, z };
	}
	Vec3::operator std::array<Float, 3>() const {
		return { x, y, z };
	}

	Vec3 Vec3::operator+ (const Vec3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}
	Vec3 Vec3::operator- (const Vec3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	Vec3 Vec3::operator* (const Vec3& other) const {
		return { x * other.x, y * other.y, z * other.z };
	}
	Vec3 Vec3::operator/ (const Vec3& other) const {
		return { x / other.x, y / other.y, z / other.z };
	}
	Vec3 Vec3::operator* (Float scale) const {
		return { x * scale, y * scale, z * scale };
	}
	Vec3 Vec3::operator/ (Float scale) const {
		return { x / scale, y / scale, z / scale };
	}
	Vec3 Vec3::operator- () const {
		return { -x, -y, -z };
	}

	Vec3& Vec3::operator+= (const Vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}
	Vec3& Vec3::operator-= (const Vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}
	Vec3& Vec3::operator*= (const Vec3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}
	Vec3& Vec3::operator/= (const Vec3& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}
	Vec3& Vec3::operator*= (Float scale) {
		x *= scale;
		y *= scale;
		z *= scale;

		return *this;
	}
	Vec3& Vec3::operator/= (Float scale) {
		x /= scale;
		y /= scale;
		z /= scale;

		return *this;
	}

	constexpr Float Vec3::lengthSqrt() const {
		return x * x + y * y + z * z;
	}
	constexpr Float Vec3::length() const {
		return std::sqrt(lengthSqrt());
	}
	constexpr Float Vec3::dot(const Vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	Vec3 Vec3::cross(const Vec3& other) const {
		return {
			x * other.y - y * other.x,
			y * other.z - z * other.y,
			z * other.x - x * other.z
		};
	}
	Float Vec3::distance(const Vec3& other) const {
		return (other - *this).length();
	}
	Vec3 Vec3::normalized() const {
		return *this / length();
	}
	Vec3 Vec3::min(const Vec3& min) const {
		return { Min(x, min.x), Min(y, min.y), Min(z, min.z) };
	}
	Vec3 Vec3::max(const Vec3& max) const {
		return { Max(x, max.x), Max(y, max.y), Max(z, max.z) };
	}
	Vec3 Vec3::clamp(const Vec3& min, const Vec3& max) const {
		return { Clamp(x, min.x, max.x), Clamp(y, min.y, max.y), Clamp(z, min.z, max.z) };
	}
	Vec3 Vec3::Lerp(const Vec3& min, const Vec3& max, Float amount) {
		return { Gctk::Lerp(min.x, max.x, amount), Gctk::Lerp(min.y, max.y, amount), Gctk::Lerp(min.z, max.z, amount) };
	}

	const Vec3 Vec3::Zero = { 0, 0, 0 };
	const Vec3 Vec3::UnitX = { 1, 0, 0 };
	const Vec3 Vec3::UnitY = { 0, 1, 0 };
	const Vec3 Vec3::UnitZ = { 0, 0, 1 };
	const Vec3 Vec3::One = { 1, 1, 1 };

	Vec4::operator GlmVec4() const {
		return { x, y, z, w };
	}
	Vec4::operator std::array<Float, 4>() const {
		return { x, y, z, w };
	}

	Vec4 Vec4::operator+ (const Vec4& other) const {
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}
	Vec4 Vec4::operator- (const Vec4& other) const {
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}
	Vec4 Vec4::operator* (const Vec4& other) const {
		return { x * other.x, y * other.y, z * other.z, w * other.w };
	}
	Vec4 Vec4::operator/ (const Vec4& other) const {
		return { x / other.x, y / other.y, z / other.z, w / other.w };
	}
	Vec4 Vec4::operator* (Float scale) const {
		return { x * scale, y * scale, z * scale, w * scale };
	}
	Vec4 Vec4::operator/ (Float scale) const {
		return { x / scale, y / scale, z / scale, w / scale };
	}
	Vec4 Vec4::operator- () const {
		return { -x, -y, -z, -w };
	}

	Vec4& Vec4::operator+= (const Vec4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}
	Vec4& Vec4::operator-= (const Vec4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}
	Vec4& Vec4::operator*= (const Vec4& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}
	Vec4& Vec4::operator/= (const Vec4& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}
	Vec4& Vec4::operator*= (Float scale) {
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;

		return *this;
	}
	Vec4& Vec4::operator/= (Float scale) {
		x /= scale;
		y /= scale;
		z /= scale;
		w /= scale;

		return *this;
	}
	constexpr Float Vec4::lengthSqrt() const {
		return x * x + y * y + z * z + w * w;
	}
	constexpr Float Vec4::length() const {
		return std::sqrt(lengthSqrt());
	}
	constexpr Float Vec4::dot(const Vec4& other) const {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	Float Vec4::distance(const Vec4& other) const {
		return (other - *this).length();
	}
	Vec4 Vec4::normalized() const {
		return *this / length();
	}

	Vec4 Vec4::min(const Vec4& min) const {
		return { Min(x, min.x), Min(y, min.y), Min(z, min.z), Min(w, min.w) };
	}
	Vec4 Vec4::max(const Vec4& max) const {
		return {Max(x, max.x), Max(y, max.y), Max(z, max.z), Max(w, max.w) };
	}
	Vec4 Vec4::clamp(const Vec4& min, const Vec4& max) const {
		return {
			Clamp(x, min.x, max.x),
			Clamp(y, min.y, max.y),
			Clamp(z, min.z, max.z),
			Clamp(w, min.w, max.w)
		};
	}
	Vec4 Vec4::Lerp(const Vec4& min, const Vec4& max, Float amount) {
		return {
			Gctk::Lerp(min.x, max.x, amount),
			Gctk::Lerp(min.y, max.y, amount),
			Gctk::Lerp(min.z, max.z, amount),
			Gctk::Lerp(min.w, max.w, amount)
		};
	}

	const Vec4 Vec4::Zero = { 0, 0, 0, 0 };
	const Vec4 Vec4::UnitX = { 1, 0, 0, 0 };
	const Vec4 Vec4::UnitY = { 0, 1, 0, 0 };
	const Vec4 Vec4::UnitZ = { 0, 0, 1, 0 };
	const Vec4 Vec4::UnitW = { 0, 0, 0, 1 };
	const Vec4 Vec4::One = { 1, 1, 1, 1 };

	Quat::operator GlmQuat() const {
		return { w, x, y, z };
	}

	AxisAngle Quat::toAxisAngle() const {
		Vec3 axis = glm::axis(static_cast<GlmQuat>(*this));
		Angle angle = { glm::angle(static_cast<GlmQuat>(*this)), AngleKind::Radians };
		return { axis, angle };
	}
	Vec3 Quat::toEuler() const {
		return glm::eulerAngles(static_cast<GlmQuat>(*this));
	}

	Quat Quat::FromAxisAngle(const AxisAngle& axisAngle) {
		return FromEuler(axisAngle.axis * axisAngle.angle.asDegrees());
	}
	Quat Quat::FromEuler(const Vec3& euler) {
		return GlmQuat(static_cast<GlmVec3>(euler));
	}
	Quat Quat::LookAt(const Vec3& dir, const Vec3& up) {
		return glm::quatLookAt(static_cast<GlmVec3>(dir), static_cast<GlmVec3>(up));
	}

	Mat4::Mat4(const std::array<Float, 16>& array): Mat4() {
		std::memcpy(columns.data(), array.data(), sizeof(Float) * 16);
	}

	Mat4::operator GlmMat4() const {
		return {
			static_cast<GlmVec4>(columns[0]),
			static_cast<GlmVec4>(columns[1]),
			static_cast<GlmVec4>(columns[2]),
			static_cast<GlmVec4>(columns[3])
		};
	}
	Mat4::operator std::array<Vec4, 4>() const {
		return {
			static_cast<GlmVec4>(columns[0]),
			static_cast<GlmVec4>(columns[1]),
			static_cast<GlmVec4>(columns[2]),
			static_cast<GlmVec4>(columns[3])
		};
	}
	Mat4::operator std::array<Float, 16>() const {
		return {
			columns[0].x, columns[0].y, columns[0].z, columns[0].w,
			columns[1].x, columns[1].y, columns[1].z, columns[1].w,
			columns[2].x, columns[2].y, columns[2].z, columns[2].w,
			columns[3].x, columns[3].y, columns[3].z, columns[3].w
		};
	}

	constexpr bool Mat4::operator== (const Mat4& mat) const {
		return columns[0] == mat.columns[0] && columns[1] == mat.columns[1] && columns[2] == mat.columns[2] && columns[3] == mat.columns[3];
	}

	std::array<Vec4, 4> Mat4::rows() const {
		const GlmMat4 mat = static_cast<GlmMat4>(*this);
		return {
			glm::row(mat, 0),
			glm::row(mat, 1),
			glm::row(mat, 2),
			glm::row(mat, 3)
		};
	}
	Mat4 Mat4::transpose() const {
		return glm::transpose(static_cast<GlmMat4>(*this));
	}

	Mat4 Mat4::operator* (const Mat4& mat) const {
		return static_cast<GlmMat4>(*this) * static_cast<GlmMat4>(mat);
	}
	Mat4& Mat4::operator*=(const Mat4& mat) {
		GlmMat4* self = reinterpret_cast<GlmMat4*>(this);
		*self *= static_cast<GlmMat4>(mat);
		return *this;
	}

	const Float* Mat4::operator*() const {
		return asRawPtr();
	}

	Mat4 Mat4::translate(const Vec3& position) const {
		return glm::translate(static_cast<GlmMat4>(*this), static_cast<GlmVec3>(position));
	}
	Mat4 Mat4::scale(const Vec3& scale) const {
		return glm::scale(static_cast<GlmMat4>(*this), static_cast<GlmVec3>(scale));
	}
	Mat4 Mat4::rotate(const AxisAngle& aa) const {
		return glm::rotate(static_cast<GlmMat4>(*this), aa.angle.asRadians(), static_cast<GlmVec3>(aa.axis));
	}
	Mat4 Mat4::rotate(const Quat& quat) const {
		return rotate(quat.toAxisAngle());
	}

	Mat4 Mat4::CreateTranslation(const Vec3& position) {
		return glm::translate(GlmMat4(1.0), static_cast<GlmVec3>(position));
	}
	Mat4 Mat4::CreateScale(const Vec3& scale) {
		return glm::scale(GlmMat4(1.0), static_cast<GlmVec3>(scale));
	}
	Mat4 Mat4::CreateRotation(const AxisAngle& aa) {
		return glm::rotate(GlmMat4(1.0), aa.angle.asRadians(), static_cast<GlmVec3>(aa.axis));
	}
	Mat4 Mat4::CreateRotation(const Quat& quat) {
		return CreateRotation(quat.toAxisAngle());
	}
	Mat4 Mat4::CreateOrtho(Float left, Float right, Float bottom, Float top, Float near, Float far) {
		return glm::ortho(left, right, bottom, top, near, far);
	}
	Mat4 Mat4::CreatePerspective(const Angle& fov, Float aspect, Float near, Float far) {
		return glm::perspective(fov.asRadians(), aspect, near, far);
	}

	const Float* Mat4::asRawPtr() const {
		return glm::value_ptr(static_cast<GlmMat4>(*this));
	}

	const Mat4 Mat4::Identity = { };

	Color Color::FromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return {
			Float(r) / Float(255.0),
			Float(g) / Float(255.0),
			Float(b) / Float(255.0),
			Float(a) / Float(255.0)
		};
	}

	constexpr uint32_t Color::toRgba() const {
		uint8_t _r, _g, _b, _a;
		toBytes(_r, _g, _b, _a);
		return (_r << 24) | (_g << 16) | (_b << 8) | _a;
	}

	constexpr void Color::toBytes(uint8_t& r, uint8_t& g, uint8_t& b) const {
		r = uint8_t(this->r * 255);
		g = uint8_t(this->g * 255);
		b = uint8_t(this->b * 255);
	}
	constexpr void Color::toBytes(uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) const {
		toBytes(r, g, b);
		a = uint8_t(this->a * 255);
	}

#include "generated/gctk_colours_impl.hpp"
}
