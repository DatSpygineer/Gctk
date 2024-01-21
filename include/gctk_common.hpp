#pragma once

#include <iostream>
#include <optional>
#include <vector>

#ifndef __GNUC__
	#if SIZE_MAX == UINT32_MAX
		typedef int32_t ssize_t;
	#else
		typedef int64_t ssize_t;
	#endif
#else
	#include <unistd.h>
#endif

namespace Gctk {
	template<typename T>
	using Option = std::optional<T>;

	template<typename T, typename E>
	class Result {
		static_assert(!std::is_same<T, E>(), "Template type \"T\" and \"E\" must be different!");

		Option<T> m_value;
		Option<E> m_error;

	public:
		Result(const Option<T>& value, const Option<E>& error): m_value(value), m_error(error) { }
		Result(const T& value): m_value({ value }), m_error(std::nullopt) { }
		Result(const E& error): m_value(std::nullopt), m_error({ error }) { }

		inline constexpr T unwrap() const {
			if (m_value.has_value()) {
				return *m_value;
			}
			throw std::runtime_error("Failed to unwrap result.");
		}
		inline constexpr T unwrap_or(const T& value) const {
			if (m_value.has_value()) {
				return *m_value;
			}
			return value;
		}

		inline constexpr Option<T> value() const { return m_value; }
		inline constexpr Option<E> error() const { return m_error; }

		[[nodiscard]] inline constexpr bool has_value() const { return m_value.has_value(); }
		[[nodiscard]] inline constexpr bool is_ok() const { return m_value.has_value(); }
		[[nodiscard]] inline constexpr bool is_err() const { return !m_value.has_value(); }
	};

	struct ByteReader {
		std::vector<uint8_t> m_data;
		size_t m_uPosition;
	public:
		using difference_type = std::vector<uint8_t>::difference_type;

		explicit ByteReader(size_t capacity = 0): m_data(), m_uPosition(0) { m_data.reserve(capacity); }
		ByteReader(std::vector<uint8_t>&& data): m_data(std::move(data)), m_uPosition(0) { }
		ByteReader(const std::vector<uint8_t>& data): m_data(data), m_uPosition(0) { }
		ByteReader(ByteReader&& reader) = default;
		ByteReader(const ByteReader& reader) = default;

		template<typename T>
		inline bool read(T& value) {
			std::vector<uint8_t> bytes;
			bytes.reserve(sizeof(T));
			read(bytes, sizeof(T));
			std::memcpy(&value, bytes.data(), sizeof(T));

			return true;
		}
		inline bool read(std::vector<uint8_t>& bytes, size_t n) {
			for (size_t i = 0; i < n; i++) {
				if (m_uPosition >= m_data.size()) {
					return false;
				}
				bytes.push_back(m_data.at(m_uPosition++));
			}
			return true;
		}
		inline uint8_t read_byte() {
			if (m_uPosition >= m_data.size()) {
				return 0;
			}
			return m_data.at(m_uPosition++);
		}

		inline constexpr size_t remaining() const { return m_data.size() - m_uPosition; }
		inline constexpr size_t position() const { return m_uPosition; }
		inline bool position(size_t pos) {
			if (pos >= m_data.size()) {
				return false;
			}
			m_uPosition = pos;
			return true;
		}
		inline bool offset(ssize_t off) {
			if (static_cast<ssize_t>(m_uPosition) + off >= m_data.size() && static_cast<ssize_t>(m_uPosition) + off > 0) {
				return false;
			}
			m_uPosition += off;
			return true;
		}

		inline constexpr auto begin() { return m_data.begin(); }
		[[nodiscard]] inline constexpr auto begin() const { return m_data.begin(); }
		inline constexpr auto end() { return m_data.end(); }
		[[nodiscard]] inline constexpr auto end() const { return m_data.end(); }
		inline constexpr auto current() {
			return m_data.begin() + static_cast<difference_type>(m_uPosition);
		}
		[[nodiscard]] inline constexpr auto current() const {
			return m_data.begin() + static_cast<difference_type>(m_uPosition);
		}
	};
}