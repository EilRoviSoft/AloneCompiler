#pragma once

//std
#include <string>

namespace alone {
	template<typename... Ts>
	struct overloaded : Ts... { using Ts::operator()...; };

	inline auto string_hasher = std::hash<std::string>();

	constexpr size_t gen_mask(size_t pos) {
		return 1ull << (pos - 1);
	}

	template <typename T>
	struct array_t {
		size_t size;
		T* ptr;
	};

	constexpr bool is_alpha(char c) {
		return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
	}
	constexpr bool is_binary(char c) {
		return c == '0' || c == '1';
	}
	constexpr bool is_numeric(char c) {
		return c >= '0' && c <= '9';
	}
	constexpr bool is_hexadecimal(char c) {
		return is_numeric(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
	}
	constexpr bool is_alpha_numeric(char c) {
		return is_alpha(c) || is_numeric(c);
	}
	constexpr bool is_whitespace(char c) {
		return c == ' ' || c == '\n' || c == '\t';
	}

	inline std::string gen_str(char a, char b) {
		std::string result;

		result.resize(b - a + 1);
		for(char i = a; i <= b; ++i)
			result[i - a] = i;

		return result;
	}
}